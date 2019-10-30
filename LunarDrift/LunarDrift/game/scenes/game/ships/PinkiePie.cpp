#include "PinkiePie.h"
#include "../../../../engine/components/PhysicsComponent.h"
#include "../../../../engine/components/PropulsionComponent.h"
#include "../../../../engine/components/CollisionComponent.h"
#include "../../../../engine/components/HitpointComponent.h"
#include "../../../../engine/comms/MessageSystem.h"
#include "../../../../engine/comms/predefined/ButtonDownEvent.h"
#include "../../../../engine/comms/predefined/ButtonUpEvent.h"
#include "../../../../engine/comms/predefined/CollisionMessage.h"
#include "../../../../engine/comms/predefined/DeathMessage.h"

const double startingFuel = 40.0;

PinkiePie::PinkiePie(
	Context* context,
	std::weak_ptr<MetaManager> managers,
	const glm::vec3& position,
	bool isHardMode,
	bool isPlayer2)
	: Ship(context, managers, position, 50, isHardMode, isPlayer2)
{
	setPosition(position);

	addFuel(isHardMode ? startingFuel * 0.5 : startingFuel);

	MessageSystem::getInstance().subscribe("Collision", this);
	MessageSystem::getInstance().subscribe("Death", this);
	MessageSystem::getInstance().subscribe("ButtonDown", this);
	MessageSystem::getInstance().subscribe("ButtonUp", this);

	std::vector<CollisionSphere> points;
	points.push_back({ 1, glm::vec3(0, -0.05, 0.2), 0.15 });
	points.push_back({ 2, glm::vec3(0, -0.05, -0.1), 0.15 });
	points.push_back({ 3, glm::vec3(0.2, -0.05, -0.25), 0.15 });
	points.push_back({ 4, glm::vec3(-0.2, -0.05, -0.25), 0.15 });

	setupJets(context, managers);
	addComponent(std::make_shared<PhysicsComponent>(this, managers, 20.f));
	addComponent(std::make_shared<CollisionComponent>(this, managers, 0.9f, points));
	//addComponent(std::make_shared<HitpointComponent>(this, managers, 10));

	m_Model = std::make_shared<Model>(context, "PinkiePie");
	m_Texture = std::make_unique<Texture>(context, "TextureC1.dds");

	//setupOutline(context);
}

void PinkiePie::setupJets(Context* context, std::weak_ptr<MetaManager> managers)
{
	JetDescription jetDesc;
	jetDesc.emissionRadius = 0.1f;
#ifdef DEBUG
	jetDesc.emissionRate = 1000;
#else
	jetDesc.emissionRate = 3000;
#endif
	jetDesc.speed = 1.f;
	jetDesc.speedVariance = 0.2f;
	jetDesc.lifeTime = 1.0f;
	jetDesc.lifeTimeVariance = 0.5f;
	jetDesc.startColor = glm::vec4(1.f, 1.f, 0.f, 1.f);
	jetDesc.decay = glm::vec4(0.f, 2.f, 0.f, 0.01f);
	jetDesc.decayTime = 2.f;

	std::vector<PropulsionJet> jets;
	PropulsionJet jet0;
	jet0.id = 0;
	jet0.offset = glm::vec3();
	jet0.direction = glm::vec3(0.f, 0.f, -1.f);
	jet0.gfxDesc = jetDesc;
	jet0.forceMagnitude = 50.f;
	jet0.active = false;
	jets.push_back(jet0);
	PropulsionJet jet1;
	jet1.id = 1;
	jet1.offset = glm::vec3();
	jet1.direction = glm::vec3(0.f, 0.f, 1.f);
	jet1.gfxDesc = jetDesc;
	jet1.forceMagnitude = 50.f;
	jet1.active = false;
	jets.push_back(jet1);
	PropulsionJet jet2;
	jet2.id = 2;
	jet2.offset = glm::vec3(0.f, 0.f, -0.5f);
	jet2.direction = glm::vec3(1.f, 0.f, 0.f);
	jet2.gfxDesc = jetDesc;
	jet2.forceMagnitude = 50.f;
	jet2.active = false;
	jets.push_back(jet2);
	PropulsionJet jet3;
	jet3.id = 3;
	jet3.offset = glm::vec3(0.f, 0.f, -0.5f);
	jet3.direction = glm::vec3(-1.f, 0.f, 0.f);
	jet3.gfxDesc = jetDesc;
	jet3.forceMagnitude = 50.f;
	jet3.active = false;
	jets.push_back(jet3);
	PropulsionJet jet4;
	jet4.id = 4;
	jet4.offset = glm::vec3(0.f, 0.f, -0.5f);
	jet4.direction = glm::vec3(0.f, 1.f, 0.f);
	jet4.gfxDesc = jetDesc;
	jet4.forceMagnitude = 50.f;
	jet4.active = false;
	jets.push_back(jet4);
	PropulsionJet jet5;
	jet5.id = 5;
	jet5.offset = glm::vec3(0.f, 0.f, -0.5f);
	jet5.direction = glm::vec3(0.f, -1.f, 0.f);
	jet5.gfxDesc = jetDesc;
	jet5.forceMagnitude = 50.f;
	jet5.active = false;
	jets.push_back(jet5);
	PropulsionJet jet6;
	jet6.id = 6;
	jet6.offset = glm::vec3();
	jet6.direction = glm::vec3(1.f, 0.f, 0.f);
	jet6.gfxDesc = jetDesc;
	jet6.forceMagnitude = 50.f;
	jet6.active = false;
	jets.push_back(jet6);
	PropulsionJet jet7;
	jet7.id = 7;
	jet7.offset = glm::vec3();
	jet7.direction = glm::vec3(-1.f, 0.f, 0.f);
	jet7.gfxDesc = jetDesc;
	jet7.forceMagnitude = 50.f;
	jet7.active = false;
	jets.push_back(jet7);
	PropulsionJet jet8;
	jet8.id = 8;
	jet8.offset = glm::vec3();
	jet8.direction = glm::vec3(0.f, 1.f, 0.f);
	jet8.gfxDesc = jetDesc;
	jet8.forceMagnitude = 50.f;
	jet8.active = false;
	jets.push_back(jet8);
	PropulsionJet jet9;
	jet9.id = 9;
	jet9.offset = glm::vec3();
	jet9.direction = glm::vec3(0.f, -1.f, 0.f);
	jet9.gfxDesc = jetDesc;
	jet9.forceMagnitude = 50.f;
	jet9.active = false;
	jets.push_back(jet9);

	addComponent(std::make_shared<PropulsionComponent>(this, managers, context, jets));
}

void PinkiePie::setupOutline(Context* context)
{
	std::unique_ptr<Model> model = std::make_unique<Model>(context, "test");

	std::vector<Model::Vertex> vertices;
	std::shared_ptr<VertexBufferObject<Model::Vertex>> vbo =
		std::static_pointer_cast<VertexBufferObject<Model::Vertex>>(
		model->getVertexArrayObject().lock()->getVertexBuffer());
	vbo->getVertices(&vertices);

	for (Model::Vertex& vertex : vertices)
		vertex.color = glm::vec4(1.f, 1.f, 1.f, 1.f);

	//m_Outline = std::make_shared<Model>(context, vertices);
	//m_Outline->setPrimitiveMode(PrimitiveMode::Lines);
}

PinkiePie::~PinkiePie()
{
	MessageSystem::getInstance().unsubscribe("Collision", this);
	MessageSystem::getInstance().unsubscribe("Death", this);
	MessageSystem::getInstance().unsubscribe("ButtonDown", this);
	MessageSystem::getInstance().unsubscribe("ButtonUp", this);
}


void PinkiePie::step(double dt)
{
	Ship::step(dt);


	std::shared_ptr<PropulsionComponent> prc =
		std::static_pointer_cast<PropulsionComponent>(
			getComponent("propulsion"));
	for (unsigned int i = 0; i <= 9; ++i)
	{
		if (prc->isJetActive(i))
			addFuel(-dt);
	}

	if (getRemainingFuel() <= 0.0)
	{
		for (unsigned int i = 0; i <= 9; ++i)
			prc->setJetActive(i, false);
	}

}

void PinkiePie::predraw(Context* context)
{
	Ship::predraw(context);
}

void PinkiePie::draw(Context* context, std::shared_ptr<ShaderManager> shaders)
{
	// Draw main body of ship
	std::shared_ptr<ShaderProgram> shader = shaders->get("texbox");
	updateModelMatrixState(context, shader);

	m_Texture->bind(context);
	m_Model->draw(context, shader);

	// Draw outline
	//shader = shaders->get("ship");
	//updateModelMatrixState(context, shader);
	//m_Outline->draw(context, shader);
}

void PinkiePie::onMessage(std::shared_ptr<Message> event)
{
	const GLfloat throttle = 50.f;
	Ship::onMessage(event);

	const char* accelerateP1 = "Accelerate";
	const char* accelerateP2 = "Accelerate2";
	const char* accelerate = isPlayer2Controlled() ? accelerateP2 : accelerateP1;
	const char* brakeP1 = "Brake";
	const char* brakeP2 = "Brake2";
	const char* brake = isPlayer2Controlled() ? brakeP2 : brakeP1;
	const char* yawLeftP1 = "YawLeft";
	const char* yawLeftP2 = "YawLeft2";
	const char* yawLeft = isPlayer2Controlled() ? yawLeftP2 : yawLeftP1;
	const char* yawRightP1 = "YawRight";
	const char* yawRightP2 = "YawRight2";
	const char* yawRight = isPlayer2Controlled() ? yawRightP2 : yawRightP1;
	const char* pitchUpP1 = "PitchUp";
	const char* pitchUpP2 = "PitchUp2";
	const char* pitchUp = isPlayer2Controlled() ? pitchUpP2 : pitchUpP1;
	const char* pitchDownP1 = "PitchDown";
	const char* pitchDownP2 = "PitchDown2";
	const char* pitchDown = isPlayer2Controlled() ? pitchDownP2 : pitchDownP1;
	const char* panRightP1 = "PanRight";
	const char* panRightP2 = "PanRight2";
	const char* panRight = isPlayer2Controlled() ? panRightP2 : panRightP1;
	const char* panLeftP1 = "PanLeft";
	const char* panLeftP2 = "PanLeft2";
	const char* panLeft = isPlayer2Controlled() ? panLeftP2 : panLeftP1;
	const char* panUpP1 = "PanUp";
	const char* panUpP2 = "PanUp2";
	const char* panUp = isPlayer2Controlled() ? panUpP2 : panUpP1;
	const char* panDownP1 = "PanDown";
	const char* panDownP2 = "PanDown2";
	const char* panDown = isPlayer2Controlled() ? panDownP2 : panDownP1;

	if (event->getType().compare("Collision") == 0)
	{
		std::shared_ptr<CollisionMessage> dMsg =
			std::static_pointer_cast<CollisionMessage>(event);
	}
	else if (event->getType().compare("ButtonDown") == 0)
	{
		std::shared_ptr<ButtonDownEvent> bde =
			std::static_pointer_cast<ButtonDownEvent>(event);

		std::shared_ptr<PhysicsComponent> pc =
			std::static_pointer_cast<PhysicsComponent>(
				getComponent("physics"));

		std::shared_ptr<PropulsionComponent> prc =
			std::static_pointer_cast<PropulsionComponent>(
				getComponent("propulsion"));

		if (getRemainingFuel() <= 0.0)
			return;

		if (bde->getInputName().compare(accelerate) == 0)
			prc->setJetActive(0, true);
		if (bde->getInputName().compare(brake) == 0)
			prc->setJetActive(1, true);
		if (bde->getInputName().compare(yawLeft) == 0)
			prc->setJetActive(2, true);
		if (bde->getInputName().compare(yawRight) == 0)
			prc->setJetActive(3, true);
		if (bde->getInputName().compare(pitchUp) == 0)
			prc->setJetActive(5, true);
		if (bde->getInputName().compare(pitchDown) == 0)
			prc->setJetActive(4, true);
		if (bde->getInputName().compare(panRight) == 0)
			prc->setJetActive(6, true);
		if (bde->getInputName().compare(panLeft) == 0)
			prc->setJetActive(7, true);
		if (bde->getInputName().compare(panUp) == 0)
			prc->setJetActive(8, true);
		if (bde->getInputName().compare(panDown) == 0)
			prc->setJetActive(9, true);
	}
	else if (event->getType().compare("ButtonUp") == 0)
	{
		std::shared_ptr<ButtonUpEvent> bue =
			std::static_pointer_cast<ButtonUpEvent>(event);

		std::shared_ptr<PhysicsComponent> pc =
			std::static_pointer_cast<PhysicsComponent>(
				getComponent("physics"));

		std::shared_ptr<PropulsionComponent> prc =
			std::static_pointer_cast<PropulsionComponent>(
				getComponent("propulsion"));

		if (bue->getInputName().compare(accelerate) == 0)
			prc->setJetActive(0, false);
		if (bue->getInputName().compare(brake) == 0)
			prc->setJetActive(1, false);
		if (bue->getInputName().compare(yawLeft) == 0)
			prc->setJetActive(2, false);
		if (bue->getInputName().compare(yawRight) == 0)
			prc->setJetActive(3, false);
		if (bue->getInputName().compare(pitchUp) == 0)
			prc->setJetActive(5, false);
		if (bue->getInputName().compare(pitchDown) == 0)
			prc->setJetActive(4, false);
		if (bue->getInputName().compare(panRight) == 0)
			prc->setJetActive(6, false);
		if (bue->getInputName().compare(panLeft) == 0)
			prc->setJetActive(7, false);
		if (bue->getInputName().compare(panUp) == 0)
			prc->setJetActive(8, false);
		if (bue->getInputName().compare(panDown) == 0)
			prc->setJetActive(9, false);
	}
}
