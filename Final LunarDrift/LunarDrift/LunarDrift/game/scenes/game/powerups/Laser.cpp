#include "Laser.h"
#include "../../../../engine/comms/MessageSystem.h"
#include "../../../../engine/components/CollisionComponent.h"
#include "../../../../engine/components/PhysicsComponent.h"
#include "../../../../engine/comms/predefined/CollisionMessage.h"
#include "../../../../engine/comms/predefined/DeathMessage.h"

Laser::Laser(Context* context,
	std::weak_ptr<MetaManager> managers,
	const glm::vec3& position)
	: PowerUp(context, managers, position)
{
	setPosition(position);
	MessageSystem::getInstance().subscribe("Death", this);
	addComponent(std::make_shared<PhysicsComponent>(this, managers, 1.0f));

	m_Model = std::make_shared<Model>(context, "Laser");
	m_Texture = std::make_unique<Texture>(context, "HoopGreen.dds");
}


Laser::~Laser()
{
	MessageSystem::getInstance().unsubscribe("Death", this);
}

void Laser::step(double dt)
{
	SceneObject::step(dt);
}

void Laser::predraw(Context* context)
{
	SceneObject::predraw(context);
}

void Laser::draw(Context* context, std::shared_ptr<ShaderManager> shaders)
{
	std::shared_ptr<ShaderProgram> shader = shaders->get("texbox");
	updateModelMatrixState(context, shader);
	m_Texture->bind(context);
	m_Model->draw(context, shader);
}

void Laser::onMessage(std::shared_ptr<Message> event)
{
	if (event->getType().compare("Death") == 0)
	{
		std::shared_ptr<DeathMessage> dMsg =
			std::static_pointer_cast<DeathMessage>(event);

		if (dMsg->getDoomed() == this)
			setDying();
	}


}