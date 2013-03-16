/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include "LoadableAsset.h"
#include "Model.h"
#include <vector>

class ObjMeshAssetManager;
class StringAsset;

/**
 * @brief .obj mesh asset.
 */
class ObjMeshAsset : public LoadableAsset, public Model, public gc_cleanup
{
public:
	ObjMeshAssetManager* const manager;

	/// Creates a new empty .obj mesh asset with the given name.
	ObjMeshAsset(const std::string& name, AssetManager<ObjMeshAsset>* manager);
	virtual ~ObjMeshAsset();
	AssetStaticGetDecl(ObjMeshAsset);

	// Model Interface.
	virtual int getVertexCount();
	virtual Vertex& getVertex(int i);
	virtual int getMeshCount();
	virtual Mesh& getMesh(int i);

protected:
	virtual void locklessLoad();
	virtual void locklessDispose();

	StringAsset* objData;

	/// An object contained in the .obj file.
	struct Object : public Mesh {
	public:
		std::string name;
		std::string material;
		std::vector<Triangle> triangles;
		virtual int getTriangleCount();
		virtual Triangle& getTriangle(int i);
	};
	typedef std::vector<Object> Objects;
	typedef std::vector<Vertex> Vertices;
	Objects objects;
	Vertices vertices;
};

/**
 * @brief Manager for ObjMeshAsset objects.
 */
class ObjMeshAssetManager : public AssetManager<ObjMeshAsset>
{
};