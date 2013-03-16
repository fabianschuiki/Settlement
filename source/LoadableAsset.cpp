/* Copyright © 2013 Fabian Schuiki */
#include "LoadableAsset.h"

using boost::shared_lock;
using boost::unique_lock;
using boost::upgrade_lock;
using boost::upgrade_to_unique_lock;
using boost::shared_mutex;


LoadableAsset::LoadableAsset(const std::string& name)
:	Asset(name)
{
	loaded = false;
}

void LoadableAsset::require()
{
	upgrade_lock<shared_mutex> lock(mutex);
	if (!loaded) {
		upgrade_to_unique_lock<shared_mutex> uniqueLock(lock);
		locklessLoad();
	}
}

void LoadableAsset::load()
{
	unique_lock<shared_mutex> lock(mutex);
	loaded = true;
	locklessLoad();
}

void LoadableAsset::dispose()
{
	unique_lock<shared_mutex> lock(mutex);
	if (loaded) {
		loaded = false;
		locklessDispose();
	}
}