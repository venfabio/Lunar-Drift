#include "Mars.h"
#include "../../../../engine/components/PhysicsComponent.h"

Mars::Mars(
	Context* context,
	std::weak_ptr<MetaManager> managers,
	const glm::vec3& position)
	:SceneObject(context, managers)
{
	setPosition(position);

	m_Model = std::make_shared<Model>(context, "Mars");
	m_Texture = std::make_unique<Texture>(context, "Mars.dds");
}

Mars::~Mars()
{
}

void Mars::step(double dt)
{
	SceneObject::step(dt);

	GLfloat rotationSpeed = 0.2f;
	setYaw(getYaw() + (static_cast<float>(dt * rotationSpeed)));
}

void Mars::predraw(Context* context)
{
	SceneObject::predraw(context);

}

void Mars::draw(Context* context, std::shared_ptr<ShaderManager> shaders)
{
	std::shared_ptr<ShaderProgram> shader = shaders->get("texbox");
	updateModelMatrixState(context, shader);

	m_Texture->bind(context);
	m_Model->draw(context, shader);
}
