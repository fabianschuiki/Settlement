/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include <gc_cpp.h>

class TerrainNode;

/**
 * @brief A cell of the terrain.
 * I.e. a single triangle consisting of three corner
 * nodes. A cell holds additional occupation information, such as what type
 * of building is built ontop of it, what terrain type it covers and other
 * calculated attributes.
 */
class TerrainCell : public gc
{
public:
	/// The three corner nodes of this cell.
	TerrainNode* nodes[3];
};