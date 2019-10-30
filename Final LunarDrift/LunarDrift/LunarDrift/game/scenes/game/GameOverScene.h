#pragma once
#include "../../../engine/containers/Scene.h"
#include "../../../engine/comms/IMessageListener.h"
#include "../mainmenu/MenuBox.h"
#include "../mainmenu/MenuBoxAlt.h"
#include "../mainmenu/Cursor.h"

/**
 * @brief Scene class for showing when the race is over
 *
 * @author Mariana
 * @date   26/05/2016
 * @ingroup Game
 */
class GameOverScene : public Scene, public IMessageListener
{
public:
	/**
	* @brief Default constructor
	* @param context Graphics context
	*/
	GameOverScene(Context* context);

	/** @brief Default destructor */
	~GameOverScene();

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
	GLfloat m_Distance;
	std::shared_ptr<MenuBox> m_GameOverTitle;
	std::shared_ptr<MenuBoxAlt> m_DerpySmoking;
	std::shared_ptr<MenuBox> m_Replay;
};

