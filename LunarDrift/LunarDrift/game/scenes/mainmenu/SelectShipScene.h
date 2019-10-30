#pragma once
#include "../../../engine/containers/Scene.h"
#include "../../../engine/comms/IMessageListener.h"
#include "../game/ships/Derpy.h"
#include "../game/ships/PinkiePie.h"
#include "MenuBox.h"
#include "MenuBoxAlt.h"
#include "Cursor.h"

/**
 * @brief Scene class for selecting which ship to race in
 *
 * @author Mariana
 * @date   26/05/2016
 * @ingroup Game
 */
class SelectShipScene : public Scene, public IMessageListener
{
public:
	/**
	* @brief Default constructor
	* @param context Graphics context
    * @param isPlayer1 If true, we are selecting for player 2
	*/
	SelectShipScene(Context* context, bool isPlayer2);

	/** @brief Default destructor */
	~SelectShipScene();

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
	enum class ShipSelection //!< Current Ship selection
	{
		Derpy,
		PinkiePie
	} m_SelectedShip;

	ShipSelection GetShipSelection() { return m_SelectedShip; }

	GLfloat m_Distance;
	std::shared_ptr<MenuBox> m_MenuTitle;
	std::shared_ptr<MenuBoxAlt> m_DerpyPreview;
	std::shared_ptr<MenuBoxAlt> m_PinkiePiePreview;
	std::shared_ptr<Cursor> m_Cursor;
	bool m_IsPlayer2;
};