#include "GameOverScene.h"
#include "../../scenes/alphaspace/TestStarfield.h"
#include "../../../engine/comms/MessageSystem.h"
#include "../../../engine/comms/predefined/ButtonDownEvent.h"
#include "../../../engine/comms/predefined/SceneActivateMessage.h"
#include "../../../engine/comms/predefined/ExitMessage.h"
#include "../../../engine/audio/AudioPlayer.h"
#include "../../../engine/Config.h"


GameOverScene::GameOverScene(Context* context)
	: Scene(context),
	m_Distance(4.5f),
	m_GameOverTitle(std::make_shared<MenuBox>(context, getManagers(), glm::vec3(0.f, .9f, 1.f))),
	m_DerpySmoking(std::make_shared<MenuBoxAlt>(context, getManagers(), glm::vec3(0.0f, 0.4f, 0.f))),
	m_Replay(std::make_shared<MenuBox>(context, getManagers(), glm::vec3(0.0f, -0.9f, 0.f)))
{
	AudioPlayer::getInstance().setActiveMusic("A Message To Humanity.mp3");

	m_GameOverTitle->setTexture(context, "gameOver.dds");
	m_DerpySmoking->setTexture(context, "DerpySmoking.dds");
	m_Replay->setTexture(context, "replay.dds");

	addObject(std::make_shared<TestStarfield>(context, getManagers()));
	addObject(m_GameOverTitle);
	addObject(m_DerpySmoking);
	addObject(m_Replay);

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

GameOverScene::~GameOverScene()
{
	MessageSystem::getInstance().unsubscribe("ButtonDown", this);
}

void GameOverScene::setSize(GLsizei width, GLsizei height)
{
	Scene::setSize(width, height);
}

void GameOverScene::step(double dt)
{
	Scene::step(dt);
}

void GameOverScene::predraw(Context* context)
{
	Scene::predraw(context);
}

void GameOverScene::onMessage(std::shared_ptr<Message> event)
{
	if (event->getType().compare("ButtonDown") == 0)
	{
		std::shared_ptr<ButtonDownEvent> bde =
			std::dynamic_pointer_cast<ButtonDownEvent>(event);

		if (bde->getInputName() == "Replay")
		{
			std::shared_ptr<SceneActivateMessage> sam =
				std::make_shared<SceneActivateMessage>("Replay");
			MessageSystem::getInstance().broadcast(sam);
		}
		else if (bde->getInputName() == "Enter")
		{
			std::shared_ptr<SceneActivateMessage> sam =
				std::make_shared<SceneActivateMessage>("MainMenu");
			MessageSystem::getInstance().broadcast(sam);
		}
	}
}