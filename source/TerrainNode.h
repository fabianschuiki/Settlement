/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include "Vector.h"
#include <gc_cpp.h>

/**
 * @brief A single node of the terrain.
 * A node is the corner of 1-6 different terrain cells. It has up to 6
 * adjacent nodes, i.e. nodes that share the same cell as this node.
 */

class TerrainNode : public gc
{
public:

	/// The terrain elevation at this point.
	double elevation;

	/// Position of the node.
	vec3 position;

	/**
	 * @brief Normal of the node.
	 * This actually is an average of the normals of the cells touching this
	 * node.
	 */
	vec3 normal;

	double gray;
	bool ridge;
	bool riverbed;
	bool mountainTip;
	bool trenchFloor;

	/**
	 * @brief The six nodes closest to this.
	 * Note that some entries may be NULL for nodes at the edge of the terrain.
	 */
	TerrainNode* adjacentNodes[6];

	/**
	 * @brief Cells containing this node.
	 */
	TerrainCell* cells[6];
};