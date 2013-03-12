/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include "AABox.h"
#include "Line.h"
#include "Vector.h"

namespace geo
{
	bool intersect(const AABox& b, const Line& l, vec3& pa, vec3& pb);
	inline bool intersects(const AABox& b, const Line& l)
	{
		vec3 _a, _b;
		return intersect(b, l, _a, _b);
	}

	bool intersects(const AABox& b, const vec3& p);
}