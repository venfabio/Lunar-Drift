#include "SelectShipScene.h"
#include "../../scenes/alphaspace/TestStarfield.h"
#include "../../../engine/comms/MessageSystem.h"
#include "../../../engine/comms/predefined/ButtonDownEvent.h"
#include "../../../engine/comms/predefined/SceneActivateMessage.h"
#include "../../../engine/comms/predefined/ExitMessage.h"
#include "../../../engine/input/InputMap.h"
#include "../../../engine/Config.h"

SelectShipScene::SelectShipScene(Context* context, bool isPlayer2)
	: Scene(context),
	m_MenuTitle(std::make_shared<MenuBox>(context, getManagers(), glm::vec3(0.f, .9f, 1.f))),
	m_DerpyPreview(std::make_shared<MenuBoxAlt>(context, getManagers(), glm::vec3(0.0f, 0.4f, 0.f))),
	m_PinkiePiePreview(std::make_shared<MenuBoxAlt>(context, getManagers(), glm::vec3(0.0f, -0.8f, 0.f))),
	m_Cursor(std::make_shared<Cursor>(context, getManagers(), glm::vec3(-1.0f, 0.4f, 0.f))),
	m_Distance(4.5f),
	m_IsPlayer2(isPlayer2)
{
	m_MenuTitle->setTexture(context, "SelectShip.dds");
	m_DerpyPreview->setTexture(context, "DerpyPreview.dds");
	m_PinkiePiePreview->setTexture(context, "PinkiePiePreview.dds");

	addObject(std::make_shared<TestStarfield>(context, getManagers()));
	addObject(m_MenuTitle);
	addObject(m_DerpyPreview);
	addObject(m_PinkiePiePreview);
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

SelectShipScene::~SelectShipScene()
{
	MessageSystem::getInstance().unsubscribe("ButtonDown", this);
}

void SelectShipScene::setSize(GLsizei width, GLsizei height)
{
	Scene::setSize(width, height);
}

void SelectShipScene::step(double dt)
{
	Scene::step(dt);
}

void SelectShipScene::predraw(Context* context)
{
	Scene::predraw(context);
}

void SelectShipScene::onMessage(std::shared_ptr<Message> event)
{
	if (event->getType().compare("ButtonDown") == 0)
	{
		std::shared_ptr<ButtonDownEvent> bde =
			std::dynamic_pointer_cast<ButtonDownEvent>(event);

		glm::vec3 pos = m_Cursor->getPosition();

		// Move the cursor based on keyboard input

		if (bde->getInputName() == "Back")
		{
			std::shared_ptr<SceneActivateMessage> sam =
				std::make_shared<SceneActivateMessage>("MainMenu");
			MessageSystem::getInstance().broadcast(sam);
		} 
		else if (bde->getInputName() == "Up")
		{
			pos.y = 0.4f;
			m_Cursor->setPosition(pos);
		}
		else if (bde->getInputName() == "Down")
		{
			pos.y = -0.8f;
			m_Cursor->setPosition(pos);
		}

		if (pos.y > 0.3f)
			m_SelectedShip = ShipSelection::Derpy;
		else if (pos.y < -0.7)
			m_SelectedShip = ShipSelection::PinkiePie;

		if (bde->getInputName() == "Enter")
		{
			switch (m_SelectedShip)
			{
				case ShipSelection::Derpy:
				{
					std::shared_ptr<SceneActivateMessage> sam =
						std::make_shared<SceneActivateMessage>("Derpy");
					MessageSystem::getInstance().broadcast(sam);
					return;
				}

				case ShipSelection::PinkiePie:
				{
					std::shared_ptr<SceneActivateMessage> sam =
						std::make_shared<SceneActivateMessage>("PinkiePie");
					MessageSystem::getInstance().broadcast(sam);
					return;
				}
			}
		}
	}
}