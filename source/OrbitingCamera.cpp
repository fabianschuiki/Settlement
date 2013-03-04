/* Copyright © 2013 Fabian Schuiki */
#include "OrbitingCamera.h"


OrbitingCamera::OrbitingCamera()
{
	azimuth = 0;
	inclination = M_PI / 6;
	distance = 2;

	// Copy the above defaults to the animation variables.
	poi_anim = poi;
	azimuth_anim = azimuth;
	inclination_anim = inclination;
	distance_anim = distance;
}

void OrbitingCamera::setPOI(vec3 p, bool immediate)
{
	poi = p;
	if (immediate)
		poi_anim = p;
}

void OrbitingCamera::setAzimuth(double a, bool immediate)
{
	azimuth = a;
	if (immediate)
		azimuth_anim = a;
}

void OrbitingCamera::setInclination(double i, bool immediate)
{
	inclination = i;
	if (immediate)
		inclination_anim = i;
}

void OrbitingCamera::setDistance(double d, bool immediate)
{
	distance = d;
	if (immediate)
		distance_anim = d;
}

void OrbitingCamera::advance(double dt)
{
	// The higher the snappiness the quicker the camera animates to its new
	// coordinates.
	const double snappiness = 1;

	// Calculat the distance between the current and target values that is
	// being covered during this animation step.
	double f = std::min(1.0, dt * snappiness);

	// Animate the values.
	poi_anim += (poi - poi_anim) * f;
	azimuth_anim += (azimuth - azimuth_anim) * f;
	inclination_anim += (inclination - inclination_anim) * f;
	distance_anim += (distance - distance_anim) * f;

	// Calculate the actual camera position and direction.
	vec3 poiToCamera(
		sin(azimuth_anim) * cos(inclination_anim),
		sin(inclination_anim),
		cos(azimuth_anim) * cos(inclination_anim)
	);
	double radius = distance * distance; // Gives the camera a constant zooming speed.
	this->at = poi;
	this->pos = poi + poiToCamera * radius;
}