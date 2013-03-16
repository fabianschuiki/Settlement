/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include "LoadableAsset.h"
#include "AssetManager.h"
#include <gc_cpp.h>
#include <boost/thread/shared_mutex.hpp>

class StringAssetManager;

/**
 * @brief A string loaded from disk.
 * This asset contains an std::string loaded from the disk. This is useful for
 * general text loading.
 */
class StringAsset : public LoadableAsset, public gc_cleanup
{
public:
	StringAssetManager* const manager;

	/// Creates a new empty string asset with the given name.
	StringAsset(const std::string& name, AssetManager<StringAsset>* manager);
	~StringAsset();
	AssetStaticGetDecl(StringAsset);

	/// Returns the wrapped std::string.
	const std::string& getString();

protected:
	/// The string being wrapped by this asset.
	std::string str;

	virtual void locklessLoad();
	virtual void locklessDispose();
};

/**
 * @brief Manager for StringAsset objects.
 */
class StringAssetManager : public AssetManager<StringAsset>
{
protected:
	//AssetManagerCurrentDecl();
};