/* Copyright © 2013 Fabian Schuiki */
#include "WorldEntity.h"

WorldEntity::WorldEntity()
{
	parent = NULL;
	boundsDirty = false;
}

void WorldEntity::setParent(WorldEntity* p)
{
	if (parent != p) {
		parent = p;
	}
}

WorldEntity* WorldEntity::getParent()
{
	return parent;
}

const BoundingVolume& WorldEntity::getBounds()
{
	return bounds;
}

/**
 * @brief Marks the entity's bounding volume as outdated.
 */
void WorldEntity::markBoundsDirty()
{
	if (!boundsDirty) {
		boundsDirty = true;
		if (parent)
			parent->markBoundsDirty();
	}
}

void WorldEntity::updateBoundsIfDirty()
{
	if (boundsDirty)
		updateBounds();
}