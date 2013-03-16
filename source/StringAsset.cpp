/* Copyright © 2013 Fabian Schuiki */
#include "StringAsset.h"
#include "Logger.h"
#include <fstream>

using boost::shared_lock;
using boost::unique_lock;
using boost::upgrade_lock;
using boost::upgrade_to_unique_lock;
using boost::shared_mutex;
using std::string;
using std::ifstream;


StringAsset::StringAsset(const std::string& name, AssetManager<StringAsset>* manager)
:	Asset(name), manager((StringAssetManager*)manager)
{
	loaded = false;
}

StringAsset::~StringAsset()
{
	dispose();
}

void StringAsset::require()
{
	upgrade_lock<shared_mutex> lock(mutex);
	if (!loaded) {
		upgrade_to_unique_lock<shared_mutex> uniqueLock(lock);
		_locklessLoad();
	}
}

void StringAsset::load()
{
	unique_lock<shared_mutex> lock(mutex);
	_locklessLoad();
}

void StringAsset::_locklessLoad()
{
	LOG(kLogDebug, "Loading string asset %s", name.c_str());
	loaded = true; // prevents us from multiple load attempts if loading fails
	string path = string("assets/") + name;
	ifstream fin(path.c_str(), std::ios::in | std::ios::binary);
	if (!fin.good()) {
		LOG(kLogError, "Unable to load string asset %s.", name.c_str());
		return;
	}
	str.assign((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
}

void StringAsset::dispose()
{
	unique_lock<shared_mutex> lock(mutex);
	if (loaded) {
		loaded = false;
		str.clear();
	}
}

const std::string& StringAsset::getString()
{
	shared_lock<shared_mutex> lock(mutex);
	return str;
}