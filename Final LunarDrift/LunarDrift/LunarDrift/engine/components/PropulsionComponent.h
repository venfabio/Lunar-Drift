#pragma once

#include "SceneObjectComponent.h"
#include "../comms/IMessageListener.h"
#include "../managers/MetaManager.h"
#include "../prefabs/ParticleSystem.h"


/**
* @brief Individual propulsion jet graphical representation metadata
*
* @author Hayley Hatton
* @date   18/05/2016
* @see    ParticleSystem
*/
struct JetDescription
{
	float emissionRadius;
	unsigned int emissionRate;
	float speed;
	float speedVariance;
	float lifeTime;
	float lifeTimeVariance;
	glm::vec4 startColor;
	glm::vec4 decay;
	float decayTime;
};

/**
* @brief Individual propulsion jet metadata
*
* @author Hayley Hatton
* @date   18/05/2016
* @see    ParticleSystem
*/
struct PropulsionJet
{
	int id;
	glm::vec3 offset;
	glm::vec3 direction;
	JetDescription gfxDesc;
	float forceMagnitude;
	bool active;
};

/**
 * @brief Attach to a scene object to give it physics-enabled thrusters
 *
 * @author Hayley Hatton
 * @date   18/05/2016
 * @see    ParticleSystem
 * @ingroup Components
 */
class PropulsionComponent : public SceneObjectComponent, public IMessageListener
{
public:
    /**
     * @brief Default constructor
     * @param parent Pointer to the scene object to attach component to
     * @param managers Metamanager for accessing scene's managers
     * @param context Graphics context
     * @param jets Initial jet state
     */
	PropulsionComponent(
		SceneObject* parent,
		std::weak_ptr<MetaManager> managers,
		Context* context,
		std::vector<PropulsionJet> jets);

    /** @brief Default destructor */
	~PropulsionComponent();

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

	/**
	 * @brief Set whether a jet is active or not
	 * @param id ID of the jet to toggle
	 * @param state True to make active; false to make inactive
	 */
	void setJetActive(int id, bool state);

	/**
	* @brief Check whether a jet is active or not
	* @param id ID of the jet to test
	* @param state True if active; false if inactive
	*/
	bool isJetActive(int id) const;

	/**
	 * @brief Flip the current emission state of the jet
	 * @param id ID of the jet to toggle
	 */
	void toggleJetState(int id);

private:
	std::vector<PropulsionJet> m_Jets;
	std::vector<std::shared_ptr<ParticleSystem>> m_ParticleSystems;
};
