/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include "Vector.h"

/**
 * An infinte 2D plane in 3D space.
 */

class Plane
{
public:
	vec3 n;
	double d;
	
	Plane();
	Plane(vec3 a, vec3 b, vec3 c)
	{
		set(a,b,c);
	}
	
	void set(vec3 a, vec3 b, vec3 c);
	double getDistance(vec3 p);
};