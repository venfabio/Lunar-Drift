#pragma once

#include "../../../../engine/containers/SceneObject.h"
#include "../../../../engine/containers/Model.h"
#include "../../../../engine/comms/IMessageListener.h"

/**
 * @brief Base class for ships in the game
 *
 * @author Hayley Hatton
 * @date   20/04/2016
 * @ingroup Game
 */
class Ship : public SceneObject, public IMessageListener
{
public:
    /** 
     * @brief Default constructor 
     * @param context Graphics context
     * @param managers Weak reference to scene's metamanager
	 * @param maxHP Maximum/starting HP of the ship
	 * @param isPlayer2 Set to true to make player 2 (optional)
     */
    Ship(
        Context* context, 
		std::weak_ptr<MetaManager> managers,
		const glm::vec3& position, 
        unsigned int maxHP,
		bool isHardMode,
		bool isPlayer2 = false);

    /** @brief Default destructor */
    ~Ship();

	enum class PowerUps
	{
		None,
		Laser,
		Meteor,
		Turbo
	} m_CurrentPowerUp;

    /**
     * @brief Step the simulation state
     * @param dt Delta-time since last step call in seconds
     */
    virtual void step(double dt) override;

    /**
     * @brief Called before the renderer draws the scene
     *
     * This allows the scene objects to do updates that require a
     * graphics context before the graphical state is drawn
     * @param context Graphics context
     */
    virtual void predraw(Context* context) override;

    /**
     * @brief Draw the object in its current state
     * @param context Graphics context
     * @param shaders Shader manager containing shader programs for use
     */
    virtual void draw(Context* context, std::shared_ptr<ShaderManager> shaders) override;

    /**
     * @brief Called when a subscribed event is raised
     * @param event Smart pointer to the raised event object
     */
	virtual void onMessage(std::shared_ptr<Message> event) override;

	/**
	* @brief Test if player 2 controls this ship
	* @return True if player 2 controls; false if player 1 controls
	*/
	bool isPlayer2Controlled() const { return m_IsPlayer2; }

	void SetPowerUp(PowerUps p) { m_CurrentPowerUp = p; }

	void addFuel(double additionalFuel) { m_Fuel += additionalFuel; }

	double getRemainingFuel() const { return m_Fuel; }

protected:
    /**
     * @brief Set the model of the ship
     * @param model Smart pointer to the model instance
     */
    void setModel(std::shared_ptr<Model> model) { m_Model = model; }

private:
    std::shared_ptr<Model> m_Model; //!< Active model instance
	double m_Fuel;					//!< Remaining fuel
	bool m_IsPlayer2;				//!< Set to true to bind to player 2
};
