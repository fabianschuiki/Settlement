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
};