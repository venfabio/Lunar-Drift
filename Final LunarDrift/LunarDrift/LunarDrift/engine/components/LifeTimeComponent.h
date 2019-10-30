#pragma once

#include "SceneObjectComponent.h"

/**
 * @brief Attach to a scene object to give it a time limit to its existence
 *
 * @author Hayley Hatton
 * @date   06/03/2016
 * @see	   DeathMessage
 * @ingroup Components
 */
class LifeTimeComponent : public SceneObjectComponent
{
public:
	/**
	 * @brief Default constructor
	 * @param parent Pointer to the scene object to attach component to
	 * @param managers Metamanager for accessing scene's managers
	 * @param lifeSpan Time in seconds until the timeout is fired
	 */
	LifeTimeComponent(
		SceneObject* parent, 
		std::weak_ptr<MetaManager> managers,
		double lifeSpan
	);

	/** @brief Default destructor */
	virtual ~LifeTimeComponent();


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
	void predraw(Context* context) override {};

	/** @brief Reset the timer to the original lifespan */
	void reset();

private:
	const double m_InitialLifeSpan;	//!< The time to reset the lifespan to
	double m_LifeSpan;				//!< Time before an object dies in seconds
	bool m_Triggered;				//!< True if the timeout has activated
};
