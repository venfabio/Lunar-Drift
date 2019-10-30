#include "Ship.h"
#include "../../../../engine/components/HitpointComponent.h"
#include "../../../../engine/components/PhysicsComponent.h"
#include "../../../../engine/comms/MessageSystem.h"
#include "../../../../engine/comms/predefined/DeathMessage.h"
#include "../../../../engine/comms/predefined/ButtonDownEvent.h"
#include "../../../../engine/comms/predefined/FireMissileMessage.h"
#include "../../../../engine/input/InputMap.h"

Ship::Ship(
	Context* context,
	std::weak_ptr<MetaManager> managers, const glm::vec3& position,
    unsigned int maxHP,
	bool isHardMode,
	bool isPlayer2) 
	: SceneObject(context, managers),
	  m_IsPlayer2(isPlayer2),
	  m_Fuel(0.0)
{
	setPosition(position);

    addComponent(std::make_shared<HitpointComponent>(this, managers, maxHP));
	//addComponent(std::make_shared<PhysicsComponent>(this, managers, static_cast<float>(maxHP)));

    /*std::unique_ptr<EXPLOSION_DESCRIPTION> desc = 
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
    addComponent(std::make_shared<ExplodableComponent>(this, managers, std::move(desc)));*/
	MessageSystem::getInstance().subscribe("Death", this);
	MessageSystem::getInstance().subscribe("ButtonDown", this);
}

Ship::~Ship()
{
	MessageSystem::getInstance().unsubscribe("Death", this);
	MessageSystem::getInstance().unsubscribe("ButtonDown", this);
}


void Ship::step(double dt)
{
    SceneObject::step(dt);

	const GLfloat decay = 0.3f * static_cast<float>(dt);

	Keyboard* kbd = InputMap::getInstance().getKeyboard();
	if ((kbd->isKeyDown("EmergencyBreak") && !isPlayer2Controlled()) ||
		(kbd->isKeyDown("EmergencyBreak2") && isPlayer2Controlled()))
	{
		std::shared_ptr<PhysicsComponent> pc =
			std::static_pointer_cast<PhysicsComponent>(
				getComponent("physics"));

		if (pc != nullptr)
		{
			glm::vec3 u = pc->getVelocity();
			pc->setVelocity(u * (1.0f - decay));
			glm::vec3 ur = pc->getAngularVelocity();
			pc->setAngularVelocity(ur * (1.0f - decay));
		}
	}
}

void Ship::predraw(Context* context)
{
    SceneObject::predraw(context);
}

void Ship::draw(Context* context, std::shared_ptr<ShaderManager> shaders)
{
}

void Ship::onMessage(std::shared_ptr<Message> event)
{
    if(event->getType().compare("Death") == 0)
    {
        std::shared_ptr<DeathMessage> dMsg =
            std::static_pointer_cast<DeathMessage>(event);
        
        if (dMsg->getDoomed() == this)
            setDying();
	}
	else if (event->getType().compare("ButtonDown") == 0)
	{
		std::shared_ptr<ButtonDownEvent> bde =
			std::static_pointer_cast<ButtonDownEvent>(event);

		if (bde->getInputName() == "Fire" && !isPlayer2Controlled())
		{
			std::shared_ptr<FireMissileMessage> fmm
				= std::make_shared<FireMissileMessage>(true);
			MessageSystem::getInstance().broadcast(fmm);
		}
		else if (bde->getInputName() == "Fire2" && isPlayer2Controlled())
		{
				std::shared_ptr<FireMissileMessage> fmm
					= std::make_shared<FireMissileMessage>(false);
				MessageSystem::getInstance().broadcast(fmm);
		}
	}
}
