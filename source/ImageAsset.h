/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include "Asset.h"
#include "AssetManager.h"
#include <gc_cpp.h>
#include <boost/thread/shared_mutex.hpp>
#include <SFML/Graphics/Image.hpp>

class ImageAssetManager;

/**
 * @brief An image loaded from disk.
 * This asset contains an sf::Image loaded from the disk. This is useful for
 * general image loading and is used in TextureAsset to load the texture data.
 */
class ImageAsset : public Asset, public gc_cleanup
{
public:
	ImageAssetManager* const manager;

	/// Creates a new empty image asset with the given name.
	ImageAsset(const std::string& name, AssetManager<ImageAsset>* manager);
	~ImageAsset();

	virtual void require();
	virtual void load();
	virtual void dispose();

	/// Returns the wrapped sf::Image.
	const sf::Image& getImage();

protected:
	/// Mutex that protects access to the loaded data.
	boost::shared_mutex mutex;

	/// The image being wrapped by this asset.
	sf::Image image;

	/// Whether the image is loaded or not.
	bool loaded;

	void _locklessLoad();
};

/**
 * @brief Manager for ImageAsset objects.
 */
class ImageAssetManager : public AssetManager<ImageAsset>
{
};