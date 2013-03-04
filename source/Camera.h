/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include "Frustum.h"
#include "Vector.h"
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
	
	Camera();
	
	void apply();
	void applyViewport();
	void updateFrustum();
	
	void setViewport(int w, int h);
	
private:
};