#include "LifeTimeComponent.h"
#include "../comms/predefined/DeathMessage.h"
#include "../comms/MessageSystem.h"
#include "../containers/SceneObject.h"


LifeTimeComponent::LifeTimeComponent(
	SceneObject* parent,
	std::weak_ptr<MetaManager> managers,
	double lifeSpan) 
	: SceneObjectComponent("lifetime", parent, managers),
	  m_InitialLifeSpan(lifeSpan),
	  m_LifeSpan(lifeSpan),
	  m_Triggered(false)
{
}

LifeTimeComponent::~LifeTimeComponent()
{
}


void LifeTimeComponent::step(double dt)
{
	m_LifeSpan -= dt;
	if (m_LifeSpan <= 0 && !m_Triggered)
	{
		m_Triggered = true;

        MessageSystem::getInstance().broadcast(
            std::make_shared<DeathMessage>(getParent()));
	}
}

void LifeTimeComponent::reset()
{
	m_LifeSpan = m_InitialLifeSpan;
	m_Triggered = false;
}
