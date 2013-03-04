/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include "Camera.h"

/**
 * A special camera that orbits a point of interest at a given distance,
 * inclination and rotation (azimuth).
 */

class OrbitingCamera : public Camera
{
public:
	OrbitingCamera();

	vec3 poi;
	double azimuth;
	double inclination;
	double distance;

	void setPOI(vec3 p, bool immediate = false);
	void setAzimuth(double a, bool immediate = false);
	void setInclination(double i, bool immediate = false);
	void setDistance(double d, bool immediate = false);

	void advance(double dt);

private:
	vec3 poi_anim;
	double azimuth_anim;
	double inclination_anim;
	double distance_anim;
};