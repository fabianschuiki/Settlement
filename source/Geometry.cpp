/* Copyright © 2013 Fabian Schuiki */
#include "Geometry.h"
#include "Logger.h"

namespace geo
{
	/**
	 * @brief Intersects an axis-aligned box and a line.
	 * Potential intersection points are stored in pa and pb.
	 * @return Returns true if the two intersect, false if they don't.
	 */
	bool intersect(const AABox& b, const Line& l, vec3& pa, vec3& pb)
	{
		// Find the points on the six face planes of the AABox.
		#define LineAABoxPlaneIntersection(boxDim, lineDim) l.p + l.d * ((b.boxDim - l.p.lineDim) / l.d.lineDim)
		vec3 px0 = LineAABoxPlaneIntersection(x0, x);
		vec3 px1 = LineAABoxPlaneIntersection(x1, x);
		vec3 py0 = LineAABoxPlaneIntersection(y0, y);
		vec3 py1 = LineAABoxPlaneIntersection(y1, y);
		vec3 pz0 = LineAABoxPlaneIntersection(z0, z);
		vec3 pz1 = LineAABoxPlaneIntersection(z1, z);

		// Check which of the six points actually are on the plane.
		#define IsPointOnAABoxPlane(p, dima, dimb) (p.dima >= b.dima##0 && p.dima <= b.dima##1 && p.dimb >= b.dimb##0 && p.dimb <= b.dimb##1)
		bool ix0 = IsPointOnAABoxPlane(px0, y, z);
		bool ix1 = IsPointOnAABoxPlane(px1, y, z);
		bool iy0 = IsPointOnAABoxPlane(py0, x, z);
		bool iy1 = IsPointOnAABoxPlane(py1, x, z);
		bool iz0 = IsPointOnAABoxPlane(pz0, x, y);
		bool iz1 = IsPointOnAABoxPlane(pz1, x, y);

		// Select the two points that intersect and return true, or false if
		// no two points can be found.
		vec3* p = &pa;
		if (ix0) { *p = px0; if (p == &pa) p = &pb; else return true; }
		if (ix1) { *p = px1; if (p == &pa) p = &pb; else return true; }
		if (iy0) { *p = py0; if (p == &pa) p = &pb; else return true; }
		if (iy1) { *p = py1; if (p == &pa) p = &pb; else return true; }
		if (iz0) { *p = pz0; if (p == &pa) p = &pb; else return true; }
		if (iz1) { *p = pz1; if (p == &pa) p = &pb; else return true; }
		return false;
	}

	/**
	 * @brief Checks whether an axis-aligned box contains a point.
	 */
	bool intersects(const AABox& b, const vec3& p)
	{
		return (p.x >= b.x0 && p.x <= b.x1 && p.y >= b.y0 && p.y <= b.y1 && p.z >= b.z0 && p.z <= b.z1);
	}
}