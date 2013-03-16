/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include "Asset.h"
#include "AssetManager.h"
#include <gc_cpp.h>
#include <boost/thread/shared_mutex.hpp>

class StringAssetManager;

/**
 * @brief A string loaded from disk.
 * This asset contains an std::string loaded from the disk. This is useful for
 * general text loading.
 */
class StringAsset : public Asset, public gc_cleanup
{
public:
	StringAssetManager* const manager;

	/// Creates a new empty image asset with the given name.
	StringAsset(const std::string& name, AssetManager<StringAsset>* manager);
	~StringAsset();

	virtual void require();
	virtual void load();
	virtual void dispose();

	/// Returns the wrapped std::string.
	const std::string& getString();

protected:
	/// Mutex that protects access to the loaded data.
	boost::shared_mutex mutex;

	/// The string being wrapped by this asset.
	std::string str;

	/// Whether the string is loaded or not.
	bool loaded;

	void _locklessLoad();
};

/**
 * @brief Manager for StringAsset objects.
 */
class StringAssetManager : public AssetManager<StringAsset>
{
};