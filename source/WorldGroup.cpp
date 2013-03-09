/* Copyright © 2013 Fabian Schuiki */
#include "WorldGroup.h"

WorldGroup::WorldGroup() : WorldEntity()
{
}

/**
 * @brief Adds the given child to the group.
 * Also marks the group's bounding volume as dirty if appropriate, which causes
 * the world manager to update it before rendering.
 */
void WorldGroup::addChild(WorldEntity* c)
{
	if (!children.count(c)) {
		children.insert(c);
		markBoundsDirty();
	}
}

/**
 * @brief Removes the given child from the group.
 * Also marks the group's bounding volume as dirty, which causes the world
 * manager to update it before rendering.
 */
void WorldGroup::removeChild(WorldEntity *c)
{
	if (children.count(c)) {
		children.erase(c);
		markBoundsDirty();
	}
}

/**
 * @brief Recalculates the bounding volume to encapsulate all children.
 * Should not be called directly. Use updateBoundsIfDirty() instead so no
 * superficial updates are performed.
 */
void WorldGroup::updateBounds()
{
	boundsDirty = false;
	bounds.clear();
	for (Children::iterator it = children.begin(); it != children.end(); it++) {
		WorldEntity* c = *it;
		c->updateBoundsIfDirty();
		bounds.add(c->getBounds());
	}
}