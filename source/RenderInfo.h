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
};