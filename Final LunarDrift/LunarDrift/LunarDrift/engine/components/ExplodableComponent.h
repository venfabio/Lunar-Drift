#pragma once

#include "SceneObjectComponent.h"
#include "../comms/IMessageListener.h"
#include "../managers/MetaManager.h"
#include "../prefabs/Explosion.h"

/**
 * @brief Attach to a scene object to make it explode upon death
 *
 * This receives DeathMessages and adds appropriate Explosion objects to the
 * scene.
 *
 * @author Hayley Hatton
 * @date   17/03/2016
 * @see	   DeathMessage
 * @see    Explosion
 * @ingroup Components
 */
class ExplodableComponent : public SceneObjectComponent, public IMessageListener
{
public:
    /**
     * @brief Default constructor
     * @param parent Pointer to the scene object to attach component to
     * @param managers Metamanager for accessing scene's managers
     * @param desc Managed ptr to explosion description information struct
     */
	ExplodableComponent(
		SceneObject* parent,
		std::weak_ptr<MetaManager> managers,
		std::unique_ptr<EXPLOSION_DESCRIPTION> desc);

    /** @brief Default destructor */
	~ExplodableComponent();

    /**
     * @brief Step the simulation state
     * @param dt Delta-time since last step call in seconds
     */
    void step(double dt) override;

    /**
     * @brief Called before the renderer draws the scene
     * This allows the scene objects to do updates that require a
     * graphics context before the graphical state is drawn
     * @param context Graphics context
     */
    void predraw(Context* context) override;

    /**
     * @brief Called when a message is sent to this entity
     * @param msg Smart pointer to the received message object
     */
    void onMessage(std::shared_ptr<Message> msg) override;

private:
	std::unique_ptr<EXPLOSION_DESCRIPTION> m_Desc;

    /** @brief Boom! :D */
    void makeExplosion();
};
