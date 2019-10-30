#pragma once

#include "../../../engine/containers/Scene.h"
#include "../../../engine/comms/IMessageListener.h"
#include "../game/environment/Planet.h"
#include "MenuBox.h"
#include "Cursor.h"

/**
 * @brief First scene; main menu, for choosing game modes and accessing scores
 *
 * @author Mariana
 * @date   26/05/2016
 * @ingroup Game
 */
class MainMenuScene : public Scene, public IMessageListener
{
public:
    /**
     * @brief Default constructor
     * @param context Graphics context
     */
    MainMenuScene(Context* context);

    /** @brief Default destructor */
    ~MainMenuScene();


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

	enum class Option               //!< Current Main Menu option
	{
		SinglePlayer,
		Multiplayer,
		Settings,
		Quit
	} m_MainMenuOption;
protected:

private:
	GLfloat m_Distance;
	std::shared_ptr<MenuBox> m_MenuTitle;
	std::shared_ptr<MenuBox> m_MenuOption1;
	std::shared_ptr<MenuBox> m_MenuOption2;
	std::shared_ptr<MenuBox> m_MenuOption3;
	std::shared_ptr<MenuBox> m_MenuOption4;
	std::shared_ptr<Cursor> m_Cursor;
	std::shared_ptr<Planet> m_Moon;
};
