/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include <gc_cpp.h>

/**
 * @brief Superclass of all assets used throughout the game.
 * Exposes loading and referencing mechanisms that allow uniform handling of
 * game assets, as well as asynchronous loading of them.
 */

class Asset
{
public:
	/**
	 * @brief Forces the asset to be loaded.
	 * Does nothing if the asset is already loaded. If it is not loaded, blocks
	 * until it is. After calling this function, the asset is guaranteed to be
	 * ready for use.
	 */
	virtual void require() = 0;
}