#include "PropulsionComponent.h"
#include "PhysicsComponent.h"
#include "../managers/SceneObjectManager.h"
#include "../comms/MessageSystem.h"
#include "../MathHelper.h"
#include <ctime>


PropulsionComponent::PropulsionComponent(
	SceneObject* parent,
	std::weak_ptr<MetaManager> managers,
	Context* context,
	std::vector<PropulsionJet> jets)
    : SceneObjectComponent("propulsion", parent, managers),
	  m_Jets(jets)
{
	MessageSystem::getInstance().subscribe("Death", this);

	std::shared_ptr<SceneObjectManager> som =
		std::static_pointer_cast<SceneObjectManager>(
			managers.lock()->get("scene").lock());
	if (som == nullptr)
		return;
	
	for (PropulsionJet& jet : m_Jets)
	{
		std::unique_ptr<PARTICLE_SYSTEM_DESCRIPTION> desc =
			std::make_unique<PARTICLE_SYSTEM_DESCRIPTION>();
		desc->emissionRadius = jet.gfxDesc.emissionRadius;
		desc->emissionRate = jet.gfxDesc.emissionRate;
		desc->speed = jet.gfxDesc.speed;
		desc->speedVariance = jet.gfxDesc.speedVariance;
		desc->lifeTime = jet.gfxDesc.lifeTime;
		desc->lifeTimeVariance = jet.gfxDesc.lifeTimeVariance;
		desc->startColor = jet.gfxDesc.startColor;
		desc->decay = jet.gfxDesc.decay;
		desc->decayTime = jet.gfxDesc.decayTime;
		desc->active = jet.active;

		std::shared_ptr<ParticleSystem> ps = std::make_shared<ParticleSystem>(
			context, managers, std::move(desc));
		m_ParticleSystems.push_back(ps);
		som->addSceneObject(ps);
	}
}

PropulsionComponent::~PropulsionComponent()
{
    MessageSystem::getInstance().unsubscribe("Death", this);

	for (std::shared_ptr<ParticleSystem> ps : m_ParticleSystems)
		ps->setDying();
}


void PropulsionComponent::step(double dt)
{
	std::shared_ptr<PhysicsComponent> pc =
		std::static_pointer_cast<PhysicsComponent>(
			getParent()->getComponent("physics"));

	glm::mat4 mm;
	getParent()->getModelMatrix(&mm);
	glm::vec2 rotations(getParent()->getPitch(), getParent()->getYaw());

	for (unsigned int i = 0; i < m_Jets.size(); ++i)
	{
		PropulsionJet& jet = m_Jets[i];
		std::shared_ptr<ParticleSystem> ps = m_ParticleSystems[i];

		ps->setPosition(glm::vec3(mm * glm::vec4(-jet.offset, 1.0)));
		ps->setDirection(glm::vec3(mm * glm::vec4(-jet.direction, 0.0)));

		if ((pc != nullptr) && jet.active)
		{
			glm::vec3 direction(jet.direction);
			if (glm::length(jet.offset) < 0.1f)
			{
				direction = glm::vec3(mm * glm::vec4(direction, 0.0));
				int x = 5 - 3;
			}

			pc->setForce({ direction, jet.offset, jet.forceMagnitude }, jet.id);
		}
	}
}

void PropulsionComponent::predraw(Context* context)
{
}

void PropulsionComponent::onMessage(std::shared_ptr<Message> msg)
{
	if (msg->getType().compare("Death") == 0)
	{
		/*std::shared_ptr<DeathMessage> dMsg = 
            std::static_pointer_cast<DeathMessage>(msg);
		SceneObject* dead = dMsg->getDoomed();
        if (dead == getParent())
            makeExplosion();*/
	}
}

void PropulsionComponent::setJetActive(int id, bool state)
{
	for (unsigned int i = 0; i < m_Jets.size(); ++i)
	{
		if (m_Jets[i].id == id)
		{
			if (m_Jets[i].active ^ state)
				toggleJetState(id);
			return;
		}
	}
}

bool PropulsionComponent::isJetActive(int id) const
{
	for (unsigned int i = 0; i < m_Jets.size(); ++i)
	{
		if (m_Jets[i].id == id)
		{
			return m_Jets[i].active;
		}
	}

	return false;
}

void PropulsionComponent::toggleJetState(int id)
{
	for (unsigned int i = 0; i < m_Jets.size(); ++i)
	{
		PropulsionJet& jet = m_Jets[i];
		if (jet.id == id)
		{
			jet.active = !jet.active;
			m_ParticleSystems[i]->setActive(jet.active);

			std::shared_ptr<PhysicsComponent> pc =
				std::static_pointer_cast<PhysicsComponent>(
					getParent()->getComponent("physics"));
			if (pc != nullptr)
			{
				if (jet.active)
					pc->applyForce(jet.forceMagnitude, false, jet.direction, jet.offset, true, id);
				else
					pc->removeForce(id);
			}

			return;
		}
	}
}
