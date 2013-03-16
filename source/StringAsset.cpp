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
:	LoadableAsset(name), manager((StringAssetManager*)manager)
{
}

StringAsset::~StringAsset()
{
	dispose();
}

AssetStaticGetDef(StringAsset);

void StringAsset::locklessLoad()
{
	LOG(kLogDebug, "Loading string asset %s", name.c_str());
	string path = string("assets/") + name;
	ifstream fin(path.c_str(), std::ios::in | std::ios::binary);
	if (!fin.good()) {
		LOG(kLogError, "Unable to load string asset %s.", name.c_str());
		return;
	}
	str.assign((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
}

void StringAsset::locklessDispose()
{
	str.clear();
}

const std::string& StringAsset::getString()
{
	shared_lock<shared_mutex> lock(mutex);
	return str;
}