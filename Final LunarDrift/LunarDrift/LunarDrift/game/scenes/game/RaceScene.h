#pragma once

#include "../../../engine/containers/Scene.h"
#include "../../../engine/comms/IMessageListener.h"
#include  "../../../game/scenes/game/ships/Ship.h"
#include "../../GameParameters.h"

/**
 * @brief Specialised scene class for use as a parent for all tracks
 *
 * @author Hayley Hatton
 * @date   20/05/2016
 * @ingroup Game
 */
class RaceScene : public Scene, public IMessageListener
{
public:
    /**
     * @brief Default constructor
     * @param context Graphics context
	 * @param params Previously selected parameters via menus for the game to use for dynamic play
	 * @param skyboxDir Folder of the skybox to use
     */
    RaceScene(
		Context* context, 
		const GameParameters& params, 
		const std::string& skyboxDir);

    /** @brief Default destructor */
    virtual ~RaceScene();


	/**
	 * @brief Determine if the scene is multiplayer
	 * @return True if multiplayer; false if single player
	 */
	bool isMultiplayer() const { return m_IsMultiplayer; }

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
	 * @brief Notify the scene that the renderer intends to perform a given pass
     * @param context Graphics context
	 * @param Pass number, with 0 being the first pass in the frame
	 */
	void setRenderPass(Context* context, unsigned int pass) override;

    /**
     * @brief Called when a subscribed event is raised
     * @param event Smart pointer to the raised event object
     */
    void onMessage(std::shared_ptr<Message> event) override;

	int getPlayer1Score() const { return m_ScoreP1; }

	int getPlayer2Score() const { return m_ScoreP2; }

protected:
	/**
	 * @brief Called when the birds eye view camera for the scene is needed
	 * @param context Graphics context
	 * @param event Smart pointer to the appropriate camera
	 */
	virtual std::shared_ptr<Camera> getBirdsEyeCam(Context* context, const glm::vec3& pos) const = 0;

private:
	std::shared_ptr<Camera> m_StandardCamera;
	std::shared_ptr<Ship> m_Player1, m_Player2;
	double m_Time;
	GLfloat m_Distance, m_RotAngle;
	GLfloat m_DistanceP2, m_RotAngleP2;
	int m_ScoreP1, m_ScoreP2;
	bool m_IsMultiplayer;
	bool m_IsBirdsEyeView, m_IsBirdsEyeViewP2;
};
