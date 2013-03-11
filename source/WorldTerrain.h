/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include "WorldTerrainChunk.h"
#include <gc_cpp.h>

class Terrain;

/**
 * @brief Entity rendering the terrain.
 * 
 * The WorldTerrain object maintains a list of WorldTerrainChunks to divide
 * terrain rendering into multiple small chunks that may be view frustum culled.
 */

class WorldTerrain : public WorldGroup, public gc_cleanup
{
public:
	WorldTerrain();

	void setTerrain(Terrain* t);
	Terrain* getTerrain();

	void draw(const RenderInfo& info);

	void markChunksDirty();
	void updateChunks();
	void updateChunksIfDirty();

protected:
	Terrain* terrain;

	typedef std::vector <WorldTerrainChunk*, gc_allocator<WorldTerrainChunk*> > Chunks;
	Chunks chunks;
	bool chunksDirty;
};