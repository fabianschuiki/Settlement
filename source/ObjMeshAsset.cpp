/* Copyright © 2013 Fabian Schuiki */
#include "ObjMeshAsset.h"
#include "StringAsset.h"
#include "Logger.h"
#include "Vector.h"
#include <sstream>

using std::string;
using std::stringstream;


// Object as loaded from the .obj file.
struct Object {
	std::string name;
	std::string material;
	typedef std::vector<double[2], gc_allocator<double[2]> > vector2f;
	typedef std::vector<double[3], gc_allocator<double[3]> > vector3f;
	typedef std::vector<int[3][3], gc_allocator<int[3][3]> > vector3x3i;
	vector3f v;
	vector2f vt;
	vector3f vn;
	vector3x3i f;
};


ObjMeshAsset::ObjMeshAsset(const std::string& name, AssetManager<ObjMeshAsset>* manager)
:	LoadableAsset(name), manager((ObjMeshAssetManager*)manager)
{
	objData = StringAsset::get(name);
}

ObjMeshAsset::~ObjMeshAsset()
{
	dispose();
}

AssetStaticGetDef(ObjMeshAsset);

void ObjMeshAsset::locklessLoad()
{
	objData->require();

	// Objects loaded.
	std::vector<vec3> vs;
	std::vector<vec3> vns;
	std::vector<vec3> vts;
	vertices.clear();
	objects.clear();

	// Iterate through the data line by line.
	const std::string& data = objData->getString();
	size_t pos = 0;
	while (pos != string::npos) {
		size_t nl = data.find('\n', pos);

		// Skip empty lines or comment lines.
		if (nl > pos && data[pos] != '#') {
			stringstream line(data.substr(pos, nl));

			// Read the operation.
			string op;
			line >> op;

			// Vertex data.
			if (op == "v") {
				vec3 v;
				line >> v.x;
				line >> v.y;
				line >> v.z;
				vs.push_back(v);
			}
			// Vertex texture data.
			else if (op == "vt") {
				vec3 vt;
				line >> vt.x;
				line >> vt.y;
				vts.push_back(vt);
			}
			// Vertex normal data.
			else if (op == "vn") {
				vec3 vn;
				line >> vn.x;
				line >> vn.y;
				line >> vn.z;
				vns.push_back(vn);
			}
			// Faces.
			else if (op == "f") {
				if (objects.empty()) {
					LOG(kLogInfo, "Creating implicit object for OBJ data stream without \"o <name>\" statement.");
					Object o;
					o.name = "<implicit>";
					objects.push_back(o);
				}
				Triangle t;
				int i;
				for (i = 0; line.tellg() < nl - pos; i++) {
					string s;
					line >> s;
					if (i >= 3) {
						LOG(kLogWarning, "Ignoring face with more than 3 vertices.");
						continue;
					}

					// Parse the line.
					int idx_v = -1, idx_vn = -1, idx_vt = -1;
					size_t slash1 = s.find('/');
					size_t slash2 = (slash1 != string::npos ? s.find('/', slash1 + 1) : string::npos);
					idx_v = atoi(s.substr(0, slash1).c_str());
					if (slash1 != string::npos && slash1 + 1 != slash2) {
						idx_vt = atoi(s.substr(slash1 + 1, slash2).c_str());
					}
					if (slash2 != string::npos) {
						idx_vn = atoi(s.substr(slash2 + 1).c_str());
					}

					// Create the vertex.
					Vertex v;
					v.r = 1;
					v.g = 1;
					v.b = 1;
					v.x = vs[idx_v - 1].x;
					v.y = vs[idx_v - 1].y;
					v.z = vs[idx_v - 1].z;
					if (idx_vt > 0) {
						v.u = vts[idx_vt - 1].x;
						v.v = vts[idx_vt - 1].y;
					} else {
						v.u = 0;
						v.v = 0;
					}
					if (idx_vn > 0) {
						v.nx = vns[idx_vn - 1].x;
						v.ny = vns[idx_vn - 1].y;
						v.nz = vns[idx_vn - 1].z;
					} else {
						v.nx = 0;
						v.ny = 0;
						v.nz = 0;
					}
					LOG(kLogDebug, "Vertex %f,%f,%f", v.x, v.y, v.z);
					t.v[i] = vertices.size();
					vertices.push_back(v);
				}
				if (i < 3) {
					LOG(kLogWarning, "Face with less than 3 vertices detected.");
				}
				objects.back().triangles.push_back(t);
			}
			// Objects.
			else if (op == "o") {
				Object o;
				line >> o.name;
				objects.push_back(o);
			}
			// Materials.
			else if (op == "usemtl") {
				if (objects.empty()) {
					LOG(kLogWarning, "Found \"usemtl\" without an object in place.");
				} else if (!objects.back().material.empty()) {
					LOG(kLogWarning, "Object %s already has material %s set.", objects.back().name.c_str(), objects.back().material.c_str());
				} else {
					line >> objects.back().material;
				}
			}
		}

		// Advance to the next line.
		pos = nl;
		if (pos < string::npos) pos++;
	}

	// Dump what we know.
	for (Objects::iterator it = objects.begin(); it != objects.end(); it++) {
		LOG(kLogInfo, "Loaded Object %s: %i triangles, %s", (*it).name.c_str(), (*it).triangles.size(), (*it).material.c_str());
	}
}

void ObjMeshAsset::locklessDispose()
{
	//objData->release();
}

int ObjMeshAsset::getVertexCount()
{
	return vertices.size();
}

Model::Vertex& ObjMeshAsset::getVertex(int i)
{
	return vertices[i];
}

int ObjMeshAsset::getMeshCount()
{
	return objects.size();
}

Model::Mesh& ObjMeshAsset::getMesh(int i)
{
	return objects[i];
}

int ObjMeshAsset::Object::getTriangleCount()
{
	return triangles.size();
}

Model::Triangle& ObjMeshAsset::Object::getTriangle(int i)
{
	return triangles[i];
}