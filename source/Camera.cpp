/*
 * Copyright © 2012 Fabian Schuiki
 */

#include <SFML/OpenGL.hpp>
#include "Camera.h"


Camera::Camera()
{
	fov = 45;
	aspect = 1;
	near = 1;
	far = 100;
	
	up.y = 1;
	at.z = 1;
	
	viewportWidth = 0;
	viewportHeight = 0;
}

/** Resets the projection matrix to the appropriate perspective and the modelview matrix according
 * to the location and direction of the camera. */
void Camera::apply()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, aspect, near, far);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(pos.x, pos.y, pos.z,  at.x,  at.y,  at.z,  up.x,  up.y,  up.z);
}

void Camera::applyViewport()
{
	glViewport(0, 0, viewportWidth, viewportHeight);
}

void Camera::updateFrustum()
{
	//Calculate the near and far plane dimensions.
	double tanfov = tan(fov * 0.5 / 180 * M_PI);
	
	double nh = near * tanfov;
	double nw = nh * aspect;
	
	double fh = far * tanfov;
	double fw = fh * aspect;
	
	//Calculate the camera axis.
	vec3 z = pos - at;
	z.normalize();
	vec3 x = up.cross(z);
	x.normalize();
	vec3 y = z.cross(x);
	
	//Compute the centers of the near and far planes.
	vec3 nc = pos - z*near;
	vec3 fc = pos - z*far;
	
	//Compute the 4 corners of the near plane.
	vec3 ntl = nc + y*nh - x*nw;
	vec3 ntr = nc + y*nh + x*nw;
	vec3 nbl = nc - y*nh - x*nw;
	vec3 nbr = nc - y*nh + x*nw;
	
	//Compute the 4 corners of the far plane.
	vec3 ftl = fc + y*fh - x*fw;
	vec3 ftr = fc + y*fh + x*fw;
	vec3 fbl = fc - y*fh - x*fw;
	vec3 fbr = fc - y*fh + x*fw;
	
	//Compute the frustum.
	frustum.t.set(ntr, ntl, ftl);
	frustum.b.set(nbl, nbr, fbr);
	frustum.l.set(ntl, nbl, fbl);
	frustum.r.set(nbr, ntr, fbr);
	frustum.n.set(ntl, ntr, nbr);
	frustum.f.set(ftr, ftl, fbl);
}

void Camera::setViewport(int w, int h)
{
	viewportWidth = w;
	viewportHeight = h;
	aspect = (float)w / h;
	
	K = viewportWidth / (2 * tan(fov / 2));
}