#include "Planet.h"
#include "../../../../engine/components/PhysicsComponent.h"

Planet::Planet(
	Context* context,
	std::weak_ptr<MetaManager> managers,
	const glm::vec3& position)
	:SceneObject(context, managers)
{
	setPosition(position);

	m_Model = std::make_shared<Model>(context, "planet");
	m_Texture = std::make_unique<Texture>(context, "moon.dds");
}

Planet::~Planet()
{
}

void Planet::step(double dt)
{
	SceneObject::step(dt);

	GLfloat rotationSpeed = 1.0f;
	setYaw(getYaw() + (static_cast<float>(dt * rotationSpeed)));
}

void Planet::predraw(Context* context)
{
	SceneObject::predraw(context);

}

void Planet::draw(Context* context, std::shared_ptr<ShaderManager> shaders)
{
	std::shared_ptr<ShaderProgram> shader = shaders->get("texbox");
	updateModelMatrixState(context, shader);

	m_Texture->bind(context);
	m_Model->draw(context, shader);
}
