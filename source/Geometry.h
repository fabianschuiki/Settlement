/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include "AABox.h"
#include "Line.h"
#include "Vector.h"
#include "Plane.h"

/**
 * A triangle in 3D space.
 */
class Triangle
{
public:
	vec3 a,b,c;

	Triangle() {}
	Triangle(vec3 a, vec3 b, vec3 c) : a(a), b(b), c(c) {}

	inline Plane getPlane() const { return Plane(a,b,c); }
};

namespace geo
{
	// AABox & Line
	bool intersect(const AABox& b, const Line& l, vec3& pa, vec3& pb);
	inline bool intersects(const AABox& b, const Line& l) { vec3 _a, _b; return intersect(b, l, _a, _b); }

	// AABox & Point
	bool intersects(const AABox& b, const vec3& p);

	// Plane & Line
	bool intersect(const Plane& p, const Line& l, vec3& q);
	inline bool intersects(const Plane& p, const Line& l) {	vec3 _a; return intersect(p, l, _a); }

	// Triangle & Line
	bool intersect(const Triangle& t, const Line& l, vec3& q);
	inline bool intersects(const Triangle& t, const Line& l) { vec3 _a; return intersect(t, l ,_a); }
}