/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include "Vector.h"

/**
 * A line described by a point and a direction vector.
 */
class Line
{
public:
	/// A point on the line.
	vec3 p;

	/// The direction of the line.
	vec3 d;

	Line() {}
	Line(vec3 p, vec3 d) : p(p), d(d) {}

	/// Shortest vector from the line to the point a.
	vec3 displacement(vec3 a) const
	{
		vec3 n = d.unit();
		vec3 pma = p-a;
		return pma - n * (pma * n);
	}

	/// Distance of the point a from the line.
	double distance(vec3 a) const { return displacement(a).length(); }
	/// Square distance of the point a from the line.
	double distance2(vec3 a) const { return displacement(a).length2(); }
};