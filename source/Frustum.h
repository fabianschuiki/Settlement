/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include "AABox.h"
#include "Plane.h"
#include "Sphere.h"

/**
 * A frustum as generated as the viewing volume of a camera. Useful to perform
 * frustum culling on the world.
 */
class Frustum
{
public:
	typedef enum {
		kOutside   = 0,
		kInside    = 1,
		kIntersect = 2,
	} Coverage;
	
	/// The six planes limiting the furstum.
	Plane t, b, l, r, n, f;
	
	Coverage contains(vec3 p);
	Coverage contains(Sphere s);
	Coverage contains(AABox a);
};