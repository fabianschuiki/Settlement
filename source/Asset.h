/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include <string>
#include <gc_cpp.h>

/**
 * @brief Superclass of all assets used throughout the game.
 * Exposes loading and referencing mechanisms that allow uniform handling of
 * game assets, as well as asynchronous loading of them.
 */
class Asset
{
public:
	/// The name of this asset.
	const std::string name;

	/// Creates a new asset with the given name.
	Asset(const std::string& name) : name(name) {}

	/**
	 * @brief Forces the asset to be loaded.
	 * Does nothing if the asset is already loaded. If it is not loaded, blocks
	 * until it is. After calling this function, the asset is guaranteed to be
	 * ready for use.
	 */
	virtual void require() = 0;

	/**
	 * @brief Loads the asset if required.
	 * This function may be called in a threaded environment, so the
	 * implementation is required to add proper synchronization mechanisms to
	 * prevent loading and using of the asset at the same time.
	 */
	virtual void load() = 0;

	/**
	 * @brief Disposes of all loaded data.
	 *
	 * The asset should dispose of all resources is has loaded, i.e. blocks of
	 * memory, OpenGL textures and other objects, etc.. This is required since
	 * in a garbage collected environment the actual deconstruction of obejcts
	 * cannot be controlled properly.
	 *
	 * Dispose may be called multiple times, so make sure the resources are
	 * only destroyed if they actually exist. Call dispose() in the destructor
	 * of classes deriving from Asset.
	 */
	virtual void dispose() = 0;
};

#define AssetStaticGetDecl(AssetType) static AssetType* get(const std::string& name)
#define AssetStaticGetDef(AssetType) AssetType* AssetType::get(const std::string& name) { return AssetType##Manager::current()->get(name); }