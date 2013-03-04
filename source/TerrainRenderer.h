/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include "Vector.h"
#include <vector>

class Terrain;
class RenderInfo;

class TerrainRenderer
{
public:
	TerrainRenderer();
	void draw(const RenderInfo &info);
	void update();

	Terrain *terrain;
	void setTerrain(Terrain *t);

protected:
	struct Node {
		vec3 c;
		vec3 p;
		vec3 n;
	};

	unsigned int w, h;
	std::vector<Node> nodes;
};