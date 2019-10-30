#include "Hill.h"
#include "../../../../engine/components/PhysicsComponent.h"
#include "../../../../engine/components/CollisionComponent.h"

Hill::Hill(
	Context* context,
	std::weak_ptr<MetaManager> managers,
	const glm::vec3& position)
	:SceneObject(context, managers)
{
	setPosition(position);

	m_Model = std::make_shared<Model>(context, "Hill");
	m_Texture = std::make_unique<Texture>(context, "HoopGreen.dds");

	std::vector<CollisionSphere> points;
	points.push_back({ 1, glm::vec3(0, 0.0, 0.0), 0.35 }); 
	addComponent(std::make_shared<CollisionComponent>(this, managers, 1.0f, points));
}

Hill::~Hill()
{
}

void Hill::step(double dt)
{
	SceneObject::step(dt);

	GLfloat rotationSpeed = 1.0f;
	setYaw(getYaw() + (static_cast<float>(dt * rotationSpeed)));
}

void Hill::predraw(Context* context)
{
	SceneObject::predraw(context);

}

void Hill::draw(Context* context, std::shared_ptr<ShaderManager> shaders)
{
	std::shared_ptr<ShaderProgram> shader = shaders->get("texbox");
	updateModelMatrixState(context, shader);

	m_Texture->bind(context);
	m_Model->draw(context, shader);
}
