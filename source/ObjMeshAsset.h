/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include "LoadableAsset.h"
#include "MeshInterface.h"
#include <vector>

class ObjMeshAssetManager;
class StringAsset;

/**
 * @brief .obj mesh asset.
 */
class ObjMeshAsset : public LoadableAsset, public MeshInterface, public gc_cleanup
{
public:
	ObjMeshAssetManager* const manager;

	/// Creates a new empty .obj mesh asset with the given name.
	ObjMeshAsset(const std::string& name, AssetManager<ObjMeshAsset>* manager);
	virtual ~ObjMeshAsset();

protected:
	virtual void locklessLoad();
	virtual void locklessDispose();

	StringAsset* objData;

	/// An object contained in the .obj file.
	struct Object {
	public:
		std::string name;
		std::string material;
		std::vector<int> indices;
	};
	typedef std::vector<Object> Objects;
	Objects objects;

private:
	struct obj_v  { double x,y,z; };
	struct obj_vn { double x,y,z; };
	struct obj_vt { double u,v; };
	struct Vertex {
		obj_v v;
		obj_vn vn;
		obj_vt vt;
	};
};

/**
 * @brief Manager for ObjMeshAsset objects.
 */
class ObjMeshAssetManager : public AssetManager<ObjMeshAsset>
{
};