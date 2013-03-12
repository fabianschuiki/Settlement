/* Copyright © 2013 Fabian Schuiki */
#include "BoundingVolume.h"
#include "gl/Primitives.h"
#include <algorithm>
#include <SFML/OpenGL.hpp>


BoundingVolume::BoundingVolume()
{
	isClear = true;
}

/**
 * Resets this bounding volume.
 */
void BoundingVolume::clear()
{
	isClear = true;
}

/**
 * @brief Merges this bounding volume with another.
 *
 * This is accomplished by expanding this bounding volume such that it covers
 * the other one as well. Note that the resulting volume is merely an
 * approximation of the exact union of the original ones.
 *
 * Simply expect the result to be a box that is big enough to encapsulate both
 * voumes.
 */
void BoundingVolume::add(const BoundingVolume& bv)
{
	if (bv.isClear) return;
	add(bv.box);
}

/**
 * @brief Adds the given axis-aligned box to this bounding volume.
 */
void BoundingVolume::add(const AABox& b)
{
	if (isClear) {
		box = b;
		isClear = false;
	} else {
		box.x0 = std::min(box.x0, b.x0);
		box.x1 = std::max(box.x1, b.x1);
		box.y0 = std::min(box.y0, b.y0);
		box.y1 = std::max(box.y1, b.y1);
		box.z0 = std::min(box.z0, b.z0);
		box.z1 = std::max(box.z1, b.z1);
	}
}

/**
 * @brief Draws the bounding volume.
 *
 * This is useful for visualizing the bounding volumes during debugging. This
 * function leaves the entire OpenGL state as it is, it simply commits the
 * appropriate geometry. This allows different coloring of the volumes.
 */
void BoundingVolume::draw(const RenderInfo& info)
{
	if (isClear) return; // nothing to draw if the bounding volume is clear

	/*// Simply draw the OpenGL cube.
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
	glEnd();*/
	gl::drawOutline(box);
}