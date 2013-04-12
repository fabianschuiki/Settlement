/* Copyright © 2013 Fabian Schuiki */
#include "Terrain.h"
#include "model/Building.h"
#include <cassert>

Terrain::Terrain(unsigned int w, unsigned int h) : width(w), height(h)
{
	resize(w, h);
}

/**
 * @brief Resizes the internal storage for the terrain to accomodate w x h
 *        nodes.
 *
 * Gathers groups of 3 nodes into cells, and calculates each node's adjacent
 * nodes and touching cells.
 */
void Terrain::resize(unsigned int w, unsigned int h)
{
	// Reserve enough memory for all nodes and cells.
	num_nodes_x = w;
	num_nodes_y = h;
	num_cells_x = (w - 1) * 2;
	num_cells_y = (h - 1);
	nodes.reserve(num_nodes_x * num_nodes_y);
	cells.reserve(num_cells_x * num_cells_y);

	// Gather nodes into cells.
	for (int y = 0; y < num_cells_y; y++) {
		for (int x = 0; x < num_cells_x - 1; x += 2) {
			TerrainCell &c0 = cells[y * num_cells_x + x + 0];
			TerrainCell &c1 = cells[y * num_cells_x + x + 1];
			c0.building = NULL;
			c1.building = NULL;

			TerrainNode &n0 = nodes[y * num_nodes_x + x/2];
			TerrainNode &n1 = nodes[y * num_nodes_x + x/2 + 1];
			TerrainNode &n2 = nodes[(y + 1) * num_nodes_x + x/2 + 1];
			TerrainNode &n3 = nodes[(y + 1) * num_nodes_x + x/2];

			if (y & 1) {
				c0.nodes[0] = &n0;
				c0.nodes[1] = &n2;
				c0.nodes[2] = &n3;

				c1.nodes[0] = &n0;
				c1.nodes[1] = &n1;
				c1.nodes[2] = &n2;
			} else {
				c0.nodes[0] = &n0;
				c0.nodes[1] = &n1;
				c0.nodes[2] = &n3;

				c1.nodes[0] = &n1;
				c1.nodes[1] = &n2;
				c1.nodes[2] = &n3;
			}
		}
	}

	// Gather cells per node and adjacent nodes..
	const struct { int x; int y; } cellOffsets[6] = {
		{-2, -1}, {-1, -1}, {0, -1}, {0, 0}, {-1, 0}, {-2, 0}
	};
	const struct { int x; int y; } nodeOffsets[6] = {
		{0, -1}, {1, -1}, {-1, 0}, {1, 0}, {0, 1}, {1, 1}
	};
	for (int y = 0; y < num_nodes_y; y++) {
		for (int x = 0; x < num_nodes_x; x++) {
			TerrainNode &node = nodes[y * num_nodes_x + x];
			node.ridge = false;
			node.riverbed = false;

			// Cells touching this node.
			for (int d = 0; d < 6; d++) {
				int nx = x * 2 + cellOffsets[d].x;
				int ny = y + cellOffsets[d].y;
				if (nx >= 0 && nx < num_cells_x && ny >= 0 && ny < num_cells_y) {
					node.cells[d] = &cells[ny * num_cells_x + nx];
				} else {
					node.cells[d] = NULL;
				}
			}

			// Adjacent nodes.
			for (int d = 0; d < 6; d++) {
				int nx = x + nodeOffsets[d].x;
				int ny = y + nodeOffsets[d].y;
				if ((y & 1) == 0 && (ny & 1) != 0) nx--;
				if (nx >= 0 && nx < num_nodes_x && ny >= 0 && ny < num_nodes_y) {
					node.adjacentNodes[d] = &nodes[ny * num_nodes_x + nx];
				} else {
					node.adjacentNodes[d] = NULL;
				}
			}
		}
	}
}

/**
 * @brief Analyzes the terrain.
 *
 * Calculates which nodes may be part of a ridge line or riverbed lines.
 */
void Terrain::analyze()
{
	const double yscale = 5;

	for (int y = 0; y < num_nodes_x; y++) {
		for (int x = 0; x < num_nodes_x; x++) {
			TerrainNode& node = nodes[y * num_nodes_x + x];

			// Calculate the node's position.
			node.position.x = x + 0.5 * (y & 1);
			node.position.y = node.elevation * yscale;
			node.position.z = y;

			// Count the number of adjacent nodes that are above/below this
			// node.
			int above = 0;
			int below = 0;
			for (int i = 0; i < 6; i++) {
				TerrainNode* adjacent = node.adjacentNodes[i];
				if (adjacent) {
					if (adjacent->elevation < node.elevation) below++;
					if (adjacent->elevation > node.elevation) above++;
				}
			}

			// This is a mountain tip if all surrounding nodes are lower.
			node.mountainTip = (below == 6);
			node.trenchFloor = (above == 6);
		}
	}

	// Calculates the normal for each cell.
	for (int y = 0; y < num_cells_y; y++) {
		for (int x = 0; x < num_cells_x; x++) {
			TerrainCell& cell = cells[y * num_cells_x + x];
			vec3 va = (cell.nodes[2]->position - cell.nodes[0]->position);
			vec3 vb = (cell.nodes[1]->position - cell.nodes[0]->position);
			cell.normal = va.cross(vb);
			cell.normal.normalize();
		}
	}

	// Calculate the normal for each node.
	for (int y = 0; y < num_nodes_y; y++) {
		for (int x = 0; x < num_nodes_x; x++) {
			TerrainNode& node = nodes[y * num_nodes_x + x];
			vec3 n;
			for (int i = 0; i < 6; i++) {
				if (node.cells[i])
					n += node.cells[i]->normal;
			}
			n.normalize();
			node.normal = n;
		}
	}
}

double Terrain::getElevation(int x, int y)
{
	if (x < 0) x = 0;
	if (y < 0) y = 0;
	if (x >= width) x = width-1;
	if (y >= height) y = height-1;
	return getNode(x,y).position.y;
}

void Terrain::addBuilding(model::Building *b)
{
	assert(buildings.count(b) == 0 && "Building is already added to the terrain.");
	buildings.insert(b);

	// Position the building in the middle of its occupied cells.
	b->position = vec3();
	for (model::Building::Cells::iterator it = b->cells.begin(); it != b->cells.end(); it++) {
		TerrainCell *c = *it;
		c->building = b;
		b->position += c->nodes[0]->position;
		b->position += c->nodes[1]->position;
		b->position += c->nodes[2]->position;
	}
	b->position /= b->cells.size() * 3;
}

void Terrain::removeBuilding(model::Building *b)
{
	assert(buildings.count(b) != 0 && "Building is not part of the terrain.");
	buildings.erase(b);

	// Remove the building from the occupied cells.
	for (model::Building::Cells::iterator it = b->cells.begin(); it != b->cells.end(); it++) {
		TerrainCell *c = *it;
		assert(c->building == b && "Removing building from cell that has not the building set as its occupying structure.");
		c->building = NULL;
	}
}