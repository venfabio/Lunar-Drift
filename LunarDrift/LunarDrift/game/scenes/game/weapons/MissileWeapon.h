#pragma once
#include "../../../../engine/graphics/IDeferredLoadable.h"
#include "../../../../engine/comms/IMessageListener.h"
#include "../../../../engine/containers/SceneObject.h"
#include "../../../../engine/containers/Model.h"
#include "../ships/Ship.h"

/**
 * @brief Missile class; incomplete and not entirely functional
 *
 * @author Aiman
 * @date   26/05/2016
 * @ingroup Game
 */
class MissileWeapon : public SceneObject, public IMessageListener, public IDeferredLoadable
{

public:
	/** @brief Default constructor */
	MissileWeapon(
		std::weak_ptr<MetaManager> managers,
		const glm::vec3& position,
		std::weak_ptr<Ship> target);

	/** @brief Default destructor */
	~MissileWeapon();


	/**
	 * @brief Override the direction of the missile
	 * @param dir Direction of missile
	 */
	void setDirection(const glm::vec3& dir);

	///////////////////////////////////////////////////////////////////////////
	// IDeferredLoadable
	///////////////////////////////////////////////////////////////////////////
	/**
	* @brief Called to load the object
	* @param context Graphics context
	*/
	void load(Context* context) override;

	/**
	* @brief Called to destroy the object
	* @param context Graphics context
	*/
	void unload(Context* context) override;


	///////////////////////////////////////////////////////////////////////////
	// SceneObject
	///////////////////////////////////////////////////////////////////////////
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
	* @brief Draw the object in its current state
	* @param context Graphics context
	* @param shaders Shader manager containing shader programs for use
	*/
	void draw(Context* context, std::shared_ptr<ShaderManager> shaders) override;

	/**
	* @brief Called when a subscribed event is raised
	* @param event Smart pointer to the raised event object
	*/
	void onMessage(std::shared_ptr<Message> event) override;

private:
	std::shared_ptr<Model> m_Model; 
	std::weak_ptr<Ship> m_Target;
};



