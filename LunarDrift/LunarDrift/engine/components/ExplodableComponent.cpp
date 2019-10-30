#include "ExplodableComponent.h"
#include "../managers/SceneObjectManager.h"
#include "../comms/predefined/DeathMessage.h"
#include "../comms/MessageSystem.h"
#include "../MathHelper.h"
#include <ctime>


ExplodableComponent::ExplodableComponent(
	SceneObject* parent,
	std::weak_ptr<MetaManager> managers,
	std::unique_ptr<EXPLOSION_DESCRIPTION> desc) 
    : SceneObjectComponent("explodable", parent, managers),
	  m_Desc(std::move(desc))
{
	MessageSystem::getInstance().subscribe("Death", this);
}

ExplodableComponent::~ExplodableComponent()
{
    MessageSystem::getInstance().unsubscribe("Death", this);
}


void ExplodableComponent::step(double dt)
{
}

void ExplodableComponent::predraw(Context* context)
{
}

void ExplodableComponent::makeExplosion()
{
    std::shared_ptr<MetaManager> mgr = getMetaManager().lock();
    if (mgr == nullptr)
        return;

    std::shared_ptr<SceneObjectManager> som =
        std::static_pointer_cast<SceneObjectManager>(
			mgr->get("scene").lock());
    if (som == nullptr)
        return;

    std::shared_ptr<Explosion> boom =
        std::make_shared<Explosion>(getMetaManager(), std::move(m_Desc));
    boom->setPosition(getParent()->getPosition());

    som->addSceneObject(boom);
}

void ExplodableComponent::onMessage(std::shared_ptr<Message> msg)
{
	if (msg->getType().compare("Death") == 0)
	{
		std::shared_ptr<DeathMessage> dMsg = 
            std::static_pointer_cast<DeathMessage>(msg);
		SceneObject* dead = dMsg->getDoomed();
        if (dead == getParent())
            makeExplosion();
	}
}
