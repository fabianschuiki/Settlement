/* Copyright © 2013 Fabian Schuiki */
#include "TerrainRenderer.h"
#include "Terrain.h"
#include <SFML/OpenGL.hpp>


TerrainRenderer::TerrainRenderer()
{
	w = 0;
	h = 0;

	/*// Initialize a temporary terrain.
	const double ax = 0.2;
	const double ay = sqrt(3)/2 * ax;
	for (int x = 0; x < 128; x++) {
		for (int y = 0; y < 128; y++) {
			struct Node &node = nodes[x][y];
			node.p.x = ((x - 64) + 0.5 * (y & 1)) * ax;
			node.p.z = (y - 64) * ay;
			node.p.y = 0;
		}
	}

	// Calculate the normals of the terrain.
	for (int x = 0; x < 128; x++) {
		for (int y = 0; y < 128; y++) {
			struct Node &node = nodes[x][y];
			node.n.x = 0;
			node.n.z = 0;
			node.n.y = 1;
		}
	}*/
}

void TerrainRenderer::draw(const RenderInfo &info)
{
	// Draw the terrain.
	glBegin(GL_TRIANGLES);
	for (int x = 0; x < w-1; x++) {
		for (int y = 0; y < h-2; y += 2) {
			#define node(_x,_y) nodes[(_y) * w + (_x)]

			struct Node &a = node(x, y);
			struct Node &b = node(x+1, y);
			struct Node &c = node(x, y+1);
			struct Node &d = node(x+1, y+1);
			struct Node &e = node(x, y+2);
			struct Node &f = node(x+1, y+2);

			#define vertex(v) glColor3f(v.c.x, v.c.y, v.c.z); glNormal3f(v.n.x, v.n.y, v.n.z); glVertex3f(v.p.x, v.p.y, v.p.z)

			vertex(a);
			vertex(b);
			vertex(c);

			vertex(b);
			vertex(d);
			vertex(c);

			vertex(c);
			vertex(f);
			vertex(e);

			vertex(c);
			vertex(d);
			vertex(f);
		}
	}
	glEnd();
}

void TerrainRenderer::setTerrain(Terrain *t)
{
	if (terrain != t) {
		terrain = t;

		// TODO: mark terrain as invalid or something similar
	}
}

/**
 * Recalculates the terrain vertex data.
 */
void TerrainRenderer::update()
{
	// Copy the required information from the terrain.
	w = terrain->width;
	h = terrain->height;
	nodes.resize(w * h);

	// Calculate the vertices.
	const double ax = 0.2;
	const double ay = sqrt(3)/2 * ax;
	const int hw = w/2;
	const int hh = h/2;
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			TerrainNode &tn = terrain->nodes[y * terrain->width + x];
			Node &n = nodes[y * w + x];

			n.p.x = ((x - hw) + 0.5 * (y & 1)) * ax;
			n.p.z = (y - hh) * ay;
			n.p.y = tn.elevation;

			n.c = vec3(tn.gray, tn.gray, tn.gray);
		}
	}
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			Node &node = nodes[y * w + x];
			node.n.x = 0;
			node.n.z = 0;
			node.n.y = 1;
		}
	}
}