/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include "Asset.h"
#include "AssetManager.h"
#include <gc_cpp.h>
#include <boost/thread/shared_mutex.hpp>

/**
 * @brief An asset loaded from disk.
 * This is an abstract asset that supoorts a loading mechanism. Subclasses will
 * want to override locklessLoad() to do their loading.
 */
class LoadableAsset : public Asset
{
public:
	LoadableAsset(const std::string& name);

	virtual void require();
	virtual void load();
	virtual void dispose();

protected:
	/// Mutex that protects access to the loaded data.
	boost::shared_mutex mutex;

	/// Whether the string is loaded or not.
	bool loaded;

	/// Function that loads the data without synchronization.
	virtual void locklessLoad() = 0;

	/// Function that disposes of the data without synchronization.
	virtual void locklessDispose() = 0;
};