/* Copyright © 2013 Fabian Schuiki */
#include "WorldModel.h"
#include "Model.h"
#include "Logger.h"


WorldModel::WorldModel()
{
	vertexBuffer = NULL;
	model = NULL;
	bufferDirty = false;
}

WorldModel::~WorldModel()
{
	dispose();
}

void WorldModel::dispose()
{
	if (vertexBuffer) {
		delete vertexBuffer;
		vertexBuffer = NULL;
	}
	disposeOfMeshes();
}

/**
 * @brief Draws the model.
 * Note that in order to change the location of the model you need to properly
 * apply a transformation matrix with glTranslate and glRotate.
 */
void WorldModel::draw(const RenderInfo& info)
{
	if (!vertexBuffer) return;

	// Bind the vertices.
	vertexBuffer->bind();

	// Configure the vertex buffer layout.
	glVertexPointer  (3, GL_FLOAT, vertexStride, &((Model::Vertex*)NULL)->x);
	glNormalPointer  (   GL_FLOAT, vertexStride, &((Model::Vertex*)NULL)->nx);
	glColorPointer   (3, GL_FLOAT, vertexStride, &((Model::Vertex*)NULL)->r);
	glTexCoordPointer(2, GL_FLOAT, vertexStride, &((Model::Vertex*)NULL)->u);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	// Draw the individual mesh groups.
	for (Meshes::iterator im = meshes.begin(); im != meshes.end(); im++) {
		Mesh& m = *im;
		if (!m.indexBuffer) continue;

		// Bind the index buffer.
		m.indexBuffer->bind();

		// Configure the material.
		// m.material->bind();
		//glEnable(GL_TEXTURE_2D);
		glColor3f(1, 1, 1);
		glEnable(GL_LIGHTING);
		//glBindTexture(GL_TEXTURE_2D, texture.id);

		// Draw the mesh.
		glDrawElements(GL_TRIANGLES, m.num_indices, GL_UNSIGNED_INT, 0);

		// Tear down the material.
		// m.material->unbind();
		glDisable(GL_LIGHTING);
		//glDisable(GL_TEXTURE_2D);
	}

	// Tear down the configuration.
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	// Tear down the vertex buffer.
	vertexBuffer->unbind();
}

/**
 * @brief Marks the OpenGL buffers as dirty.
 * This will cause the model to be recompiled on the next occasion, i.e. the
 * next call to updateBufferIfDirty() or updateBuffer().
 */
void WorldModel::markBufferDirty()
{
	if (!bufferDirty) {
		bufferDirty = true;
		// spawn jobs that actually update the buffer...
	}
}

/**
 * @brief Updates the OpenGL buffers if they're outdated.
 */
void WorldModel::updateBufferIfDirty()
{
	if (bufferDirty) {
		updateBuffer();
	}
}

/**
 * @brief Updates the OpenGL buffers.
 * You should rather use updateBufferIfDirty() or markBufferDirty() in order
 * to trigger a buffer update or mark the buffers as outdated.
 */
void WorldModel::updateBuffer()
{
	bufferDirty = false;

	// Get rid of the current mesh index buffers.
	disposeOfMeshes();

	// Don't do anything if we don't have any model.
	if (!model)
		return;
	LOG(kLogDebug, "Updating buffer for model %p", model);

	// Create a new vertex buffer if none exists.
	if (!vertexBuffer) {
		vertexBuffer = new gl::Buffer(GL_ARRAY_BUFFER);
	}

	// Iterate through the model's meshes.
	bool firstVertex = true;
	std::vector <Model::Vertex> vertices;
	vertexStride = sizeof(Model::Vertex);
	meshes.resize(model->getMeshCount());
	
	for (int im = 0; im < model->getMeshCount(); im++) {
		Model::Mesh* modelMesh = model->getMesh(im);
		Mesh& mesh = meshes[im];
		mesh.indexBuffer = new gl::Buffer(GL_ELEMENT_ARRAY_BUFFER);
		std::vector<GLint> indices;

		// Create a new compiled Mesh object for this.
		for (int it = 0; it < modelMesh->getTriangleCount(); it++) {
			const Model::Triangle& triangle = modelMesh->getTriangle(it);
			for (int n = 0; n < 3; n++) {
				const Model::Vertex& v = model->getVertex(triangle.v[n]);
				indices.push_back(vertices.size());
				vertices.push_back(v);
				if (v.x < vertexBounds.x0 || firstVertex) vertexBounds.x0 = v.x;
				if (v.x > vertexBounds.x1 || firstVertex) vertexBounds.x1 = v.x;
				if (v.y < vertexBounds.y0 || firstVertex) vertexBounds.y0 = v.y;
				if (v.y > vertexBounds.y1 || firstVertex) vertexBounds.y1 = v.y;
				if (v.z < vertexBounds.z0 || firstVertex) vertexBounds.z0 = v.z;
				if (v.z > vertexBounds.z1 || firstVertex) vertexBounds.z1 = v.z;
			}
		}

		// Load the indices into the index buffer of this mesh.
		mesh.num_indices = indices.size();
		mesh.indexBuffer->loadData(indices.size() * sizeof(GLint), &indices[0], GL_STATIC_DRAW);
	}

	// Load the accumulated meshes into the vertex buffer.
	vertexBuffer->loadData(vertices.size() * sizeof(Model::Vertex), &vertices[0], GL_STATIC_DRAW);
}

/**
 * @brief Changes the model this WorldModel renders.
 * Causes the compiled OpenGL structures to be regenerated upon the next
 * occasion.
 */
void WorldModel::setModel(Model* m)
{
	if (model != m) {
		model = m;
		markBufferDirty();
	}
}

/**
 * @brief Returns the model this WorldModel renders.
 */
Model* WorldModel::getModel()
{
	return model;
}

/**
 * @brief Gets rid of the buffered meshes.
 * Makes sure the OpenGL resources are properly released.
 */
void WorldModel::disposeOfMeshes()
{
	for (Meshes::iterator im = meshes.begin(); im != meshes.end(); im++) {
		if ((*im).indexBuffer)
			delete (*im).indexBuffer;
	}
	meshes.clear();
}