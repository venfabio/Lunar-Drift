#include "WF_Bridge.h"
#include "../../../../engine/components/PhysicsComponent.h"

WF_Bridge::WF_Bridge(
	Context* context,
	std::weak_ptr<MetaManager> managers,
	const glm::vec3& position)
	:SceneObject(context, managers)
{
	setPosition(position);

	m_Model = std::make_shared<Model>(context, "WF_Bridge");
	m_Texture = std::make_unique<Texture>(context, "moon.dds");
}

WF_Bridge::~WF_Bridge()
{
}

void WF_Bridge::step(double dt)
{
	SceneObject::step(dt);

	GLfloat rotationSpeed = 1.0f;
	setYaw(getYaw() + (static_cast<float>(dt * rotationSpeed)));
}

void WF_Bridge::predraw(Context* context)
{
	SceneObject::predraw(context);

}

void WF_Bridge::draw(Context* context, std::shared_ptr<ShaderManager> shaders)
{
	std::shared_ptr<ShaderProgram> shader = shaders->get("texbox");
	updateModelMatrixState(context, shader);

	m_Texture->bind(context);
	m_Model->draw(context, shader);
}
