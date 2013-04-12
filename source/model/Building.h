/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include <gc_cpp.h>
#include <set>

class TerrainCell;

namespace model
{
	class Building : public gc_cleanup
	{
	public:
		vec3 position;
		typedef std::set<TerrainCell*> Cells;
		Cells cells;
	};
}