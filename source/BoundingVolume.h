/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include "AABox.h"
#include "RenderInfo.h"

/**
 * @brief Container class for entity bounding volumes.
 *
 * The exact implementation and choice of the volume is handled internally,
 * this class merely acts as a high level interface for coarse collision
 * detection and view frustum culling.
 */
class BoundingVolume
{
public:
	BoundingVolume();
	void clear();
	void add(const BoundingVolume& bv);
	void add(const AABox& box);
	void draw(const RenderInfo& info);
private:
	bool isClear;
	AABox box;
};