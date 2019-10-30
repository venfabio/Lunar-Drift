#pragma once

#include "../../../engine/containers/Scene.h"
#include "../../../engine/comms/IMessageListener.h"
#include "../game/hoops/HoopBlue.h"
#include "../game/hoops/HoopRed.h"
#include "../game/hoops/HoopGreen.h"
#include "../game/hoops/HoopYellow.h"
#include "TestSpacecraft.h"
#include "../../GameParameters.h"
#include "../game/ships/Ship.h"
#include "../game/powerups/PowerUp.h"

/**
 * @defgroup TestScene
 * @brief Prototypes and demonstrations for how to use certain facets of the engine
 */

/**
 * @brief Testbed scene for testing stuff before deployment
 *
 * @author Hayley Hatton
 * @date   16/03/2016
 * @ingroup TestScene
 */
class TestScene : public Scene, public IMessageListener
{
public:
    /**
     * @brief Default constructor
     * @param context Graphics context
     */
	TestScene(Context* context, const GameParameters& params);

    /** @brief Default destructor */
    virtual ~TestScene();


    /**
     * @brief Called to update scene elements with the viewport information
     * @param width Width of the viewport in pixels
     * @param height Height of the viewport in pixels
     */
    void setSize(GLsizei width, GLsizei height) override;

    /**
     * @brief Step the scene's simulation state
     * @param dt Delta-time since last step call in seconds
     */
    void step(double dt) override;

    /**
     * @brief Called before the renderer draws the scene
     * This allows the scene and its constituent objects to do updates that
     * require a graphics context before the graphical state is drawn
     * @param context Graphics context
     */
    void predraw(Context* context) override;


    /**
     * @brief Called when a subscribed event is raised
     * @param event Smart pointer to the raised event object
     */
    void onMessage(std::shared_ptr<Message> event) override;


private:
	std::shared_ptr<Ship> m_PlayerShip;
	std::shared_ptr<PowerUp> m_pwr;
    GLfloat m_Distance, m_RotAngle;
};
