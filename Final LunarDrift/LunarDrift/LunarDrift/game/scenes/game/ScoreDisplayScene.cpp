#include "ScoreDisplayScene.h"
#include "../../scenes/alphaspace/TestStarfield.h"
#include "../../../engine/comms/MessageSystem.h"
#include "../../../engine/comms/predefined/ButtonDownEvent.h"
#include "../../../engine/comms/predefined/SceneActivateMessage.h"
#include "../../../engine/comms/predefined/ExitMessage.h"
#include "../../../engine/Config.h"
#include "../../../engine/prefabs/Text.h"
#include "../../../engine/comms/predefined/ScoreAddMessage.h"
#include "../../../engine/audio/AudioPlayer.h"

ScoreDisplayScene::ScoreDisplayScene(Context* context, int scoreP1, int  scoreP2)
	: Scene(context),
	m_Distance(4.5f),
	m_ScoreP1(scoreP1),
	m_ScoreP2(scoreP2),
	m_Width(0), 
	m_Height(0)
{
	AudioPlayer::getInstance().setActiveMusic("A Message To Humanity.mp3");

	addObject(
		"scoreReadout",
		std::make_shared<Text>(context, getManagers(), glm::vec2( 0.0f, 0.0f), "SCOREOUT", true));

	addObject(
		"scoreReadout2",
		std::make_shared<Text>(context, getManagers(), glm::vec2(0.0f, 0.0f), "SCOREOUT2", true));

	addObject(std::make_shared<TestStarfield>(context, getManagers()));

	// Camera
	GLfloat nearZ = Config::get().getFloat("config", "camera:nearZ");
	GLfloat farZ = Config::get().getFloat("config", "camera:farZ");
	GLfloat fovy = Config::get().getFloat("config", "camera:fov");

	GLsizei width, height;
	context->getDimensions(&width, &height);
	m_Width = width;
	m_Height = height;
	std::shared_ptr<Camera> camera = std::make_shared<Camera>(
		width, height, nearZ, farZ, fovy);
	camera->lookAt(
		glm::vec3(0.f, 0.f, m_Distance),
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(0.f, 1.f, 0.f));
	setActiveCamera(camera);


	m_ScoreTitle = std::make_shared<MenuBox2D>(context, getManagers(), glm::vec2(20, 20), glm::vec2(150, 150));
	m_ScoreTableHeaders = std::make_shared<MenuBox2D>(context, getManagers(), glm::vec2(50, 20), glm::vec2(20, 50));
	m_ScoreTableRow = std::make_shared<MenuBox2D>(context, getManagers(), glm::vec2(20, 50), glm::vec2(20, 20));
	m_Replay = std::make_shared<MenuBox2D>(context, getManagers(), glm::vec2(200, 400), glm::vec2(400, 100));
	//m_Cursor = std::make_shared<Cursor>(context, getManagers(), glm::vec2(50, 50), glm::vec2(20, 20));

	m_ScoreTitle->setTexture(context, "scoreTitle.dds");
	m_ScoreTableHeaders->setTexture(context, "headers.dds");
	m_ScoreTableRow->setTexture(context, "row.dds");
	m_Replay->setTexture(context, "replay2D.dds");

	addObject(m_ScoreTitle);
	addObject(m_ScoreTableHeaders);
	addObject(m_ScoreTableRow);
	addObject(m_Replay);
	//addObject(m_Cursor);

	MessageSystem::getInstance().subscribe("ButtonDown", this);
}

ScoreDisplayScene::~ScoreDisplayScene()
{
	MessageSystem::getInstance().unsubscribe("ButtonDown", this);
}

void ScoreDisplayScene::setSize(GLsizei width, GLsizei height)
{
	m_Width = width;
	m_Height = height;
	Scene::setSize(width, height);
}

void ScoreDisplayScene::step(double dt)
{
	Scene::step(dt);
}

void ScoreDisplayScene::predraw(Context* context)
{
	Scene::predraw(context);


	std::shared_ptr<Text> scoreTxt =
		std::static_pointer_cast<Text>(getObject("scoreReadout"));
	scoreTxt->setText("Player 1  ");
	scoreTxt->addNumerical(m_ScoreP1);
	scoreTxt->setPosition(glm::vec2(m_Width*0.25f, m_Height * 0.135f));

	std::shared_ptr<Text> scoreTxt2 =
		std::static_pointer_cast<Text>(getObject("scoreReadout2"));
	scoreTxt2->setText("Player 2  ");
	scoreTxt2->addNumerical(m_ScoreP2);
	scoreTxt2->setPosition(glm::vec2(m_Width*0.25f, m_Height * 0.185f));
}

void ScoreDisplayScene::onMessage(std::shared_ptr<Message> event)
{
	if (event->getType().compare("ButtonDown") == 0)
	{
		std::shared_ptr<ButtonDownEvent> bde =
			std::dynamic_pointer_cast<ButtonDownEvent>(event);

		if (bde->getInputName() == "Back")
		{
			std::shared_ptr<SceneActivateMessage> sam =
				std::make_shared<SceneActivateMessage>("MainMenu");
			MessageSystem::getInstance().broadcast(sam);
		}
		else if (bde->getInputName() == "Replay")
		{
			std::shared_ptr<SceneActivateMessage> sam =
				std::make_shared<SceneActivateMessage>("Replay");
			MessageSystem::getInstance().broadcast(sam);
		}
	}
	

}