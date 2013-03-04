/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include "TerrainCell.h"
#include "TerrainNode.h"
#include <gc_cpp.h>
#include <vector>

/**
 * Model class that maintains the world's terrain data.
 */

class Terrain : public gc
{
public:
	const unsigned int width;
	const unsigned int height;

	Terrain(unsigned int w, unsigned int h);

	typedef std::vector<TerrainCell> Cells;
	typedef std::vector<TerrainNode> Nodes;

	Cells cells;
	Nodes nodes;

	void analyze();

protected:
	unsigned int num_cells_x, num_cells_y;
	unsigned int num_nodes_x, num_nodes_y;
	void resize(unsigned int w, unsigned int h);
};