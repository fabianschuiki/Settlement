/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include <gc_cpp.h>

/**
 * @brief Interface for objects providing 3D models.
 */
class Model
{
public:
	/// Vertices returned by the model.
	struct Vertex {
		double x,y,z;
		double nx,ny,nz;
		double r,g,b;
		double u,v;
	};

	/// Triangles organized in the meshes.
	struct Triangle {
		/// Vertex indices.
		int v[3];
	};

	/**
	 * @brief Interface for objects providing vertex data.
	 */
	class Mesh {
	public:
		/// Returns the number of triangles.
		virtual int getTriangleCount() = 0;

		/// Returns the triangle for the specified index.
		virtual const Triangle& getTriangle(int i) = 0;

		/// Material of this mesh.
		//virtual Material* getMaterial() = 0;
	};

	/// Returns the number of vertices in this model.
	virtual int getVertexCount() = 0;

	/// Returns the vertex with the given index.
	virtual const Vertex& getVertex(int i) = 0;

	/// Returns the number of individual meshes in this model.
	virtual int getMeshCount() = 0;

	/// Returns the mesh with the given index.
	virtual Mesh* getMesh(int i) = 0;
};