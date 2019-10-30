#include "SelectTrackScene.h"
#include "../../scenes/alphaspace/TestStarfield.h"
#include "../../../engine/comms/MessageSystem.h"
#include "../../../engine/comms/predefined/ButtonDownEvent.h"
#include "../../../engine/comms/predefined/SceneActivateMessage.h"
#include "../../../engine/comms/predefined/ExitMessage.h"
#include "../../../engine/Config.h"


SelectTrackScene::SelectTrackScene(Context* context)
	: Scene(context),
	m_TrackOption(Option::Antares), // Default track selection
	m_Distance(4.5f),
	m_TrackTitle(std::make_shared<MenuBox>(context, getManagers(), glm::vec3(0.f, .9f, 1.f))),
	m_TrackOption1(std::make_shared<MenuBox>(context, getManagers(), glm::vec3(0.f, .6f, 0.f))),
	m_TrackOption2(std::make_shared<MenuBox>(context, getManagers(), glm::vec3(0.f, .2f, 0.f))),
	m_TrackOption3(std::make_shared<MenuBox>(context, getManagers(), glm::vec3(0.f, -.2f, 0.f))),
	m_Cursor(std::make_shared<Cursor>(context, getManagers(), glm::vec3(-1.f, .6f, 0.f)))
{
	m_TrackTitle->setTexture(context, "track.dds");
	m_TrackOption1->setTexture(context, "track1.dds");
	m_TrackOption2->setTexture(context, "track2.dds");
	m_TrackOption3->setTexture(context, "track3.dds");

	addObject(std::make_shared<TestStarfield>(context, getManagers()));
	addObject(m_TrackTitle);
	addObject(m_TrackOption1);
	addObject(m_TrackOption2);
	addObject(m_TrackOption3);
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

SelectTrackScene::~SelectTrackScene()
{
	MessageSystem::getInstance().unsubscribe("ButtonDown", this);
}

void SelectTrackScene::setSize(GLsizei width, GLsizei height)
{
	Scene::setSize(width, height);
}

void SelectTrackScene::step(double dt)
{
	Scene::step(dt);
}

void SelectTrackScene::predraw(Context* context)
{
	Scene::predraw(context);

	// Change texture on selected option
	if (m_TrackOption == Option::Antares)
		m_TrackOption1->setTexture(context, "track1Selected.dds");
	else
		m_TrackOption1->setTexture(context, "track1.dds");

	if (m_TrackOption == Option::Andromeda)
		m_TrackOption2->setTexture(context, "track2Selected.dds");
	else
		m_TrackOption2->setTexture(context, "track2.dds");

	if (m_TrackOption == Option::KesselRun)
		m_TrackOption3->setTexture(context, "track3Selected.dds");
	else
		m_TrackOption3->setTexture(context, "track3.dds");
}

void SelectTrackScene::onMessage(std::shared_ptr<Message> event)
{
	if (event->getType().compare("ButtonDown") == 0)
	{
		std::shared_ptr<ButtonDownEvent> bde =
			std::dynamic_pointer_cast<ButtonDownEvent>(event);

		float limitUp = 0.3f;
		float limitDown = 0.0f;
		glm::vec3 pos = m_Cursor->getPosition();


		// Move the cursor based on keyboard input
		if (bde->getInputName() == "Back")
		{
			std::shared_ptr<SceneActivateMessage> sam =
				std::make_shared<SceneActivateMessage>("ShipSelect");
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
			m_TrackOption = Option::Antares;
		else if (pos.y <= 0.21f && pos.y >= 0.19f)
			m_TrackOption = Option::Andromeda;
		else if (pos.y >= -0.21f && pos.y <= -0.19f)
			m_TrackOption = Option::KesselRun;

		// Take action according to the option selected
		if (bde->getInputName() == "Enter")
		{
			switch (m_TrackOption)
			{
			case Option::Antares:
			{
				std::shared_ptr<SceneActivateMessage> sam =
					std::make_shared<SceneActivateMessage>("Antares");
				MessageSystem::getInstance().broadcast(sam);
				return;
			}

			case Option::Andromeda:
			{
				std::shared_ptr<SceneActivateMessage> sam =
					std::make_shared<SceneActivateMessage>("Andromeda");
				MessageSystem::getInstance().broadcast(sam);
				return;
			}

			case Option::KesselRun:
			{
				std::shared_ptr<SceneActivateMessage> sam =
					std::make_shared<SceneActivateMessage>("KesselRun");
				MessageSystem::getInstance().broadcast(sam);
				return;
			}
			}
		}
	}
}