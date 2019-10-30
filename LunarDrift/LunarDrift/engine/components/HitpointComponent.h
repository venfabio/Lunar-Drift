#pragma once

#include "SceneObjectComponent.h"
#include "../comms/IMessageListener.h"

/**
 * @brief Attach to a scene object to give it hitpoints
 *
 * This receives DamageMessages, and once hp is zero, sends a DeathMessage
 * about the parent
 *
 * @author Hayley Hatton
 * @date   06/03/2016
 * @see    DamageMessage
 * @see	   DeathMessage
 * @ingroup Components
 */
class HitpointComponent : public SceneObjectComponent, public IMessageListener
{
public:
    /**
     * @brief Default constructor
     * @param parent Pointer to the scene object to attach component to
     * @param managers Metamanager for accessing scene's managers
     * @param hitpoints Initial hitpoints of the parent
     */
    HitpointComponent(
        SceneObject* parent,
        std::weak_ptr<MetaManager> managers,
        int hitpoints);

    /** @brief Default destructor */
    virtual ~HitpointComponent();



    /**
     * @brief Step the simulation state
     * @param dt Delta-time since last step call in seconds
     */
    void step(double dt) override {};

    /**
     * @brief Called before the renderer draws the scene
     * This allows the scene objects to do updates that require a
     * graphics context before the graphical state is drawn
     * @param context Graphics context
     */
    void predraw(Context* context) override {};

    /**
     * @brief Called when a message is sent to this entity
     * @param msg Smart pointer to the received message object
     */
	void onMessage(std::shared_ptr<Message> msg) override;

    /**
     * @brief Reset the hitpoints to a given amount, limited by m_MaxHitpoints
     * @param Hitpoints to reset to
     */
	void setHitpoints(int hp);

    /**
     * @brief Access the remaining hitpoints of the scene object
     * @return Hitpoints remaining
     */
	int getHitpoints() const { return m_Hitpoints; }

    /**
     * @brief Does damage to the scene object
     * 
     * This fires a DeathMessage if hp remaining less than zero.
     * This can also heal! Just do negative damage ;)
     * @param Difference in hitpoints to deal
     */
	void deductHitpoints(int hp);

    /**
    * @brief Access the maximum hitpoints of the scene object
    * @return Maximum possible hitpoints
    */
	int getMaxHitpoints() const { return m_MaxHitpoints; }

private:
	int m_MaxHitpoints;
	int m_Hitpoints;
};
