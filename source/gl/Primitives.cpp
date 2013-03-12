/* Copyright © 2013 Fabian Schuiki */
#include "Primitives.h"
#include <SFML/OpenGL.hpp>

void gl::drawOutline(const AABox& box)
{
	#define V0 glVertex3f(box.x0, box.y0, box.z0)
	#define V1 glVertex3f(box.x1, box.y0, box.z0)
	#define V2 glVertex3f(box.x1, box.y0, box.z1)
	#define V3 glVertex3f(box.x0, box.y0, box.z1)
	#define V4 glVertex3f(box.x0, box.y1, box.z0)
	#define V5 glVertex3f(box.x1, box.y1, box.z0)
	#define V6 glVertex3f(box.x1, box.y1, box.z1)
	#define V7 glVertex3f(box.x0, box.y1, box.z1)

	glBegin(GL_LINE_STRIP);
	V0; V1; V2; V3; V0;
	V4; V5; V6; V7; V4;
	glEnd();
	glBegin(GL_LINES);
	V7; V3;
	V6; V2;
	V5; V1;
	glEnd();
}