/* Copyright © 2013 Fabian Schuiki */
#include "WorldGroup.h"

WorldGroup::WorldGroup() : WorldEntity()
{
}

void WorldGroup::addChild(WorldEntity* c)
{
	if (!children.count(c)) {
		children.insert(c);
		markBoundsDirty();
	}
}

void WorldGroup::removeChild(WorldEntity *c)
{
	if (children.count(c)) {
		children.erase(c);
		markBoundsDirty();
	}
}

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