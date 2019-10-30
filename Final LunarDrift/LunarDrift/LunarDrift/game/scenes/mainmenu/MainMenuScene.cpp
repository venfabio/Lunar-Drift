#include "MainMenuScene.h"
#include "../../scenes/alphaspace/TestStarfield.h"
#include "../../../engine/comms/MessageSystem.h"
#include "../../../engine/comms/predefined/ButtonDownEvent.h"
#include "../../../engine/comms/predefined/SceneActivateMessage.h"
#include "../../../engine/comms/predefined/ExitMessage.h"
#include "../../../engine/audio/AudioPlayer.h"
#include "../../../engine/Config.h"

MainMenuScene::MainMenuScene(Context* context)
	: Scene(context), 
	  m_MainMenuOption(Option::SinglePlayer),	// Default menu selection
	  m_Distance(4.5f),
	  m_MenuTitle(std::make_shared<MenuBox>(context, getManagers(), glm::vec3(0.f, .7f, 1.7f))),
	  m_MenuOption1(std::make_shared<MenuBox>(context, getManagers(), glm::vec3(0.f, .6f, 0.f))),
	  m_MenuOption2(std::make_shared<MenuBox>(context, getManagers(), glm::vec3(0.f, .2f, 0.f))),
	  m_MenuOption3(std::make_shared<MenuBox>(context, getManagers(), glm::vec3(0.f, -.2f, 0.f))),
	  m_MenuOption4(std::make_shared<MenuBox>(context, getManagers(), glm::vec3(0.f, -.6f, 0.f))),
	  m_Cursor(std::make_shared<Cursor>(context, getManagers(), glm::vec3(-1.f, .6f, 0.f))),
	  m_Moon(std::make_shared<Planet>(context, getManagers(), glm::vec3(-1.7f, 1.2f, 0.f)))
{
	AudioPlayer::getInstance().setActiveMusic("The Universe Map (Mission Select).mp3");

	m_MenuTitle->setTexture(context, "ld.dds");
	m_MenuOption1->setTexture(context, "singlePlayer.dds");
	m_MenuOption2->setTexture(context, "multiplayer.dds");
	m_MenuOption3->setTexture(context, "settings.dds");
	m_MenuOption4->setTexture(context, "quit.dds");

	addObject(std::make_shared<TestStarfield>(context, getManagers()));
	addObject(m_MenuTitle);
    addObject(m_MenuOption1);
	addObject(m_MenuOption2);
	addObject(m_MenuOption3);
	addObject(m_MenuOption4);
	addObject(m_Cursor);
	addObject(m_Moon);

	// Camera
	GLfloat nearZ = Config::get().getFloat("config", "camera:nearZ");
	GLfloat farZ = Config::get().getFloat("config", "camera:farZ");
	GLfloat fovy = Config::get().getFloat("config", "camera:fov");

	GLsizei width, height;
	context->getDimensions(&width, &height);
	std::shared_ptr<Camera> camera = std::make_shared<Camera>(
		width, height, nearZ, farZ, fovy);
	camera->lookAt(
		glm::vec3(0.f, 0.f, m_Distance),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(0.f, 1.f, 0.f));
	setActiveCamera(camera);

    MessageSystem::getInstance().subscribe("ButtonDown", this);
}

MainMenuScene::~MainMenuScene()
{
	MessageSystem::getInstance().unsubscribe("ButtonDown", this);
}


void MainMenuScene::setSize(GLsizei width, GLsizei height)
{
	Scene::setSize(width, height);
}

void MainMenuScene::step(double dt)
{
    Scene::step(dt);
}

void MainMenuScene::predraw(Context* context)
{
    Scene::predraw(context);

	// Change texture on selected option
	if (m_MainMenuOption == Option::SinglePlayer)
		m_MenuOption1->setTexture(context, "singlePlayerSelected.dds");
	else
		m_MenuOption1->setTexture(context, "singlePlayer.dds");

	if (m_MainMenuOption == Option::Multiplayer)
		m_MenuOption2->setTexture(context, "multiplayerSelected.dds");
	else
		m_MenuOption2->setTexture(context, "multiplayer.dds");

	if (m_MainMenuOption == Option::Settings)
		m_MenuOption3->setTexture(context, "settingsSelected.dds");
	else
		m_MenuOption3->setTexture(context, "settings.dds");

	if (m_MainMenuOption == Option::Quit)
		m_MenuOption4->setTexture(context, "quitSelected.dds");
	else
		m_MenuOption4->setTexture(context, "quit.dds");
}

void MainMenuScene::onMessage(std::shared_ptr<Message> event)
{
    if(event->getType().compare("ButtonDown") == 0)
    {
        std::shared_ptr<ButtonDownEvent> bde =
            std::dynamic_pointer_cast<ButtonDownEvent>(event);

		float limitUp = 0.3f;
		float limitDown = -0.3f;
		glm::vec3 pos = m_Cursor->getPosition();


		// Move the cursor based on keyboard input
		if ((bde->getInputName() == "Down") && (pos.y >= limitDown))
		{
			pos.y -= 0.4f;
			m_Cursor->setPosition(pos);
		}
		else if ((bde->getInputName() == "Up") && (pos.y <= limitUp))
		{
			pos.y += 0.4f;
			m_Cursor->setPosition(pos);
		}

		// Set Menu option based on the position of the cursor
		if (pos.y == 0.6f)
			m_MainMenuOption = Option::SinglePlayer;
		else if (pos.y <= 0.21f && pos.y >= 0.19f)
			m_MainMenuOption = Option::Multiplayer;
		else if (pos.y >= -0.21f && pos.y <= -0.19f)
			m_MainMenuOption = Option::Settings;
		else if (pos.y == -0.6f)
			m_MainMenuOption = Option::Quit;

		// Take action according to the option selected
		if (bde->getInputName() == "Enter")
		{
			switch (m_MainMenuOption)
			{
				case Option::SinglePlayer:
				{
					std::shared_ptr<SceneActivateMessage> sam = 
						std::make_shared<SceneActivateMessage>("SinglePlayer");
					MessageSystem::getInstance().broadcast(sam);
					return;
				}

				case Option::Multiplayer:
				{
					std::shared_ptr<SceneActivateMessage> sam =
						std::make_shared<SceneActivateMessage>("Multiplayer");
					MessageSystem::getInstance().broadcast(sam);
					return;
				}

				case Option::Settings:
				{
					std::shared_ptr<SceneActivateMessage> sam =
						std::make_shared<SceneActivateMessage>("Settings");
					MessageSystem::getInstance().broadcast(sam);
					return;
				}

				case Option::Quit:
				{
					std::shared_ptr<ExitMessage> em = std::make_shared<ExitMessage>();
					MessageSystem::getInstance().broadcast(em);
					return;
				}
			}
		}

        /*if(bde->getInputName().compare("PanForward") == 0)
            m_TakenControl = true;
        else if(bde->getInputName().compare("PanBackward") == 0)
            m_TakenControl = true;
        else if(bde->getInputName().compare("PanLeft") == 0)
            m_TakenControl = true;
        else if(bde->getInputName().compare("PanRight") == 0)
            m_TakenControl = true;*/
    }
}
