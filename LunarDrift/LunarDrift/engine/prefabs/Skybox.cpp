#include "Skybox.h"
#include "../containers/Model.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


Skybox::Skybox(
	Context* context, 
	std::weak_ptr<MetaManager> managers,
	const std::string& texDir)
	: SceneObject(context, managers),
	  m_VAO(nullptr),
	  m_CubeMapTex(std::make_unique<CubeMapTexture>(context, texDir))
{
	struct VERTEX 
	{
		GLfloat x, y, z;
	} rawVertices[] = 
	{ 
		{ -2.2f,  2.2f,  2.2f },     // Top Left Front
		{ -2.2f, -2.2f,  2.2f },     // Bottom Left Front
		{  2.2f, -2.2f,  2.2f },     // Bottom Right Front
		{  2.2f,  2.2f,  2.2f },     // Top Right Front
		{ -2.2f,  2.2f, -2.2f },     // Top Left Back
		{ -2.2f, -2.2f, -2.2f },     // Bottom Left Back
		{  2.2f, -2.2f, -2.2f },     // Bottom Right Back
		{  2.2f,  2.2f, -2.2f }      // Top Right Back
	};
	std::vector<VERTEX> vertices;
	vertices.assign(rawVertices, rawVertices + 8);


	unsigned short rawIndices[] = {
		0, 3, 2, 2, 1, 0,     // Front
		3, 7, 6, 6, 2, 3,     // Right
		7, 4, 5, 5, 6, 7,     // Back
		4, 0, 1, 1, 5, 4,     // Left
		0, 4, 7, 7, 3, 0,     // Top
		1, 2, 6, 6, 5, 1,     // Bottom
	};
	std::vector<unsigned short> indices;
	indices.assign(
		rawIndices, 
		rawIndices + (sizeof(rawIndices) / sizeof(unsigned short)));

	std::vector<VertexAttribute> attribs;
	getLayoutAttribs(&attribs);

	std::shared_ptr<VertexBufferObjectBase> vbo =
		std::make_shared<VertexBufferObject<VERTEX>>(context, vertices);
	std::shared_ptr<IndexBufferObject> ibo =
		std::make_shared<IndexBufferObject>(context, indices);
	m_VAO = std::make_unique<VertexArrayObject>(context, vbo, attribs, ibo);
}

Skybox::~Skybox()
{
    //dtor
}


void Skybox::getLayoutAttribs(std::vector<VertexAttribute>* attribs) const
{
	attribs->push_back(VertexAttribute(
		"Position",
		0,
		sizeof(glm::vec3),
		VertexAttribute::Type::Float,
		3));
}

void Skybox::draw(Context* context, std::shared_ptr<ShaderManager> shaders)
{
    glDisable(GL_DEPTH_TEST);

	std::shared_ptr<ShaderProgram> shader = shaders->get("skybox");
	updateModelMatrixState(context, shader);

	m_CubeMapTex->bind(context);

	m_VAO->bind(context, shader);
	glDrawElements(
		GL_TRIANGLES, 
		m_VAO->getIndexBuffer()->size(),
		m_VAO->getIndexBuffer()->getOpenGLClass(), 
		nullptr);

    glEnable(GL_DEPTH_TEST);
}
