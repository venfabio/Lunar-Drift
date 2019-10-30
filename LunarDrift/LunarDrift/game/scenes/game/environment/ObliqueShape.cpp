#include "ObliqueShape.h"
#include "../../../../engine/components/PhysicsComponent.h"

ObliqueShape::ObliqueShape(
	Context* context,
	std::weak_ptr<MetaManager> managers,
	const glm::vec3& position)
	:SceneObject(context, managers)
{
	setPosition(position);

	m_Model = std::make_shared<Model>(context, "ObliqueShape");
	m_Texture = std::make_unique<Texture>(context, "moon.dds");
}

ObliqueShape::~ObliqueShape()
{
}

void ObliqueShape::step(double dt)
{
	SceneObject::step(dt);

	GLfloat rotationSpeed = 1.0f;
	setYaw(getYaw() + (static_cast<float>(dt * rotationSpeed)));
}

void ObliqueShape::predraw(Context* context)
{
	SceneObject::predraw(context);

}

void ObliqueShape::draw(Context* context, std::shared_ptr<ShaderManager> shaders)
{
	std::shared_ptr<ShaderProgram> shader = shaders->get("texbox");
	updateModelMatrixState(context, shader);

	m_Texture->bind(context);
	m_Model->draw(context, shader);
}
