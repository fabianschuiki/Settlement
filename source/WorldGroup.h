/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include "WorldEntity.h"
#include <set>

class WorldEntity;

/**
 * A group of other entities in the world.
 */

class WorldGroup : public WorldEntity
{
public:
	WorldGroup();
	void addChild(WorldEntity* c);
	void removeChild(WorldEntity *c);
	void removeAllChildren();
	virtual void updateBounds();

protected:
	typedef std::set<WorldEntity*> Children;
	Children children;
};