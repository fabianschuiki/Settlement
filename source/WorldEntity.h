/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include "BoundingVolume.h"

/**
 * Base class for all entities in the 3D world. The entity maintains a bounding
 * volume of itself and its children, if there are any.
 */

class WorldEntity
{
public:
	WorldEntity();

	void setParent(WorldEntity* p);
	WorldEntity* getParent();

	const BoundingVolume& getBounds();
	void markBoundsDirty();
	void updateBoundsIfDirty();
	virtual void updateBounds() = 0;

protected:
	WorldEntity* parent;
	BoundingVolume bounds;
	bool boundsDirty;
};