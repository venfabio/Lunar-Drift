#include "TestScene.h"
#include "TestTriangle.h"
#include "TestStarfield.h"
#include "TestTexturedBox.h"
#include "../game/ships/Derpy.h"
#include "../game/ships/PinkiePie.h"
#include "../../../engine/prefabs/Skybox.h"
#include "../../../engine/prefabs/Text.h"
#include "../../../engine/comms/MessageSystem.h"
#include "../../../engine/comms/predefined/ButtonDownEvent.h"
#include "../../../engine/comms/predefined/SceneActivateMessage.h"
#include "../../../engine/input/InputMap.h"
#include "../../../engine/components/PhysicsComponent.h"
#include "../../../engine/Config.h"
#include "../../../engine/MathHelper.h"
#include "../game/powerups/Laser.h"

TestScene::TestScene(Context* context, const GameParameters& params)
    : Scene(context),
      m_Distance(3.5f),
      m_RotAngle(0.f),
	  m_PlayerShip(nullptr)
{
	setNumRenderPasses(params.raceMode == RaceModeSelection::Multiplayer ? 2 : 1);

	addObject(std::make_shared<Skybox>(context, getManagers(), "testSkybox"));

	switch (params.ship1)
	{
		case ShipSelection::Derpy:
		{
			m_PlayerShip = std::make_shared<Derpy>(context, getManagers(), glm::vec3(1.f, 0.f, 1.f), false);
			break;
		}
		case ShipSelection::PinkiePie:
		{
			m_PlayerShip = std::make_shared<PinkiePie>(context, getManagers(), glm::vec3(1.f, 0.f, 1.f), false);
			break;
		}
	}

    addObject(std::make_shared<TestTriangle>(context, getManagers()));
    addObject(std::make_shared<TestStarfield>(context, getManagers()));
    addObject(std::make_shared<TestTexturedBox>(context, getManagers(), glm::vec3(3.f, 0.f, 6.f)));
	/*
	addObject(std::make_shared<HoopGreen>(context, getManagers(), glm::vec3(3.f, 0.f, -3.f)));

	addObject(std::make_shared<HoopYellow>(context, getManagers(), glm::vec3(18.f, 3.f, -3.f)));
	addObject(std::make_shared<HoopGreen>(context, getManagers(), glm::vec3(26.f, 5.f, -4.f)));
	addObject(std::make_shared<HoopBlue>(context, getManagers(), glm::vec3(33.f, 7.f, -6.f)));*/

	

    addObject(m_PlayerShip);

    addObject(
        "speedReadout", 
        std::make_shared<Text>(context, getManagers(), glm::vec2(20.f, 20.f), "SPEEDOUT"));
    addObject(
        "accelerationReadout",
        std::make_shared<Text>(context, getManagers(), glm::vec2(20.f, 40.f)));


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

TestScene::~TestScene()
{
    MessageSystem::getInstance().unsubscribe("ButtonDown", this);
}


void TestScene::setSize(GLsizei width, GLsizei height)
{
	GLfloat nearZ = Config::get().getFloat("config", "camera:nearZ");
	GLfloat farZ = Config::get().getFloat("config", "camera:farZ");
	GLfloat fovy = Config::get().getFloat("config", "camera:fov");

    std::shared_ptr<Camera> camera = getActiveCamera().lock();
    camera->setPerspective(width, height, nearZ, farZ, fovy);
}

void TestScene::step(double dt)
{
    Scene::step(dt);

    const GLfloat speed = 0.03f;

    Keyboard* kbd = InputMap::getInstance().getKeyboard();
    if(kbd->isKeyDown("PanLeft"))
		m_RotAngle -= speed * static_cast<GLfloat>(dt);
    if(kbd->isKeyDown("PanRight"))
        m_RotAngle += speed * static_cast<GLfloat>(dt);

    GLfloat x = sin(DEG2RAD(m_RotAngle)) * m_Distance;
    GLfloat z = cos(DEG2RAD(m_RotAngle)) * m_Distance;
    glm::vec4 displacement = glm::vec4(x, 1.f, z, 1.f);
    glm::mat4 modelMatrix;
    m_PlayerShip->getModelMatrix(&modelMatrix);
    displacement = modelMatrix * displacement;
	glm::vec4 shipPos = modelMatrix * glm::vec4(0, 0, 0, 1);

    getActiveCamera().lock()->lookAt(
        glm::vec3(displacement),
        glm::vec3(shipPos),
        glm::vec3(0.f, 1.f, 0.f));
}

void TestScene::predraw(Context* context)
{
    Scene::predraw(context);


    std::shared_ptr<PhysicsComponent> pc =
        std::static_pointer_cast<PhysicsComponent>(
            m_PlayerShip->getComponent("physics"));

    std::shared_ptr<Text> speedTxt =
        std::static_pointer_cast<Text>(getObject("speedReadout"));
    speedTxt->setText("Speed ");
    speedTxt->addNumerical(glm::length(pc->getVelocity()));

    std::shared_ptr<Text> accelerationTxt =
        std::static_pointer_cast<Text>(getObject("accelerationReadout"));
    accelerationTxt->setText("Acceleration ");
    accelerationTxt->addNumerical(glm::length(pc->getAcceleration()));
}

void TestScene::onMessage(std::shared_ptr<Message> event)
{
	if (event->getType().compare("ButtonDown") == 0)
	{
		std::shared_ptr<ButtonDownEvent> bde =
			std::static_pointer_cast<ButtonDownEvent>(event);

		if (bde->getInputName() == "Back")
		{
			std::shared_ptr<SceneActivateMessage> sam =
				std::make_shared<SceneActivateMessage>("MainMenu");
			MessageSystem::getInstance().broadcast(sam);
		}
		else if (bde->getInputName() == "Pause")
		{
			std::shared_ptr<SceneActivateMessage> sam =
				std::make_shared<SceneActivateMessage>("Pause");
			MessageSystem::getInstance().broadcast(sam);
		}
    }

}
