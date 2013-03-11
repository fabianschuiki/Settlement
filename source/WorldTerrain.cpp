/* Copyright © 2013 Fabian Schuiki */
#include "WorldTerrain.h"
#include "Logger.h"
#include "Terrain.h"


WorldTerrain::WorldTerrain()
{
	terrain = NULL;
	chunksDirty = false;
}

/**
 * Changes the terrain model this entity is supposed to draw. This will cause
 * the chunks to be marked as dirty which will result in an update to this
 * entity's structure.
 */
void WorldTerrain::setTerrain(Terrain* t)
{
	if (terrain != t) {
		terrain = t;
		markChunksDirty();
	}
}

/**
 * Returns the terrain model which is being rendered by this entity.
 */
Terrain* WorldTerrain::getTerrain()
{
	return terrain;
}

/**
 * Draws the terrain. Note that this function does not draw the entire terrain.
 * Various forms of view frustum culling and the like may be performed.
 */
void WorldTerrain::draw(const RenderInfo& info)
{
	// Update the chunks if required.
	updateChunksIfDirty();

	// For now simply draw all the chunks.
	for (Chunks::iterator it = chunks.begin(); it != chunks.end(); it++)
		(*it).draw(info);
}

/**
 * @brief Marks the chunks as dirty.
 * This will cause them to be updated as soon as possible.
 */
void WorldTerrain::markChunksDirty()
{
	if (!chunksDirty) {
		chunksDirty = true;
		// spawn the update job here...
	}
}

/**
 * Updates the chunks that are used to render segments of the terrain.
 */
void WorldTerrain::updateChunks()
{
	LOG(kLogInfo, "Updating all chunks");
	chunksDirty = false;
	if (!terrain) {
		LOG(kLogWarning, "No terrain model specified");
		chunks.clear();
		return;
	}

	// Calculate how many chunks are required to cover the terrain and make
	// sure enough chunks are around.
	const int chunkSize = 32;
	int cw = ceil((double)terrain->width  / chunkSize) + 1e-9;
	int ch = ceil((double)terrain->height / chunkSize) + 1e-9;
	LOG(kLogInfo, "%i x %i (= %i) chunks required", cw, ch, cw*ch);
	chunks.resize(cw*ch);

	// Configure the individual chunks.
	for (int y = 0; y < ch; y++) {
		for (int x = 0; x < cw; x++) {
			WorldTerrainChunk& chunk = chunks[y * cw + x];
			chunk.setTerrain(terrain);

			WorldTerrainChunk::Chunk c;
			c.x0 = x * chunkSize;
			c.y0 = y * chunkSize;
			c.x1 = std::min<int>((x+1) * chunkSize, terrain->width-1);
			c.y1 = std::min<int>((y+1) * chunkSize, terrain->height-1);
			chunk.setChunk(c);

			chunk.updateIfDirty();
		}
	}
}

/**
 * Updates the chunks if they were marked as dirty by calling markChunksDirty()
 * beforehand.
 */
void WorldTerrain::updateChunksIfDirty()
{
	if (chunksDirty)
		updateChunks();
}