/* Copyright © 2013 Fabian Schuiki */
#include "WorldEntity.h"
#include <cstdlib>

WorldEntity::WorldEntity()
{
	parent = NULL;
	boundsDirty = false;
}

/**
 * @brief Sets the entity's parent entity.
 * You should never have to invoke this function directly outside of grouping
 * entities, as they cope with keeping the hierarchy up-to-date.
 */
void WorldEntity::setParent(WorldEntity* p)
{
	if (parent != p) {
		parent = p;
	}
}

/**
 * Returns the entity's parent entity, or NULL if there is none.
 */
WorldEntity* WorldEntity::getParent()
{
	return parent;
}

/**
 * Returns the bounding volume of the entity.
 */
const BoundingVolume& WorldEntity::getBounds()
{
	return bounds;
}

/**
 * @brief Marks the entity's bounding volume as outdated.
 * This will recursively mark all its parents as to be updated.
 */
void WorldEntity::markBoundsDirty()
{
	if (!boundsDirty) {
		boundsDirty = true;
		if (parent)
			parent->markBoundsDirty();
	}
}

/**
 * @brief Updates the entity's bounding volume if it is outdated.
 */
void WorldEntity::updateBoundsIfDirty()
{
	if (boundsDirty)
		updateBounds();
}