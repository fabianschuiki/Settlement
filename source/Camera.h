/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include "Frustum.h"
#include "Vector.h"
#include "Line.h"
#include <gc_cpp.h>

/**
 * Manages the information required to transform a 3D scene as if viewed
 * through a camera.
 */

class Camera : public gc
{
public:
	int viewportWidth;
	int viewportHeight;
	
	float fov, aspect;
	float near, far;
	vec3 pos, up, at;
	float K; //perspective scaling factor
	Frustum frustum;

	// Four corners of the near and far plane.
	vec3 ntl, ntr, nbl, nbr;
	vec3 ftl, ftr, fbl, fbr;
	
	Camera();
	
	void apply();
	void applyViewport();
	void updateFrustum();
	
	void setViewport(int w, int h);

	Line unproject(int x, int y) const;
	Line unproject(double fx, double fy) const;
	
private:
};