/* Copyright © 2013 Fabian Schuiki */
#pragma once

/**
 * Container that holds information about a rendering pass. References to this
 * class are passed to the various draw() functions.
 */

class RenderInfo
{
public:
	/// Width of the render target.
	int width;
	/// Height of the render target.
	int height;

	/// Mask of the bounds to be drawn.
	int drawBounds;
	enum {
		kNoBounds = 0,
		kTerrainBounds = 1 << 0,
		kAllBounds = 0x1
	};

	/// Mask of the normals to be drawn.
	int drawNormals;
	enum {
		kNoNormals = 0,
		kTerrainNormals = 1 << 0,
		kAllNormals = 0x1
	};
};