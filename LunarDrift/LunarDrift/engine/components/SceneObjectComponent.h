#pragma once

#include "BaseComponent.h"
#include "../managers/MetaManager.h"
#include "../graphics/Context.h"

class SceneObject;

/**
 * @brief Base class for components that are to be attached to scene objects
 *
 * @author Hayley Hatton
 * @date   06/03/2016
 * @ingroup Components
 */
class SceneObjectComponent : public BaseComponent
{
public:
	/**
	 * @brief Default constructor
	 * @param name Identifying name of the component class
	 * @param parent Pointer to the scene object to attach component to
	 * @param managers Metamanager for accessing scene's managers
	 */
	SceneObjectComponent(
		const std::string& name, 
		SceneObject* parent, 
		std::weak_ptr<MetaManager> managers);

	/** @brief Default destructor */
	virtual ~SceneObjectComponent();


	/**
	 * @brief Step the simulation state
	 * @param dt Delta-time since last step call in seconds
	 */
	virtual void step(double dt) = 0;

	/**
	 * @brief Called before the renderer draws the scene
	 * This allows the scene objects to do updates that require a
	 * graphics context before the graphical state is drawn
	 * @param context Graphics context
	 */
	virtual void predraw(Context* context) = 0;

	/**
	 * @brief Access the parent class that owns this component
	 * @return Reference to the parent class
	 */
	SceneObject* getParent() { return m_Parent; }

	/**
	 * @brief Access the metamanager for the scene that the parent is a part of
	 * @return Weakly-referenced pointer to the owning scene's metamanager
	 */
	std::weak_ptr<MetaManager> getMetaManager() { return m_Managers; }

protected:
	std::weak_ptr<MetaManager> m_Managers;	//!< Scene's managers

private:
	SceneObject* m_Parent;	//!< Class that has ownership of this component
};
