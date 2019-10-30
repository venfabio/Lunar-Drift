#include "Arch.h"
#include "../../../../engine/components/PhysicsComponent.h"

Arch::Arch(
	Context* context,
	std::weak_ptr<MetaManager> managers,
	const glm::vec3& position)
	:SceneObject(context, managers)
{
	setPosition(position);

	m_Model = std::make_shared<Model>(context, "Arch");
	m_Texture = std::make_unique<Texture>(context, "Grey.dds");
}

Arch::~Arch()
{
}

void Arch::step(double dt)
{
	SceneObject::step(dt);

	GLfloat rotationSpeed = 1.0f;
	setYaw(getYaw() + (static_cast<float>(dt * rotationSpeed)));
}

void Arch::predraw(Context* context)
{
	SceneObject::predraw(context);

}

void Arch::draw(Context* context, std::shared_ptr<ShaderManager> shaders)
{
	std::shared_ptr<ShaderProgram> shader = shaders->get("texbox");
	updateModelMatrixState(context, shader);

	m_Texture->bind(context);
	m_Model->draw(context, shader);
}
