#include "RaceScene.h"
#include "../../../engine/comms/MessageSystem.h"
#include "../../../engine/comms/predefined/ButtonDownEvent.h"
#include "../../../engine/comms/predefined/ButtonUpEvent.h"
#include "../../../engine/comms/predefined/FireMissileMessage.h"
#include "../../../engine/comms/predefined/SceneActivateMessage.h"
#include "../../../engine/comms/predefined/ScoreAddMessage.h"
#include "../../../engine/components/PhysicsComponent.h"
#include "../../../engine/input/InputMap.h"
#include "../../../engine/Config.h"
#include "../../../engine/prefabs/Skybox.h"
#include "../../../engine/prefabs/Text.h"
#include "../../../engine/MathHelper.h"
#include "../game/ships/Derpy.h"
#include "../game/ships/PinkiePie.h"
#include "../game/weapons/MissileWeapon.h"
#include <ctime>

RaceScene::RaceScene(
	Context* context, 
	const GameParameters& params,
	const std::string& skyboxDir)
	: Scene(context),
	  m_Player1(nullptr),
	  m_Player2(nullptr),
	  m_Time(0.0),
	  m_Distance(3.5f), m_RotAngle(0.f),
	  m_DistanceP2(3.5f), m_RotAngleP2(0.f),
	  m_ScoreP1(0), m_ScoreP2(0),
	  m_IsMultiplayer(params.raceMode == RaceModeSelection::Multiplayer),
	  m_IsBirdsEyeView(false), m_IsBirdsEyeViewP2(false)
{
	bool isHardMode = params.difficulty == DifficultySelection::Expert ? true : false;

	setNumRenderPasses(params.raceMode == RaceModeSelection::Multiplayer ? 2 : 1);

	addObject(std::make_shared<Skybox>(context, getManagers(), skyboxDir));

	switch (params.ship1)
	{
		case ShipSelection::Derpy:
			m_Player1 = std::make_shared<Derpy>(context, getManagers(), glm::vec3(1.f, 0.f, 1.f), isHardMode, false);
			break;
		case ShipSelection::PinkiePie:
			m_Player1 = std::make_shared<PinkiePie>(context, getManagers(), glm::vec3(1.f, 0.f, 1.f), isHardMode, false);
			break;
	}
	addObject(m_Player1);

	if (m_IsMultiplayer)
	{
		switch (params.ship2)
		{
			case ShipSelection::Derpy:
				m_Player2 = std::make_shared<Derpy>(context, getManagers(), glm::vec3(-1.f, 0.f, 1.f), isHardMode, true);
				break;
			case ShipSelection::PinkiePie:
				m_Player2 = std::make_shared<PinkiePie>(context, getManagers(), glm::vec3(-1.f, 0.f, 1.f), isHardMode, true);
				break;
		}
		addObject(m_Player2);
	}

	addObject(
		"speedReadout",
		std::make_shared<Text>(context, getManagers(), glm::vec2(20.f, 20.f), "SPEEDOUT", true));
	addObject(
		"fuelReadout",
		std::make_shared<Text>(context, getManagers(), glm::vec2(20.f, 60.f), "FUELOUT", true));
	addObject(
		"timeReadout",
		std::make_shared<Text>(context, getManagers(), glm::vec2(700.f, 20.f), "TIMEOUT"));
	addObject(
		"scoreReadout",
		std::make_shared<Text>(context, getManagers(), glm::vec2(700.f, 60.f), "SCOREOUT", true));


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
	m_StandardCamera = camera;

	MessageSystem::getInstance().subscribe("ButtonDown", this);
	MessageSystem::getInstance().subscribe("ButtonUp", this);
	MessageSystem::getInstance().subscribe("FireMissile", this);
	MessageSystem::getInstance().subscribe("AddScore", this);
}

RaceScene::~RaceScene()
{
	MessageSystem::getInstance().unsubscribe("ButtonDown", this);
	MessageSystem::getInstance().unsubscribe("ButtonUp", this);
	MessageSystem::getInstance().unsubscribe("FireMissile", this);
	MessageSystem::getInstance().unsubscribe("AddScore", this);
}


void RaceScene::setSize(GLsizei width, GLsizei height)
{
	GLfloat nearZ = Config::get().getFloat("config", "camera:nearZ");
	GLfloat farZ = Config::get().getFloat("config", "camera:farZ");
	GLfloat fovy = Config::get().getFloat("config", "camera:fov");

    std::shared_ptr<Camera> camera = getActiveCamera().lock();
    camera->setPerspective(width, height, nearZ, farZ, fovy);
}

void RaceScene::step(double dt)
{
	m_Time += dt;

	//const GLfloat speed = 0.03f; // 5.aiman
	Scene::step(dt);

/*  const GLfloat speed = 0.03f;

	Keyboard* kbd = InputMap::getInstance().getKeyboard();
	if (kbd->isKeyDown("PanLeft"))
		m_RotAngle -= speed * static_cast<GLfloat>(dt);
	if (kbd->isKeyDown("PanRight"))
		m_RotAngle += speed * static_cast<GLfloat>(dt);*/
}

void RaceScene::predraw(Context* context)
{
    Scene::predraw(context);

	std::shared_ptr<Text> timeTxt =
		std::static_pointer_cast<Text>(getObject("timeReadout"));
	timeTxt->setText("Time ");
	timeTxt->addNumerical(m_Time);
}

void RaceScene::setRenderPass(Context* context, unsigned int pass)
{
	std::shared_ptr<PhysicsComponent> pc;
	int score;
	double fuel;

	if (pass == 0)			// Setup scene for rendering player 1's screen
	{
		score = m_ScoreP1;
		fuel = m_Player1->getRemainingFuel();

		pc = std::static_pointer_cast<PhysicsComponent>(
			m_Player1->getComponent("physics"));

		if (m_IsBirdsEyeView)
			setActiveCamera(getBirdsEyeCam(context, m_Player1->getPosition()));
		else
		{
			GLfloat x = sin(DEG2RAD(m_RotAngle)) * m_Distance;
			GLfloat z = cos(DEG2RAD(m_RotAngle)) * m_Distance;
			glm::vec4 displacement = glm::vec4(x, 1.f, z, 1.f);
			glm::mat4 modelMatrix;
			m_Player1->getModelMatrix(&modelMatrix);
			displacement = modelMatrix * displacement;
			glm::vec4 shipPos = modelMatrix * glm::vec4(0, 0, 0, 1);

			setActiveCamera(m_StandardCamera);

			getActiveCamera().lock()->lookAt(
				glm::vec3(displacement),
				glm::vec3(shipPos),
				glm::vec3(0.f, 1.f, 0.f));
		}
	}
	else if (pass == 1)		// Setup scene for rendering player 2's screen
	{
		score = m_ScoreP2;
		fuel = m_Player2->getRemainingFuel();

		pc = std::static_pointer_cast<PhysicsComponent>(
			m_Player2->getComponent("physics"));

		if (m_IsBirdsEyeViewP2)
			setActiveCamera(getBirdsEyeCam(context, m_Player2->getPosition()));
		else
		{
			GLfloat x = sin(DEG2RAD(m_RotAngleP2)) * m_DistanceP2;
			GLfloat z = cos(DEG2RAD(m_RotAngleP2)) * m_DistanceP2;
			glm::vec4 displacement = glm::vec4(x, 1.f, z, 1.f);
			glm::mat4 modelMatrix;
			m_Player2->getModelMatrix(&modelMatrix);
			displacement = modelMatrix * displacement;
			glm::vec4 shipPos = modelMatrix * glm::vec4(0, 0, 0, 1);

			setActiveCamera(m_StandardCamera);

			getActiveCamera().lock()->lookAt(
				glm::vec3(displacement),
				glm::vec3(shipPos),
				glm::vec3(0.f, 1.f, 0.f));
		}
	}

	std::shared_ptr<Text> scoreTxt =
		std::static_pointer_cast<Text>(getObject("scoreReadout"));
	scoreTxt->setText("Score ");
	scoreTxt->addNumerical(score);

	std::shared_ptr<Text> speedTxt =
		std::static_pointer_cast<Text>(getObject("speedReadout"));
	speedTxt->setText("Speed ");
	speedTxt->addNumerical(glm::length(pc->getVelocity()));

	std::shared_ptr<Text> fuelTxt =
		std::static_pointer_cast<Text>(getObject("fuelReadout"));
	fuelTxt->setText("Fuel ");
	fuelTxt->addNumerical(fuel);
}

void RaceScene::onMessage(std::shared_ptr<Message> event)
{
    if(event->getType().compare("ButtonDown") == 0)
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
		else if (bde->getInputName() == "PreemptiveEnd")
		{
			std::shared_ptr<SceneActivateMessage> sam =
				std::make_shared<SceneActivateMessage>("GameOver");
			MessageSystem::getInstance().broadcast(sam);
		}
		else if (bde->getInputName() == "BirdsEye")
		{
			m_IsBirdsEyeView = true;
		}
		else if (bde->getInputName() == "BirdsEye2")
		{
			m_IsBirdsEyeViewP2 = true;
		}
	}
	else if (event->getType().compare("ButtonUp") == 0)
	{
		std::shared_ptr<ButtonUpEvent> bue =
			std::static_pointer_cast<ButtonUpEvent>(event);

		if (bue->getInputName() == "BirdsEye")
		{
			m_IsBirdsEyeView = false;
		}
		else if (bue->getInputName() == "BirdsEye2")
		{
			m_IsBirdsEyeViewP2 = false;
		}
	}
	else if (event->getType().compare("FireMissile") == 0)
	{
		const GLfloat missileSpeed = 50.f;

		std::shared_ptr<FireMissileMessage> fmm =
			std::static_pointer_cast<FireMissileMessage>(event);

		if (fmm->isShooterPlayer1())
		{
			if (m_Player2 != nullptr)
				addObject(std::make_shared<MissileWeapon>(getManagers(), m_Player1->getPosition(), m_Player2));
		}
		else
		{
			addObject(std::make_shared<MissileWeapon>(getManagers(), m_Player2->getPosition(), m_Player1));
		}
	}
	else if (event->getType().compare("AddScore") == 0)
	{
		std::shared_ptr<ScoreAddMessage> sam =
			std::static_pointer_cast<ScoreAddMessage>(event);

		if (sam->getPlayer())
		{
			if (-sam->getScoreAdd() > m_ScoreP2)
				m_ScoreP2 = 0;
			else
				m_ScoreP2 += sam->getScoreAdd();
		}
		else
		{
			if (-sam->getScoreAdd() > m_ScoreP1)
				m_ScoreP1 = 0;
			else
				m_ScoreP1 += sam->getScoreAdd();
		}
	}
	
}
