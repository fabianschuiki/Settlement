/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include "TerrainCell.h"
#include "TerrainNode.h"
#include <gc_cpp.h>

/**
 * Model class that maintains the world's terrain data.
 */

class Terrain : public gc
{
public:
	const unsigned int width;
	const unsigned int height;

	Terrain(unsigned int w, unsigned int h);

protected:
	typedef std::vector<TerrainCell> CellVector;
	typedef std::vector<TerrainNode> NodeVector;
};