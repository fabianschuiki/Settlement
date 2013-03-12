/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include "WorldTerrainChunk.h"
#include "Geometry.h"
#include "Console.h"
#include <gc_cpp.h>

class Terrain;

/**
 * @brief Entity rendering the terrain.
 * 
 * The WorldTerrain object maintains a list of WorldTerrainChunks to divide
 * terrain rendering into multiple small chunks that may be view frustum culled.
 */

class WorldTerrain : public WorldGroup, public ConsoleCommandProvider, public gc_cleanup
{
public:
	WorldTerrain();

	void setTerrain(Terrain* t);
	Terrain* getTerrain();

	void draw(const RenderInfo& info);

	void markChunksDirty();
	void updateChunks();
	void updateChunksIfDirty();

	typedef std::set <TerrainCell*, std::less<TerrainCell*>, gc_allocator<TerrainCell*> > TerrainCells;
	TerrainCell* findClickedCell(const Line& clickRay);
	TerrainCells intersectCells(const Line& l);

	ConsoleCommandGroup getConsoleCommands();

protected:
	Terrain* terrain;

	typedef std::vector <WorldTerrainChunk*, gc_allocator<WorldTerrainChunk*> > Chunks;
	Chunks chunks;
	bool chunksDirty;
	int chunkStride;

private:
	ConsoleCommandGroup cli;
	void cli_updatechunks(ConsoleCall&);
	void cli_chunk(ConsoleCall&);
};