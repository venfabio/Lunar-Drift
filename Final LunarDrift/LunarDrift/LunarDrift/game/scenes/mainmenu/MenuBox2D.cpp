#include "MenuBox2D.h"


MenuBox2D::MenuBox2D(
	Context* context,
	std::weak_ptr<MetaManager> managers,
	const glm::vec2& position,
	const glm::vec2& dims)
	:SceneObject(context, managers)
{
	setPosition2D(position);

	struct VERTEX
	{
		glm::vec2 position;
		glm::vec2 texCoord;
	} rawVertices[] = {
		{ glm::vec2(0.f, 0.f), glm::vec2(0.f, 0.f) },
		{ glm::vec2(dims.x, 0.f), glm::vec2(1.f, 0.f) },
		{ glm::vec2(dims.x, dims.y), glm::vec2(1.f, 1.f) },
		{ glm::vec2(dims.x, dims.y), glm::vec2(1.f, 1.f) },
		{ glm::vec2(0.f, dims.y), glm::vec2(0.f, 1.f) },
		{ glm::vec2(0.f, 0.f), glm::vec2(0.f, 0.f) }
	};
	std::vector<VERTEX> vertices;
	vertices.assign(rawVertices, rawVertices + 6);

	std::vector<VertexAttribute> attribs;
	getLayoutAttribs(&attribs);

	std::shared_ptr<VertexBufferObjectBase> vbo =
		std::make_shared<VertexBufferObject<VERTEX>>(context, vertices);
	m_VAO = std::make_unique<VertexArrayObject>(context, vbo, attribs);

	m_Texture = std::make_unique<Texture>(context, "empty.dds");
}

MenuBox2D::~MenuBox2D()
{
}

void MenuBox2D::setTexture(Context* context, const std::string& file)
{
	m_Texture = std::make_unique<Texture>(context, file);
}

void MenuBox2D::step(double dt)
{
	SceneObject::step(dt);
}

void MenuBox2D::predraw(Context* context)
{
	SceneObject::predraw(context);
}

void MenuBox2D::draw(Context* context, std::shared_ptr<ShaderManager> shaders)
{
	std::shared_ptr<ShaderProgram> shader = shaders->get("texbox2D");
	updateModelMatrixState(context, shader);

	m_Texture->bind(context);
	m_VAO->bind(context, shader);
	glDrawArrays(GL_TRIANGLES, 0, m_VAO->getVertexBuffer()->size());
}

void MenuBox2D::getLayoutAttribs(std::vector<VertexAttribute>* attribs) const
{
	attribs->push_back(VertexAttribute(
		"Position",
		0,
		sizeof(glm::vec2) * 2,
		VertexAttribute::Type::Float,
		2));
	attribs->push_back(VertexAttribute(
		"TexCoord",
		sizeof(glm::vec2),
		sizeof(glm::vec2) * 2,
		VertexAttribute::Type::Float,
		2));
}

