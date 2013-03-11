/* Copyright © 2013 Fabian Schuiki */
#include "WorldTerrainChunk.h"
#include "Terrain.h"
#include "TerrainNode.h"
#include "TerrainCell.h"
#include "Logger.h"
#include <SFML/OpenGL.hpp>

inline void glVertexvec3(const vec3& v) { glVertex3f(v.x, v.y, v.z); }
inline void glNormalvec3(const vec3& v) { glNormal3f(v.x, v.y, v.z); }

bool operator != (const WorldTerrainChunk::Chunk& a, const WorldTerrainChunk::Chunk& b)
{
	return (a.x0 != b.x0 || a.x1 != b.x1 || a.y0 != b.y0 || a.y1 != b.y1);
}

WorldTerrainChunk::WorldTerrainChunk()
{
	dirty = false;
	terrain = NULL;
	chunk.x0 = 0;
	chunk.x1 = 0;
	chunk.y0 = 0;
	chunk.y1 = 0;
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
	for (int y = chunk.y0; y < chunk.y1; y++) {
		for (int x = chunk.x0 * 2; x < chunk.x1 * 2; x++) {
			TerrainCell& tc = terrain->getCell(x,y);
			Cell& c = cells[y * cx + x];
			c.modelCell = &tc;
			c.normal = tc.normal;

			c.nodes[0].pos = tc.nodes[0]->position;
			c.nodes[1].pos = tc.nodes[1]->position;
			c.nodes[2].pos = tc.nodes[2]->position;

			c.bounds.x0 = std::min(std::min(c.nodes[0].pos.x, c.nodes[1].pos.x), c.nodes[2].pos.x);
			c.bounds.y0 = std::min(std::min(c.nodes[0].pos.y, c.nodes[1].pos.y), c.nodes[2].pos.y);
			c.bounds.z0 = std::min(std::min(c.nodes[0].pos.z, c.nodes[1].pos.z), c.nodes[2].pos.z);
			c.bounds.x1 = std::max(std::max(c.nodes[0].pos.x, c.nodes[1].pos.x), c.nodes[2].pos.x);
			c.bounds.y1 = std::max(std::max(c.nodes[0].pos.y, c.nodes[1].pos.y), c.nodes[2].pos.y);
			c.bounds.z1 = std::max(std::max(c.nodes[0].pos.z, c.nodes[1].pos.z), c.nodes[2].pos.z);

			if (c.bounds.x0 < nodeBox.x0) nodeBox.x0 = c.bounds.x0;
			if (c.bounds.y0 < nodeBox.y0) nodeBox.y0 = c.bounds.y0;
			if (c.bounds.z0 < nodeBox.z0) nodeBox.z0 = c.bounds.z0;
			if (c.bounds.x1 > nodeBox.x1) nodeBox.x1 = c.bounds.x1;
			if (c.bounds.y1 > nodeBox.y1) nodeBox.y1 = c.bounds.y1;
			if (c.bounds.z1 > nodeBox.z1) nodeBox.z1 = c.bounds.z1;
		}
	}

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

	// Draw the individual cells.
	for (Cells::iterator it = cells.begin(); it != cells.end(); it++) {
		Cell& c = *it;

		double g = c.modelCell->nodes[1]->gray;
		vec3 col(g, g, g);
		glColor3f(col.x, col.y, col.z);
		glNormalvec3(c.normal);
		glBegin(GL_TRIANGLES);
		glVertex3f(c.nodes[0].pos.x, c.nodes[0].pos.y, c.nodes[0].pos.z);
		glVertex3f(c.nodes[1].pos.x, c.nodes[1].pos.y, c.nodes[1].pos.z);
		glVertex3f(c.nodes[2].pos.x, c.nodes[2].pos.y, c.nodes[2].pos.z);
		glEnd();

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
}

WorldTerrainChunk::Cell::Cell()
{
	modelCell = NULL;
	for (int i = 0; i < 3; i++) {
		subcells[i] = NULL;
	}
}