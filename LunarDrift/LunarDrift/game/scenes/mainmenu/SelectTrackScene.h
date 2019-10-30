#pragma once
#include "../../../engine/containers/Scene.h"
#include "../../../engine/comms/IMessageListener.h"
#include "MenuBox.h"
#include "Cursor.h"

/**
 * @brief Scene class for selecting which track to race on
 *
 * @author Mariana
 * @date   26/05/2016
 * @ingroup Game
 */
class SelectTrackScene : public Scene, public IMessageListener
{
public:
	/**
	* @brief Default constructor
	* @param context Graphics context
	*/
	SelectTrackScene(Context* context);

	/** @brief Default destructor */
	~SelectTrackScene();

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

	enum class Option         //!< Current Track option
	{
		Antares,
		Andromeda,
		KesselRun
	} m_TrackOption;

private:
	GLfloat m_Distance;
	std::shared_ptr<MenuBox> m_TrackTitle;
	std::shared_ptr<MenuBox> m_TrackOption1;
	std::shared_ptr<MenuBox> m_TrackOption2;
	std::shared_ptr<MenuBox> m_TrackOption3;
	std::shared_ptr<Cursor> m_Cursor;
};

