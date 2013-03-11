/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include "WorldGroup.h"
#include "AABox.h"
#include "RenderInfo.h"
#include "Vector.h"
#include "gl/Texture.h"
#include "gl/Buffer.h"
#include <vector>
#include <gc_cpp.h>
#include <gc_allocator.h>

class Terrain;
class TerrainCell;

/**
 * @brief A chunk of terrain to be rendered.
 *
 * This class renders a specific segment of terrain from the simulation. It's
 * supposed to be part of a bigger WorldGroup that contains the rest of the
 * world in some form.
 */
class WorldTerrainChunk : public WorldGroup, public gc_cleanup
{
public:
	WorldTerrainChunk();

	void setTerrain(Terrain* t);
	Terrain* getTerrain();

	struct Chunk { int x0, x1, y0, y1; };
	void setChunk(Chunk c);
	const Chunk& getChunk();

	virtual void updateBounds();
	
	void update();
	void updateIfDirty();
	void markDirty();

	void draw(const RenderInfo &info);

	struct Node
	{
		vec3 pos;
		vec3 normal;
	};

	struct Cell
	{
		TerrainCell* modelCell;
		AABox bounds;
		Node nodes[3];
		Cell* subcells[4];
		vec3 normal;

		Cell();
	};

protected:
	Terrain* terrain;
	Chunk chunk;
	bool dirty;

	// calculated
	AABox nodeBox;
	typedef std::vector <Cell, gc_allocator<Cell> > Cells;
	Cells cells;

	// debug texture
	gl::Texture texture;

	// Mesh Buffers
	gl::Buffer vertexBuffer;
	gl::Buffer indexBuffer;
};