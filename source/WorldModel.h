/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include "WorldEntity.h"
#include "gl/Buffer.h"
#include <gc_cpp.h>
#include <gc_allocator.h>
#include <vector>

class Model;

class WorldModel : public gc_cleanup
{
public:
	WorldModel();
	~WorldModel();
	void dispose();

	void draw(const RenderInfo& info);

	void markBufferDirty();
	void updateBufferIfDirty();
	void updateBuffer();

	void setModel(Model* m);
	Model* getModel();

protected:
	struct Mesh {
		// Material* material;
		gl::Buffer* indexBuffer;
		int num_indices;
	};
	typedef std::vector<Mesh, gc_allocator<Mesh> > Meshes;
	Meshes meshes;
	void disposeOfMeshes();

	gl::Buffer* vertexBuffer;
	int vertexStride;

	/// Whether the compiled vertex buffer and meshes are outdated.
	bool bufferDirty;

	/// The model used for rendering.
	Model* model;

	/// Simple axis-aligned box that encloses all vertices of the model.
	AABox vertexBounds;
};