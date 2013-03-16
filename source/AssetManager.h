/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include <gc_allocator.h>
#include <gc_cpp.h>
#include <map>
#include <set>
#include <boost/thread/shared_mutex.hpp>

/**
 * @brief Abstract base interface to all asset managers.
 * This allows asset managers to be gathered in containers so certain
 * operations may be performed more easily.
 */
class AbstractAssetManager
{
public:
};

/**
 * @brief Manager for a certain group of assets.
 */
template <class AssetType>
class AssetManager : public AbstractAssetManager, public gc_cleanup
{
public:
	/// Returns the asset for the given name.
	AssetType* get(const std::string& name)
	{
		boost::upgrade_lock<boost::shared_mutex> lock(mutex);

		// Check whether the asset already exists. If it does, simply return
		// it.
		typename AssetMap::iterator i = assets.find(name);
		if (i != assets.end()) {
			return i->second;
		}

		// Otherwise we need to initialize the asset and return it.
		AssetType* asset = new AssetType (name, this);
		boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock);
		assets[name] = asset;
		return asset;
	}

protected:
	// Containers for the chosen AssetType.
	typedef std::map <std::string, AssetType*, std::less<std::string>, gc_allocator<std::pair<const std::string, AssetType*> > > AssetMap;
	typedef std::set <AssetType*, std::less<AssetType*>, gc_allocator<AssetType*> > AssetSet;

	/// Protective mutex for accesses to the assets.
	boost::shared_mutex mutex;

	/// Map of the assets managed by this object.
	AssetMap assets;

	/* TODO: In the future there will be lists of assets according to their
	 * current state (initialized, loaded, etc.). */
};