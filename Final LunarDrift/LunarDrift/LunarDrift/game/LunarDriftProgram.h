#pragma once

#include "../engine/Program.h"
#include "../engine/comms/IMessageListener.h"
#include "../engine/input/InputRecorder.h"
#include "GameParameters.h"

/**
 * @defgroup Game
 * @brief All classes related to the front-end implementation of the game
 */

/**
 * @brief Top-level program class for our game
 *
 * @author Hayley Hatton
 * @date   17/03/2016
 * @ingroup Game
 */
class LunarDriftProgram : public Program, public IMessageListener
{
public:
    /**
     * @brief Object-oriented entry point to the program
     * @param context Graphics context
     */
	LunarDriftProgram(Context* context);

    /** @brief Default destructor */
	~LunarDriftProgram();


    /**
     * @brief Called on the stepping of the simulation state
     * @param dt Delta time, in seconds
     */
    void step(double dt) override;

    /**
     * @brief Called on the rendering of a frame
     * @param context Graphical context
     */
    void frame(Context* context) override;


    /**
     * @brief Called when a subscribed event is raised
     * @param event Smart pointer to the raised event object
     */
    void onMessage(std::shared_ptr<Message> event) override;


    static const std::string ms_Name;   //!< Name of the program


protected:
    /**
     * @brief Retrieve a new renderer for a given stereo mode from the subclass
     * @return Smart pointer to the new appropriate renderer
     */
    std::shared_ptr<Renderer> getRenderer(Context* context) const override;

private:
	std::shared_ptr<InputRecorder> m_InputRecorder;
	std::shared_ptr<Scene> m_CurrentRace;
	
	enum class MenuScene
	{
		MainMenu,
		Settings,
		ShipSelect,
		TrackSelect,
		LevelSelect,
		Race,
		TestRace,
		Pause,
		Continue,
		ScoreDisplay,
		Replay,
		GameOver
	};

	MenuScene m_MenuScene;			//!< Current Menu scene
	MenuScene m_RequestedMenuScene;

	GameParameters m_GameParams;	//!< Game parameters, for building up and feeding into race
	int m_ScoreP1, m_ScoreP2;
	bool m_ChoosingP2Ship;
	bool m_ReplayStarted;
};
