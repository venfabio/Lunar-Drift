#include "HitpointComponent.h"
#include "../containers/SceneObject.h"
#include "../comms/MessageSystem.h"
#include "../comms/predefined/DamageMessage.h"
#include "../comms/predefined/DeathMessage.h"


HitpointComponent::HitpointComponent(
	SceneObject* parent, 
	std::weak_ptr<MetaManager> managers,
	int hitpoints) 
	: SceneObjectComponent("hitpoint", parent, managers),
	  m_MaxHitpoints(hitpoints),
	  m_Hitpoints(hitpoints)
{
	MessageSystem::getInstance().subscribe("Damage", this);
}

HitpointComponent::~HitpointComponent()
{
	MessageSystem::getInstance().unsubscribe("Damage", this);
}


void HitpointComponent::onMessage(std::shared_ptr<Message> msg)
{
	if (msg->getType().compare("Damage") == 0)
	{
		std::shared_ptr<DamageMessage> dmgMsg =
			std::static_pointer_cast<DamageMessage>(msg);
		if (dmgMsg->getTarget() == getParent())
		{
			deductHitpoints(dmgMsg->getDamage());
			if (m_Hitpoints <= 0)
			{
				std::shared_ptr<DeathMessage> dMsg = 
					std::make_shared<DeathMessage>(
						getParent(), dmgMsg->getSource());
				MessageSystem::getInstance().broadcast(dMsg);
			}
		}
	}
}

void HitpointComponent::setHitpoints(int hp)
{
	m_Hitpoints = hp;
	if (m_Hitpoints > m_MaxHitpoints)
		m_Hitpoints = m_MaxHitpoints;
}

void HitpointComponent::deductHitpoints(int hp)
{
	m_Hitpoints -= hp;
	if (m_Hitpoints > m_MaxHitpoints)
		m_Hitpoints = m_MaxHitpoints;
}
