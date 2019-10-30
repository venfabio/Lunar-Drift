#include "MenuBoxAlt.h"


MenuBoxAlt::MenuBoxAlt(
	Context* context,
	std::weak_ptr<MetaManager> managers,
	const glm::vec3& position)
	:SceneObject(context, managers)
{
	setPosition(position);

	m_Model = std::make_shared<Model>(context, "menuOptionAlt");
	m_Texture = std::make_unique<Texture>(context, "empty.dds");
}

MenuBoxAlt::~MenuBoxAlt()
{
}

void MenuBoxAlt::setTexture(Context* context, const std::string& file)
{
	m_Texture = std::make_unique<Texture>(context, file);
}

void MenuBoxAlt::step(double dt)
{
	SceneObject::step(dt);
}

void MenuBoxAlt::predraw(Context* context)
{
	SceneObject::predraw(context);
}

void MenuBoxAlt::draw(Context* context, std::shared_ptr<ShaderManager> shaders)
{
	std::shared_ptr<ShaderProgram> shader = shaders->get("texbox");
	updateModelMatrixState(context, shader);

	m_Texture->bind(context);
	m_Model->draw(context, shader);
}