#include "PauseScene.h"
#include "../../scenes/alphaspace/TestStarfield.h"
#include "../../../engine/comms/MessageSystem.h"
#include "../../../engine/comms/predefined/ButtonDownEvent.h"
#include "../../../engine/comms/predefined/SceneActivateMessage.h"
#include "../../../engine/comms/predefined/ExitMessage.h"
#include "../../../engine/Config.h"

PauseScene::PauseScene(Context* context)
	: Scene(context),
	m_PauseOption(Option::Continue), // Default pause selection
	m_Distance(4.5f),
	m_PauseIcon(std::make_shared<MenuBox>(context, getManagers(), glm::vec3(0.f, .9f, 1.f))),
	m_PauseOption1(std::make_shared<MenuBox>(context, getManagers(), glm::vec3(0.f, .6f, 0.f))),
	m_PauseOption2(std::make_shared<MenuBox>(context, getManagers(), glm::vec3(0.f, .2f, 0.f))),
	m_Cursor(std::make_shared<Cursor>(context, getManagers(), glm::vec3(-1.f, .6f, 0.f)))
{
	m_PauseIcon->setTexture(context, "pause.dds");
	m_PauseOption1->setTexture(context, "continue.dds");
	m_PauseOption2->setTexture(context, "quit.dds");

	addObject(std::make_shared<TestStarfield>(context, getManagers()));
	addObject(m_PauseIcon);
	addObject(m_PauseOption1);
	addObject(m_PauseOption2);
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

PauseScene::~PauseScene()
{
	MessageSystem::getInstance().unsubscribe("ButtonDown", this);
}

void PauseScene::setSize(GLsizei width, GLsizei height)
{
	Scene::setSize(width, height);
}

void PauseScene::step(double dt)
{
	Scene::step(dt);
}

void PauseScene::predraw(Context* context)
{
	Scene::predraw(context);

	// Change texture on selected option
	if (m_PauseOption == Option::Continue)
		m_PauseOption1->setTexture(context, "continueSelected.dds");
	else
		m_PauseOption1->setTexture(context, "continue.dds");

	if (m_PauseOption == Option::Quit)
		m_PauseOption2->setTexture(context, "quitSelected.dds");
	else
		m_PauseOption2->setTexture(context, "quit.dds");
}

void PauseScene::onMessage(std::shared_ptr<Message> event)
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
				std::make_shared<SceneActivateMessage>("MainMenu");
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
			m_PauseOption = Option::Continue;
		else if (pos.y <= 0.21f && pos.y >= 0.19f)
			m_PauseOption = Option::Quit;


		// Take action according to the option selected
		if (bde->getInputName() == "Enter")
		{
			switch (m_PauseOption)
			{
			case Option::Continue:
			{
				std::shared_ptr<SceneActivateMessage> sam =
					std::make_shared<SceneActivateMessage>("Continue");
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
	}
}