/* Copyright © 2013 Fabian Schuiki */
#include "ImageAsset.h"
#include "Logger.h"

using boost::shared_lock;
using boost::unique_lock;
using boost::upgrade_lock;
using boost::upgrade_to_unique_lock;
using boost::shared_mutex;
using std::string;


ImageAsset::ImageAsset(const std::string& name, AssetManager<ImageAsset>* manager)
:	Asset(name), manager((ImageAssetManager*)manager)
{
	loaded = false;
}

ImageAsset::~ImageAsset()
{
	dispose();
}

void ImageAsset::require()
{
	upgrade_lock<shared_mutex> lock(mutex);
	if (!loaded) {
		upgrade_to_unique_lock<shared_mutex> uniqueLock(lock);
		_locklessLoad();
	}
}

void ImageAsset::load()
{
	unique_lock<shared_mutex> lock(mutex);
	_locklessLoad();
}

void ImageAsset::_locklessLoad()
{
	LOG(kLogDebug, "Loading image asset %s", name.c_str());
	loaded = true; // prevents us from multiple load attempts if loading fails
	if (!image.loadFromFile(string("assets/") + name)) {
		LOG(kLogError, "Unable to load image asset %s.", name.c_str());
	}
}

void ImageAsset::dispose()
{
	unique_lock<shared_mutex> lock(mutex);
	if (loaded) {
		loaded = false;
		// image.disposeSomehow();
	}
}

const sf::Image& ImageAsset::getImage()
{
	shared_lock<shared_mutex> lock(mutex);
	return image;
}