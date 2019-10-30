#include "MissileWeapon.h"
#include "../../../../engine/components/LifeTimeComponent.h"
#include "../../../../engine/components/ExplodableComponent.h"
#include "../../../../engine/components/PhysicsComponent.h"
#include "../../../../engine/comms/MessageSystem.h"
#include "../../../../engine/comms/predefined/DeathMessage.h"

const GLfloat speed = 50.f;

MissileWeapon::MissileWeapon(
	std::weak_ptr<MetaManager> managers,
	const glm::vec3& position,
	std::weak_ptr<Ship> target)
	: SceneObject(nullptr, managers),
	  m_Target(target)
{
	setPosition(position);

	std::unique_ptr<EXPLOSION_DESCRIPTION> desc =
		std::make_unique<EXPLOSION_DESCRIPTION>();
	desc->offset = glm::vec3(0.f, 0.f, 0.f);
	desc->numParticles = 5000;
	desc->originRadius = 1.f;
	desc->speed = 10.f;
	desc->speedVariance = 5.f;
	desc->lifeTime = 0.1f;
	desc->lifeTimeVariance = 3.5f;
	desc->startColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
	desc->decay = glm::vec4(0.0f, 0.8f, 2.f, 0.01f);
	desc->decayTime = 5.0f;

	addComponent(std::make_shared<ExplodableComponent>(this, managers, std::move(desc)));
	addComponent(std::make_shared<LifeTimeComponent>(this, managers, 5.0));
	addComponent(std::make_shared<PhysicsComponent>(this, managers, 1.0f));

	MessageSystem::getInstance().subscribe("Death", this);
}


MissileWeapon::~MissileWeapon()
{
	MessageSystem::getInstance().unsubscribe("Death", this);
}


void MissileWeapon::setDirection(const glm::vec3& dir)
{
	std::shared_ptr<PhysicsComponent> pc =
		std::static_pointer_cast<PhysicsComponent>(
			getComponent("physics"));

	pc->setVelocity(dir * speed);
}

void MissileWeapon::load(Context* context)
{
	m_Model = std::make_shared<Model>(context, "missile");
}

void MissileWeapon::unload(Context* context)
{

}

void MissileWeapon::step(double dt)
{
	SceneObject::step(dt);

	if (!m_Target.expired())
	{
		std::shared_ptr<Ship> target = m_Target.lock();
		if (target != nullptr)
		{
			std::shared_ptr<PhysicsComponent> pc =
				std::static_pointer_cast<PhysicsComponent>(
					getComponent("physics"));

			glm::vec3 dir = glm::normalize(target->getPosition() - getPosition());
			pc->setVelocity(dir * speed);
		}
	}
}

void MissileWeapon::predraw(Context* context)
{
	SceneObject::predraw(context);
}

void MissileWeapon::draw(Context* context, std::shared_ptr<ShaderManager> shaders)
{
	std::shared_ptr<ShaderProgram> shader = shaders->get("missile");
	updateModelMatrixState(context, shader);
	m_Model->draw(context, shader);
}

void MissileWeapon::onMessage(std::shared_ptr<Message> event)
{
	if (event->getType().compare("Death") == 0)
	{
		std::shared_ptr<DeathMessage> dMsg =
			std::static_pointer_cast<DeathMessage>(event);

		if (dMsg->getDoomed() == this)
			setDying();
	}
}