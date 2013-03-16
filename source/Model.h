/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include <gc_cpp.h>
#include <SFML/OpenGL.hpp>

/**
 * @brief Interface for objects providing 3D models.
 */
class Model
{
public:
	/// Vertices returned by the model.
	struct Vertex {
		GLfloat x,y,z;
		GLfloat nx,ny,nz;
		GLfloat r,g,b;
		GLfloat u,v;
	};

	/// Triangles organized in the meshes.
	struct Triangle {
		/// Vertex indices.
		GLint v[3];
	};

	/**
	 * @brief Interface for objects providing vertex data.
	 */
	class Mesh {
	public:
		/// Returns the number of triangles.
		virtual int getTriangleCount() = 0;

		/// Returns the triangle for the specified index.
		virtual Triangle& getTriangle(int i) = 0;

		/// Material of this mesh.
		//virtual Material* getMaterial() = 0;
	};

	/// Returns the number of vertices in this model.
	virtual int getVertexCount() = 0;

	/// Returns the vertex with the given index.
	virtual Vertex& getVertex(int i) = 0;

	/// Returns the number of individual meshes in this model.
	virtual int getMeshCount() = 0;

	/// Returns the mesh with the given index.
	virtual Mesh& getMesh(int i) = 0;
};