#include "PowerUp.h"
#include "../../../../engine/comms/MessageSystem.h"
#include "../../../../engine/components/CollisionComponent.h"
#include "../../../../engine/components/PhysicsComponent.h"
#include "../../../../engine/comms/predefined/CollisionMessage.h"
#include "../../../../engine/comms/predefined/DeathMessage.h"

PowerUp::PowerUp(Context* context,
	std::weak_ptr<MetaManager> managers,
	const glm::vec3& position)
	: SceneObject(context, managers)
{
	setPosition(position);
	MessageSystem::getInstance().subscribe("Death", this);
	addComponent(std::make_shared<PhysicsComponent>(this, managers, 1.0f));
}


PowerUp::~PowerUp()
{
	MessageSystem::getInstance().unsubscribe("Death", this);
}

void PowerUp::step(double dt)
{
	SceneObject::step(dt);
}

void PowerUp::predraw(Context* context)
{
	SceneObject::predraw(context);
}

void PowerUp::draw(Context* context, std::shared_ptr<ShaderManager> shaders)
{
	std::shared_ptr<ShaderProgram> shader = shaders->get("ship");
	updateModelMatrixState(context, shader);
	m_Model->draw(context, shader);
}

void PowerUp::onMessage(std::shared_ptr<Message> event)
{
	if (event->getType().compare("Death") == 0)
	{
		std::shared_ptr<DeathMessage> dMsg =
			std::static_pointer_cast<DeathMessage>(event);

		if (dMsg->getDoomed() == this)
			setDying();
	}
}