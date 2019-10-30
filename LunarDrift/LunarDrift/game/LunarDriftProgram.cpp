#include "LunarDriftProgram.h"
#include "GameRenderer.h"
#include "../engine/input/InputMap.h"
#include "scenes/alphaspace/TestScene.h"
#include "scenes/mainmenu/MainMenuScene.h"
#include "scenes/mainmenu/SelectShipScene.h"
#include "scenes/mainmenu/SelectLevelScene.h"
#include "scenes/mainmenu/SelectTrackScene.h"
#include "scenes/game/ScoreDisplayScene.h"
#include "scenes/game/GameOverScene.h"
#include "scenes/game/AntaresRaceScene.h"
#include "scenes/game/AndromedaRaceScene.h"
#include "scenes/game/KesselRunRaceScene.h"
#include "scenes/game/PauseScene.h"
#include "../engine/Config.h"
#include "../engine/comms/MessageSystem.h"
#include "../engine/comms/predefined/ButtonDownEvent.h"
#include "../engine/comms/predefined/SceneActivateMessage.h"
#include "../engine/exceptions/FileIOException.h"
#include "../engine/exceptions/UnsupportedFeatureException.h"

const std::string LunarDriftProgram::ms_Name = "Lunar Drift";

LunarDriftProgram::LunarDriftProgram(Context* context)
    : Program(ms_Name, context),
	  m_CurrentRace(nullptr),
	  m_ChoosingP2Ship(false),
	  m_ReplayStarted(false)
{
    Config::get().load("config");
    Config::get().set("_engine", "shaderPath", std::string("game/assets/graphics/shaders"));
    Config::get().set("_engine", "texturePath", std::string("game/assets/graphics/textures"));
	Config::get().set("_engine", "modelPath", std::string("game/assets/graphics/models"));
	Config::get().set("_engine", "sfxPath", std::string("game/assets/audio/sfx"));
	Config::get().set("_engine", "musicPath", std::string("game/assets/audio/music"));
	Config::get().set("_engine", "debugCollisions", false);

	memset((void*)&m_GameParams, 0, sizeof(GameParameters));

	setActiveScene(std::make_shared<MainMenuScene>(context));
	m_MenuScene = MenuScene::MainMenu;
	m_RequestedMenuScene = m_MenuScene;

	MessageSystem::getInstance().subscribe("ButtonDown", this);
	MessageSystem::getInstance().subscribe("SceneActivate", this);

	Keyboard* kbd = InputMap::getInstance().getKeyboard();
	// Menu navigation controls
	kbd->mapKeyToInput(VK_UP, "Up");
	kbd->mapKeyToInput(VK_DOWN, "Down");
	kbd->mapKeyToInput(VK_LEFT, "Left");
	kbd->mapKeyToInput(VK_RIGHT, "Right");
	kbd->mapKeyToInput(VK_RETURN, "Enter");
	kbd->mapKeyToInput(VK_BACK, "Back");

	// Player 1 controls
    kbd->mapKeyToInput('W', "Accelerate");
    kbd->mapKeyToInput('S', "Brake");
    kbd->mapKeyToInput('A', "PanLeft");
	kbd->mapKeyToInput('D', "PanRight");
	kbd->mapKeyToInput('Q', "PanUp");
	kbd->mapKeyToInput('E', "PanDown");
	kbd->mapKeyToInput('V', "YawLeft");
	kbd->mapKeyToInput('N', "YawRight");
	kbd->mapKeyToInput('B', "PitchUp");
	kbd->mapKeyToInput('G', "PitchDown");
	kbd->mapKeyToInput('Z', "EmergencyBreak");
	kbd->mapKeyToInput(VK_TAB, "BirdsEye");

	// Player 2 controls
	kbd->mapKeyToInput('I', "Accelerate2");
	kbd->mapKeyToInput('K', "Brake2");
	kbd->mapKeyToInput('J', "PanLeft2");
	kbd->mapKeyToInput('L', "PanRight2");
	kbd->mapKeyToInput('U', "PanUp2");
	kbd->mapKeyToInput('O', "PanDown2");
	kbd->mapKeyToInput(VK_NUMPAD4, "YawLeft2");
	kbd->mapKeyToInput(VK_NUMPAD6, "YawRight2");
	kbd->mapKeyToInput(VK_NUMPAD5, "PitchUp2");
	kbd->mapKeyToInput(VK_NUMPAD8, "PitchDown2");
	kbd->mapKeyToInput(VK_NUMPAD1, "EmergencyBreak2");
	kbd->mapKeyToInput(VK_NUMPAD7, "BirdsEye2");

	// Global ingame controls
	kbd->mapKeyToInput('P', "Pause");
	kbd->mapKeyToInput('R', "Replay");

	// Debugging controls
	kbd->mapKeyToInput(VK_F5, "DebugCollisions");
	kbd->mapKeyToInput(VK_F9, "PreemptiveEnd");
}

LunarDriftProgram::~LunarDriftProgram()
{
	MessageSystem::getInstance().unsubscribe("SceneActivate", this);
	MessageSystem::getInstance().unsubscribe("ButtonDown", this);
}


void LunarDriftProgram::step(double dt)
{
    Program::step(dt);

	if (m_MenuScene == MenuScene::Replay)
	{
		if (!m_ReplayStarted)
			m_ReplayStarted = true;

		if (m_InputRecorder != nullptr)
		{
			m_InputRecorder->stepReplay(dt);
			if (m_InputRecorder->completedReplay())
			{
				m_ReplayStarted = false;
				m_RequestedMenuScene = MenuScene::MainMenu;
			}
		}
	}
}

void LunarDriftProgram::frame(Context* context)
{
    Program::frame(context);

	if (m_MenuScene != m_RequestedMenuScene)
	{
		switch (m_RequestedMenuScene)
		{
			case MenuScene::MainMenu:
				m_ChoosingP2Ship = false;
				m_ReplayStarted = false;
				invalidate();
				setActiveScene(std::make_shared<MainMenuScene>(context));
				break;
			case MenuScene::Settings:
			{
				m_ReplayStarted = false;
				// go to settings menu
				//throw UnsupportedFeatureException("Settings menu", "transition not implemented yet");	// remove this!
				std::shared_ptr<RaceScene> prevRace = std::dynamic_pointer_cast<RaceScene>(getActiveScene());
				if (prevRace != nullptr)
				{
					m_ScoreP1 = prevRace->getPlayer1Score();
					m_ScoreP2 = prevRace->getPlayer2Score();

				}
				setActiveScene(std::make_shared<ScoreDisplayScene>(context, m_ScoreP1, m_ScoreP2));
				break;
			}
			case MenuScene::ShipSelect:
				setActiveScene(std::make_shared<SelectShipScene>(context, m_ChoosingP2Ship));
				break;
			case MenuScene::TrackSelect:
				setActiveScene(std::make_shared<SelectTrackScene>(context));
				break;
			case MenuScene::LevelSelect:
				setActiveScene(std::make_shared<SelectLevelScene>(context));
				break;
			case MenuScene::Race:
				switch (m_GameParams.track)
				{
					case TrackSelection::Antares:
						setActiveScene(std::make_shared<AntaresRaceScene>(context, m_GameParams)); 
						break;
					case TrackSelection::Andromeda:
						setActiveScene(std::make_shared<AndromedaRaceScene>(context, m_GameParams));
						break;
					case TrackSelection::KesselRun:
						setActiveScene(std::make_shared<KesselRunRaceScene>(context, m_GameParams));
						break;
				}
				m_InputRecorder = std::make_shared<InputRecorder>();
				m_InputRecorder->startRecording();
				InputMap::getInstance().getKeyboard()->setInputRecorder(m_InputRecorder);
				break;
			case MenuScene::TestRace:
				setActiveScene(std::make_shared<TestScene>(context, m_GameParams));
				break;
			case MenuScene::Pause:
				invalidate();
				m_CurrentRace = getActiveScene();
				setActiveScene(std::make_shared<PauseScene>(context));
				break;
			case MenuScene::Continue:
				m_RequestedMenuScene = MenuScene::Race;
				setActiveScene(m_CurrentRace);
				break;
			case MenuScene::ScoreDisplay:
				invalidate();
				m_ReplayStarted = false;
				setActiveScene(std::make_shared<ScoreDisplayScene>(context, m_ScoreP1, m_ScoreP2));
				invalidate();
				break;
			case MenuScene::Replay:
			{
				invalidate();

				switch (m_GameParams.track)
				{
					case TrackSelection::Antares:
						setActiveScene(std::make_shared<AntaresRaceScene>(context, m_GameParams));
						break;
					case TrackSelection::Andromeda:
						setActiveScene(std::make_shared<AndromedaRaceScene>(context, m_GameParams));
						break;
					case TrackSelection::KesselRun:
						setActiveScene(std::make_shared<KesselRunRaceScene>(context, m_GameParams));
						break;
				}

				m_RequestedMenuScene = MenuScene::Replay;
				m_MenuScene = m_RequestedMenuScene;
				break;
			}
			case MenuScene::GameOver:
				invalidate();
				m_ReplayStarted = false;
				setActiveScene(std::make_shared<GameOverScene>(context));
				break;
			default:
				throw std::logic_error("Scene transition not handled above");
		}
		m_MenuScene = m_RequestedMenuScene;
	}
}

void LunarDriftProgram::onMessage(std::shared_ptr<Message> event)
{
	if (event->getType() == "ButtonDown")
	{
		std::shared_ptr<ButtonDownEvent> bde =
			std::static_pointer_cast<ButtonDownEvent>(event);

		if (bde->getInputName() == "DebugCollisions")
		{
			bool dbg = Config::get().getBool("_engine", "debugCollisions");
			Config::get().set("_engine", "debugCollisions", !dbg);
		}
	}
	else if (event->getType() == "SceneActivate")
	{
		std::shared_ptr<SceneActivateMessage> bde =
			std::static_pointer_cast<SceneActivateMessage>(event);
		
		if (bde->getSceneName() == "MainMenu")
		{
			m_InputRecorder = nullptr;

			m_RequestedMenuScene = MenuScene::MainMenu;
		}
		if (bde->getSceneName() == "SinglePlayer")
		{
			m_GameParams.raceMode = RaceModeSelection::SinglePlayer;
			m_RequestedMenuScene = MenuScene::ShipSelect;
		}
		else if (bde->getSceneName() == "Multiplayer")
		{
			m_GameParams.raceMode = RaceModeSelection::Multiplayer;
			m_RequestedMenuScene = MenuScene::ShipSelect;
		}
		else if (bde->getSceneName() == "Settings")
		{
			if (m_InputRecorder != nullptr)
			{
				m_InputRecorder->stopRecording();
				InputMap::getInstance().getKeyboard()->setInputRecorder(nullptr);
			}

			m_RequestedMenuScene = MenuScene::Settings;
		}
		else if (bde->getSceneName() == "Pause")
		{
			m_RequestedMenuScene = MenuScene::Pause;
		}
		else if (bde->getSceneName() == "Continue")
		{
			m_RequestedMenuScene = MenuScene::Continue;
		}
		else if (bde->getSceneName() == "ShipSelect")
		{
			m_RequestedMenuScene = MenuScene::ShipSelect;
		}
		else if (bde->getSceneName() == "Derpy")
		{
			if (m_ChoosingP2Ship)
			{
				m_GameParams.ship2 = ShipSelection::Derpy;
				m_RequestedMenuScene = MenuScene::TrackSelect;
				m_ChoosingP2Ship = false;
			}
			else
			{
				m_GameParams.ship1 = ShipSelection::Derpy;
				if (m_GameParams.raceMode == RaceModeSelection::Multiplayer)
				{
					m_ChoosingP2Ship = true;
					m_MenuScene = MenuScene::TrackSelect;
					m_RequestedMenuScene = MenuScene::ShipSelect;
				}
				else
				{
					m_RequestedMenuScene = MenuScene::TrackSelect;
				}
			}
		}
		else if (bde->getSceneName() == "PinkiePie")
		{
			if (m_ChoosingP2Ship)
			{
				m_GameParams.ship2 = ShipSelection::PinkiePie;
				m_RequestedMenuScene = MenuScene::TrackSelect;
				m_ChoosingP2Ship = false;
			}
			else
			{
				m_GameParams.ship1 = ShipSelection::PinkiePie;
				if (m_GameParams.raceMode == RaceModeSelection::Multiplayer)
				{
					m_ChoosingP2Ship = true;
					m_MenuScene = MenuScene::TrackSelect;
					m_RequestedMenuScene = MenuScene::ShipSelect;
				}
				else
				{
					m_RequestedMenuScene = MenuScene::TrackSelect;
				}
			}
		}
		else if (bde->getSceneName() == "TrackSelect")
		{
			m_RequestedMenuScene = MenuScene::TrackSelect;
		}
		else if (bde->getSceneName() == "Antares")
		{
			m_GameParams.track = TrackSelection::Antares;
			m_RequestedMenuScene = MenuScene::LevelSelect;
		}
		else if (bde->getSceneName() == "Andromeda")
		{
			m_GameParams.track = TrackSelection::Andromeda;
			m_RequestedMenuScene = MenuScene::LevelSelect;
		}
		else if (bde->getSceneName() == "KesselRun")
		{
			m_GameParams.track = TrackSelection::KesselRun;
			m_RequestedMenuScene = MenuScene::LevelSelect;
		}
		else if (bde->getSceneName() == "LevelSelect")
		{
			m_RequestedMenuScene = MenuScene::LevelSelect;
		}
		else if (bde->getSceneName() == "Easy")
		{
			m_GameParams.difficulty = DifficultySelection::Easy;
			m_RequestedMenuScene = MenuScene::Race;
		}
		else if (bde->getSceneName() == "Expert")
		{
			m_GameParams.difficulty = DifficultySelection::Expert;
			m_RequestedMenuScene = MenuScene::Race;
		}
		else if (bde->getSceneName() == "GameOver")
		{
			if (m_InputRecorder != nullptr)
				m_InputRecorder->stopRecording();
			InputMap::getInstance().getKeyboard()->setInputRecorder(nullptr);
			m_RequestedMenuScene = MenuScene::GameOver;
		}
		else if (bde->getSceneName() == "Replay")
		{
			m_RequestedMenuScene = MenuScene::Replay;
		}
	}
}


std::shared_ptr<Renderer> LunarDriftProgram::getRenderer(Context* context) const
{
    GLsizei w, h;
    context->getDimensions(&w, &h);

    return std::make_shared<GameRenderer>(context, w, h);
}
