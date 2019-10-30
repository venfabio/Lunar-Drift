#include "Cursor.h"


Cursor::Cursor(
	Context* context,
	std::weak_ptr<MetaManager> managers,
	const glm::vec3& position)
	: SceneObject(context, managers)
{
	setPosition(position);

	m_Model = std::make_shared<Model>(context, "cursor");
	m_Texture = std::make_unique<Texture>(context, "ship.dds");
}

Cursor::~Cursor()
{
}

void Cursor::step(double dt)
{
	SceneObject::step(dt);
}

void Cursor::predraw(Context* context)
{
	SceneObject::predraw(context);
}

void Cursor::draw(Context* context, std::shared_ptr<ShaderManager> shaders)
{
	std::shared_ptr<ShaderProgram> shader = shaders->get("texbox");
	updateModelMatrixState(context, shader);

	m_Texture->bind(context);
	m_Model->draw(context, shader);
}