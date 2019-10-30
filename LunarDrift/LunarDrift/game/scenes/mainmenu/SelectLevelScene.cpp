#include "SelectLevelScene.h"
#include "../../scenes/alphaspace/TestStarfield.h"
#include "../../../engine/comms/MessageSystem.h"
#include "../../../engine/comms/predefined/ButtonDownEvent.h"
#include "../../../engine/comms/predefined/SceneActivateMessage.h"
#include "../../../engine/comms/predefined/ExitMessage.h"
#include "../../../engine/Config.h"

SelectLevelScene::SelectLevelScene(Context* context)
	: Scene(context),
	m_LevelOption(Option::Easy), // Default game level selection
	m_Distance(4.5f),
	m_LevelTitle(std::make_shared<MenuBox>(context, getManagers(), glm::vec3(0.f, .9f, 1.f))),
	m_LevelOption1(std::make_shared<MenuBox>(context, getManagers(), glm::vec3(0.f, .6f, 0.f))),
	m_LevelOption2(std::make_shared<MenuBox>(context, getManagers(), glm::vec3(0.f, .2f, 0.f))),
	m_Cursor(std::make_shared<Cursor>(context, getManagers(), glm::vec3(-1.f, .6f, 0.f)))
{
	m_LevelTitle->setTexture(context, "level.dds");
	m_LevelOption1->setTexture(context, "easy.dds");
	m_LevelOption2->setTexture(context, "expert.dds");

	addObject(std::make_shared<TestStarfield>(context, getManagers()));
	addObject(m_LevelTitle);
	addObject(m_LevelOption1);
	addObject(m_LevelOption2);
	addObject(m_Cursor);

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

SelectLevelScene::~SelectLevelScene()
{
	MessageSystem::getInstance().unsubscribe("ButtonDown", this);
}

void SelectLevelScene::setSize(GLsizei width, GLsizei height)
{
	Scene::setSize(width, height);
}

void SelectLevelScene::step(double dt)
{
	Scene::step(dt);
}

void SelectLevelScene::predraw(Context* context)
{
	Scene::predraw(context);

	// Change texture on selected option
	if (m_LevelOption == Option::Easy)
		m_LevelOption1->setTexture(context, "easySelected.dds");
	else
		m_LevelOption1->setTexture(context, "easy.dds");

	if (m_LevelOption == Option::Expert)
		m_LevelOption2->setTexture(context, "expertSelected.dds");
	else
		m_LevelOption2->setTexture(context, "expert.dds");
}

void SelectLevelScene::onMessage(std::shared_ptr<Message> event)
{
	if (event->getType().compare("ButtonDown") == 0)
	{
		std::shared_ptr<ButtonDownEvent> bde =
			std::dynamic_pointer_cast<ButtonDownEvent>(event);

		float limitUp = 0.3f;
		float limitDown = 0.3f;
		glm::vec3 pos = m_Cursor->getPosition();


		// Move the cursor based on keyboard input
		if (bde->getInputName() == "Back")
		{
			std::shared_ptr<SceneActivateMessage> sam =
				std::make_shared<SceneActivateMessage>("TrackSelect");
			MessageSystem::getInstance().broadcast(sam);
		}
		else if ((bde->getInputName() == "Down") && (pos.y >= limitDown))
		{
			pos.y -= 0.4f;
			m_Cursor->setPosition(pos);
		}
		else if ((bde->getInputName() == "Up") && (pos.y <= limitUp))
		{
			pos.y += 0.4f;
			m_Cursor->setPosition(pos);
		}

		// Set Pause option based on the position of the cursor
		if (pos.y == 0.6f)
			m_LevelOption = Option::Easy;
		else if (pos.y <= 0.21f && pos.y >= 0.19f)
			m_LevelOption = Option::Expert;


		// Take action according to the option selected
		if (bde->getInputName() == "Enter")
		{
			switch (m_LevelOption)
			{
			case Option::Easy:
			{
				std::shared_ptr<SceneActivateMessage> sam =
					std::make_shared<SceneActivateMessage>("Easy");
				MessageSystem::getInstance().broadcast(sam);
				return;
			}

			case Option::Expert:
			{
				std::shared_ptr<SceneActivateMessage> sam =
					std::make_shared<SceneActivateMessage>("Expert");
				MessageSystem::getInstance().broadcast(sam);
				return;
			}
			}
		}
	}
}