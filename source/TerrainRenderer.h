/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include "Vector.h"

//class Terrain;
class RenderInfo;

class TerrainRenderer
{
public:
	TerrainRenderer();

	void draw(const RenderInfo &info);

protected:
	struct Node {
		vec3 p;
		vec3 n;
	};

	struct Node nodes[128][128];
};