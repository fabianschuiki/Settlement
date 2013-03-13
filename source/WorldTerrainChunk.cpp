/* Copyright © 2013 Fabian Schuiki */
#include "WorldTerrainChunk.h"
#include "Terrain.h"
#include "TerrainNode.h"
#include "TerrainCell.h"
#include "Logger.h"
#include "gl/Primitives.h"
#include "Perlin.h"
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics/Image.hpp>

struct Vertex {
	GLfloat x,y,z;
	GLfloat nx,ny,nz;
	GLfloat cr,cg,cb;
	GLfloat u,v;
};

inline void glVertexvec3(const vec3& v) { glVertex3f(v.x, v.y, v.z); }
inline void glNormalvec3(const vec3& v) { glNormal3f(v.x, v.y, v.z); }

bool operator != (const WorldTerrainChunk::Chunk& a, const WorldTerrainChunk::Chunk& b)
{
	return (a.x0 != b.x0 || a.x1 != b.x1 || a.y0 != b.y0 || a.y1 != b.y1);
}

double smoothNoise(double x, double z, int octaves)
{
	double v = 0;
	double f = 1;
	double a = 1;
	double atot = 0;
	for (int i = 0; i < octaves; i++) {
		v += Perlin::noise2(x * f, z * f) * a;
		atot += a;
		f *= 2;
		a *= 0.5;
	}
	return v / atot;
}

WorldTerrainChunk::WorldTerrainChunk() : vertexBuffer(GL_ARRAY_BUFFER), indexBuffer(GL_ELEMENT_ARRAY_BUFFER)
{
	dirty = false;
	terrain = NULL;
	chunk.x0 = 0;
	chunk.x1 = 0;
	chunk.y0 = 0;
	chunk.y1 = 0;
	lod = 0;

	// load the debug grass texture
	texture.wrap = GL_REPEAT;
	texture.updateParameters();
	sf::Image img;
	img.loadFromFile("assets/grass3.png");
	glBindTexture(GL_TEXTURE_2D, texture.id);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.getSize().x, img.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.getPixelsPtr());
}

void WorldTerrainChunk::setTerrain(Terrain* t)
{
	if (terrain != t) {
		terrain = t;
		markDirty();
	}
}

Terrain* WorldTerrainChunk::getTerrain()
{
	return terrain;
}

void WorldTerrainChunk::setChunk(Chunk c)
{
	if (chunk != c) {
		chunk = c;
		markDirty();
	}
}

const WorldTerrainChunk::Chunk& WorldTerrainChunk::getChunk()
{
	return chunk;
}

void WorldTerrainChunk::updateBounds()
{
	WorldGroup::updateBounds();

	// The bounding volume now contains all children. All that's left is to add
	// the bounding box covering all our nodes.
	bounds.add(nodeBox);
}

void gatherBufferData(std::vector<Vertex>& vertices, std::vector<GLint>& indices, WorldTerrainChunk::Cell& cell)
{
	if (cell.subcells) {
		for (int i = 0; i < 4; i++) {
			gatherBufferData(vertices, indices, cell.subcells[i]);
		}
	} else {
		for (int i = 0; i < 3; i++) {
			WorldTerrainChunk::Node& n = cell.nodes[i];
			indices.push_back(vertices.size());
			Vertex v;
			v.x = n.pos.x;
			v.y = n.pos.y;
			v.z = n.pos.z;
			v.nx = n.normal.x;
			v.ny = n.normal.y;
			v.nz = n.normal.z;
			v.cr = n.color.x;
			v.cg = n.color.y;
			v.cb = n.color.z;
			v.u = v.x * 0.25;
			v.v = v.z * 0.25;
			vertices.push_back(v);
		}
	}
}

void WorldTerrainChunk::update()
{
	dirty = false;

	// Clear the nodes and cells.
	cells.clear();
	if (!terrain) return;

	// Create the first layer of cells taken from the terrain.
	nodeBox.x0 = nodeBox.y0 = nodeBox.z0 = INT_MAX;
	nodeBox.x1 = nodeBox.y1 = nodeBox.z1 = INT_MIN;
	unsigned int cx = (chunk.x1 - chunk.x0) * 2;
	unsigned int cy = (chunk.y1 - chunk.y0);
	cells.resize(cx * cy);
	for (int y = 0; y < cy; y++) {
		for (int x = 0; x < cx; x++) {
			TerrainCell& tc = terrain->getCell(x + chunk.x0 * 2, y + chunk.y0);
			Cell& c = cells[y * cx + x];
			c.modelCell = &tc;
			c.normal = tc.normal;

			c.nodes[0].color = vec3(1,1,1);
			c.nodes[1].color = vec3(1,1,1);
			c.nodes[2].color = vec3(1,1,1);

			for (int i = 0; i < 6; i++) {
				if (&tc == terrain->getNode(4,4).cells[i]) {
					c.nodes[0].color = vec3(0,0,1);
					c.nodes[1].color = vec3(0,0,1);
					c.nodes[2].color = vec3(0,0,1);
				}
			}
			for (int i = 0; i < 3; i++) {
				if (tc.nodes[i] == &terrain->getNode(4,4)) {
					c.nodes[i].color = vec3(1,0,0);
				}
			}

			c.nodes[0].pos = tc.nodes[0]->position;
			c.nodes[1].pos = tc.nodes[1]->position;
			c.nodes[2].pos = tc.nodes[2]->position;
			c.nodes[0].normal = tc.nodes[0]->normal;
			c.nodes[1].normal = tc.nodes[1]->normal;
			c.nodes[2].normal = tc.nodes[2]->normal;

			c.bounds.x0 = std::min(std::min(c.nodes[0].pos.x, c.nodes[1].pos.x), c.nodes[2].pos.x);
			c.bounds.y0 = std::min(std::min(c.nodes[0].pos.y, c.nodes[1].pos.y), c.nodes[2].pos.y);
			c.bounds.z0 = std::min(std::min(c.nodes[0].pos.z, c.nodes[1].pos.z), c.nodes[2].pos.z);
			c.bounds.x1 = std::max(std::max(c.nodes[0].pos.x, c.nodes[1].pos.x), c.nodes[2].pos.x);
			c.bounds.y1 = std::max(std::max(c.nodes[0].pos.y, c.nodes[1].pos.y), c.nodes[2].pos.y);
			c.bounds.z1 = std::max(std::max(c.nodes[0].pos.z, c.nodes[1].pos.z), c.nodes[2].pos.z);

			activateLevelOfDetail(c, lod);

			if (c.bounds.x0 < nodeBox.x0) nodeBox.x0 = c.bounds.x0;
			if (c.bounds.y0 < nodeBox.y0) nodeBox.y0 = c.bounds.y0;
			if (c.bounds.z0 < nodeBox.z0) nodeBox.z0 = c.bounds.z0;
			if (c.bounds.x1 > nodeBox.x1) nodeBox.x1 = c.bounds.x1;
			if (c.bounds.y1 > nodeBox.y1) nodeBox.y1 = c.bounds.y1;
			if (c.bounds.z1 > nodeBox.z1) nodeBox.z1 = c.bounds.z1;
		}
	}

	// Update the mesh buffers.
	std::vector <Vertex> vertices;
	std::vector <GLint> indices;

	// For now simply copy the vertices and normals for each cell's node
	// individually. This could be optimized later.
	for (Cells::iterator it = cells.begin(); it != cells.end(); it++) {
		gatherBufferData(vertices, indices, *it);
		/*Cell& c = *it;
		for (int i = 0; i < 3; i++) {
			Node& n = c.nodes[i];
			indices.push_back(vertices.size());
			Vertex v;
			v.x = n.pos.x;
			v.y = n.pos.y;
			v.z = n.pos.z;
			v.nx = n.normal.x;
			v.ny = n.normal.y;
			v.nz = n.normal.z;
			v.cr = n.color.x * ((lod + 1) & 1);
			v.cg = n.color.y * ((lod/2 + 1) & 1);
			v.cb = n.color.z * ((lod/4 + 1) & 1);
			v.u = v.x * 0.25;
			v.v = v.z * 0.25;
			vertices.push_back(v);
		}*/
	}

	LOG(kLogDebug, "Chunk %p: Loading %i vertices, %i indices", this, vertices.size(), indices.size());
	vertexBuffer.loadData(vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	indexBuffer.loadData(indices.size() * sizeof(GLint), &indices[0], GL_STATIC_DRAW);
	num_indices = indices.size();

	// Since we have changed our layout, the bounding box is not valid anymore.
	markBoundsDirty();
}

void WorldTerrainChunk::updateIfDirty()
{
	if (dirty)
		update();
}

void WorldTerrainChunk::markDirty()
{
	if (!dirty) {
		dirty = true;
		// do other stuff ...
	}
}

void WorldTerrainChunk::draw(const RenderInfo &info)
{
	if (info.drawBounds & RenderInfo::kTerrainBounds) {
		glColor3f(0,1,0);
		bounds.draw(info);
	}
	if (info.drawBounds & RenderInfo::kTerrainCellBounds) {
		glColor3f(1,1,0);
		for (Cells::iterator it = cells.begin(); it != cells.end(); it++)
			gl::drawOutline((*it).bounds);
	}

	#define autotex(v) glTexCoord2f(v.x * 0.25, v.z * 0.25);

	// Draw the individual cells.
	/*glEnable(GL_TEXTURE_2D);
	for (Cells::iterator it = cells.begin(); it != cells.end(); it++) {
		Cell& c = *it;

		glColor3f(1,1,1);
		glBindTexture(GL_TEXTURE_2D, texture.id);
		glBegin(GL_TRIANGLES);
		glNormalvec3(c.nodes[0].normal);
		autotex(c.nodes[0].pos);
		glVertex3f(c.nodes[0].pos.x, c.nodes[0].pos.y, c.nodes[0].pos.z);
		glNormalvec3(c.nodes[1].normal);
		autotex(c.nodes[1].pos);
		glVertex3f(c.nodes[1].pos.x, c.nodes[1].pos.y, c.nodes[1].pos.z);
		glNormalvec3(c.nodes[2].normal);
		autotex(c.nodes[2].pos);
		glVertex3f(c.nodes[2].pos.x, c.nodes[2].pos.y, c.nodes[2].pos.z);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);

		if (info.drawNormals & RenderInfo::kTerrainNormals) {
			vec3 p0 = (c.nodes[0].pos + c.nodes[1].pos + c.nodes[2].pos) / 3;
			vec3 p1 = p0 + c.normal * 0.5;
			glColor3f(1,0,0);
			glBegin(GL_LINES);
			glVertexvec3(p0);
			glVertexvec3(p1);
			glEnd();
		}
	}
	glDisable(GL_TEXTURE_2D);*/

	if (info.drawNormals & RenderInfo::kTerrainNormals) {
		glBegin(GL_LINES);
		if (info.drawNormals & RenderInfo::kTerrainCellNormals) {
			glColor3f(1,0,0);
			for (Cells::iterator it = cells.begin(); it != cells.end(); it++) {
				Cell& c = *it;
				vec3 p0 = (c.nodes[0].pos + c.nodes[1].pos + c.nodes[2].pos) / 3;
				vec3 p1 = p0 + c.normal * 0.5;
				glVertexvec3(p0);
				glVertexvec3(p1);
			}
		}
		if (info.drawNormals & RenderInfo::kTerrainNodeNormals) {
			glColor3f(1,1,0);
			for (Cells::iterator it = cells.begin(); it != cells.end(); it++) {
				Cell& c = *it;
				for (int i = 0; i < 3; i++) {
					vec3 p0 = c.nodes[i].pos;
					vec3 p1 = p0 + c.nodes[i].normal * 0.5;
					glVertexvec3(p0);
					glVertexvec3(p1);
				}
			}
		}
		glEnd();
	}

	vertexBuffer.bind();
	indexBuffer.bind();

	if (!highlight) glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture.id);

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), &((Vertex*)NULL)->x);
	glNormalPointer(GL_FLOAT, sizeof(Vertex), &((Vertex*)NULL)->nx);
	glColorPointer(3, GL_FLOAT, sizeof(Vertex), &((Vertex*)NULL)->cr);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &((Vertex*)NULL)->u);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_LIGHTING);
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, 0);
	glDisable(GL_LIGHTING);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glDisable(GL_TEXTURE_2D);

	vertexBuffer.unbind();
	indexBuffer.unbind();

	// Draw the highlighted cells.
	glColor3f(1,1,0);
	for (HighlightedCells::iterator it = highlightedCells.begin(); it != highlightedCells.end(); it++) {
		Cell* cell = *it;
		glBegin(GL_LINE_STRIP);
		glVertexvec3(cell->nodes[0].pos + cell->normal * 0.1);
		glVertexvec3(cell->nodes[1].pos + cell->normal * 0.1);
		glVertexvec3(cell->nodes[2].pos + cell->normal * 0.1);
		glVertexvec3(cell->nodes[0].pos + cell->normal * 0.1);
		glEnd();
	}
}

WorldTerrainChunk::Cell::Cell()
{
	modelCell = NULL;
	subcells = NULL;
}

/**
 * Changes the level of detail to be used for the chunk.
 */
void WorldTerrainChunk::setLevelOfDetail(int l)
{
	if (lod != l) {
		lod = l;
		markDirty();
	}
}

int WorldTerrainChunk::getLevelOfDetail()
{
	return lod;
}

void WorldTerrainChunk::activateLevelOfDetail(Cell& c, int lod)
{
	const double nscale = 1;
	const double namplitude = 0.5;
	if (lod > 0) {
		Node inter[3];
		#define interpolate(dst, a, b) \
			dst.pos = (a.pos + b.pos) * 0.5; \
			dst.color = (a.color + b.color) * 0.5; \
			dst.normal = (a.normal + b.normal).unit()
		interpolate(inter[0], c.nodes[0], c.nodes[1]);
		interpolate(inter[1], c.nodes[1], c.nodes[2]);
		interpolate(inter[2], c.nodes[2], c.nodes[0]);

		for (int i = 0; i < 3; i++)
			inter[i].pos += inter[i].normal * smoothNoise(inter[i].pos.x * nscale, inter[i].pos.z * nscale, 8) * namplitude;

		c.subcells = new (GC) Cell [4];

		c.subcells[0].nodes[0] = c.nodes[0];
		c.subcells[0].nodes[1] = inter[0];
		c.subcells[0].nodes[2] = inter[2];

		c.subcells[1].nodes[0] = inter[0];
		c.subcells[1].nodes[1] = inter[1];
		c.subcells[1].nodes[2] = inter[2];

		c.subcells[2].nodes[0] = inter[0];
		c.subcells[2].nodes[1] = c.nodes[1];
		c.subcells[2].nodes[2] = inter[1];

		c.subcells[3].nodes[0] = inter[2];
		c.subcells[3].nodes[1] = inter[1];
		c.subcells[3].nodes[2] = c.nodes[2];

		for (int i = 0; i < 4; i++) {
			activateLevelOfDetail(c.subcells[i], lod-1);
		}
	}
}