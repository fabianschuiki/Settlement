/* Copyright © 2013 Fabian Schuiki */
#include "Terrain.h"

Terrain::Terrain(unsigned int w, unsigned int h) : width(w), height(h)
{
	resize(w, h);
}

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

			TerrainNode &n0 = nodes[y * num_nodes_x + x];
			TerrainNode &n1 = nodes[y * num_nodes_x + x + 1];
			TerrainNode &n2 = nodes[(y + 1) * num_nodes_x + x + 1];
			TerrainNode &n3 = nodes[(y + 1) * num_nodes_x + x];

			c0.nodes[0] = &n0;
			c0.nodes[1] = &n1;
			c0.nodes[2] = &n3;

			c1.nodes[0] = &n1;
			c1.nodes[1] = &n2;
			c1.nodes[2] = &n3;
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

			// Cells touching this node.
			int cellIndex = 0;
			for (int d = 0; d < 6; d++) {
				int nx = x + cellOffsets[d].x;
				int ny = y + cellOffsets[d].y;
				node.cells[d] = NULL;
				if (nx >= 0 && nx < num_cells_x && ny >= 0 && ny < num_cells_y) {
					node.cells[cellIndex++] = &cells[ny * num_cells_x + nx];
				}
			}

			// Adjacent nodes.
			int nodeIndex = 0;
			for (int d = 0; d < 6; d++) {
				int nx = x + nodeOffsets[d].x;
				int ny = y + nodeOffsets[d].y;
				if ((y & 1) == 0 && (ny & 1) != 0) nx--;
				node.adjacentNodes[d] = NULL;
				if (nx >= 0 && nx < num_nodes_x && ny >= 0 && ny < num_nodes_y) {
					node.adjacentNodes[nodeIndex++] = &nodes[ny * num_nodes_x + nx];
				}
			}
		}
	}
}