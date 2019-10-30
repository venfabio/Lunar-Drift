#include "TestSpacecraft.h"
#include "../../../engine/components/PhysicsComponent.h"
#include "../../../engine/components/PropulsionComponent.h"
#include "../../../engine/components/CollisionComponent.h"
#include "../../../engine/components/HitpointComponent.h"
#include "../../../engine/comms/MessageSystem.h"
#include "../../../engine/comms/predefined/ButtonDownEvent.h"
#include "../../../engine/comms/predefined/ButtonUpEvent.h"
#include "../../../engine/comms/predefined/CollisionMessage.h"
#include "../../../engine/comms/predefined/DeathMessage.h"


TestSpacecraft::TestSpacecraft(
	Context* context,
	std::weak_ptr<MetaManager> managers,
	const glm::vec3& position)
	: SceneObject(context, managers)
{
	setPosition(position);

	MessageSystem::getInstance().subscribe("Collision", this);
	MessageSystem::getInstance().subscribe("Death", this);
	MessageSystem::getInstance().subscribe("ButtonDown", this);
	MessageSystem::getInstance().subscribe("ButtonUp", this);

	std::vector<CollisionSphere> points;
	points.push_back({ 1, glm::vec3(0, 0.2, 0.125), 0.35 });
	points.push_back({ 2, glm::vec3(0, 0.15, -0.3125), 0.25 });
	points.push_back({ 3, glm::vec3(0, 0.15, -0.65), 0.25 });
	points.push_back({ 4, glm::vec3(0.45, 0.15, -0.125), 0.35 });
	points.push_back({ 5, glm::vec3(-0.45, 0.15, -0.125), 0.35 });
	
	setupJets(context, managers);
	addComponent(std::make_shared<PhysicsComponent>(this, managers, 1000.f));
    addComponent(std::make_shared<CollisionComponent>(this, managers, 1.0f, points));
    addComponent(std::make_shared<HitpointComponent>(this, managers, 10));

	m_Model = std::make_shared<Model>(context, "PinkiePie");
	m_Texture = std::make_unique<Texture>(context, "TextureC1.dds");

    //setupOutline(context);
}

void TestSpacecraft::setupJets(Context* context, std::weak_ptr<MetaManager> managers)
{
	JetDescription jetDesc;
	jetDesc.emissionRadius = 0.1f;
	jetDesc.emissionRate = 1000;
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

	addComponent(std::make_shared<PropulsionComponent>(this, managers, context, jets));
}

void TestSpacecraft::setupOutline(Context* context)
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

TestSpacecraft::~TestSpacecraft()
{
	MessageSystem::getInstance().unsubscribe("Collision", this);
    MessageSystem::getInstance().unsubscribe("Death", this);
    MessageSystem::getInstance().unsubscribe("ButtonDown", this);
    MessageSystem::getInstance().unsubscribe("ButtonUp", this);
}


void TestSpacecraft::step(double dt)
{
    SceneObject::step(dt);
}

void TestSpacecraft::predraw(Context* context)
{
    SceneObject::predraw(context);
}

void TestSpacecraft::draw(Context* context, std::shared_ptr<ShaderManager> shaders)
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

void TestSpacecraft::onMessage(std::shared_ptr<Message> event)
{
    const GLfloat throttle = 50.f;

	if (event->getType().compare("Collision") == 0)
	{
		std::shared_ptr<CollisionMessage> dMsg =
			std::static_pointer_cast<CollisionMessage>(event);
	}
    else if(event->getType().compare("Death") == 0)
    {
        std::shared_ptr<DeathMessage> dMsg =
            std::static_pointer_cast<DeathMessage>(event);
        if (dMsg->getDoomed() == this)
            setDying();
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

        if (bde->getInputName().compare("Accelerate") == 0)
        {
            //pc->applyForce(throttle, false, glm::vec3(0.f, 0.f, -1.f), glm::vec3(), true);
			prc->setJetActive(0, true);
        }
        if (bde->getInputName().compare("Brake") == 0)
        {
			//pc->applyForce(throttle, false, glm::vec3(0.f, 0.f, 1.f), glm::vec3(), true);
			prc->setJetActive(1, true);
		}
		if (bde->getInputName().compare("YawLeft") == 0)
		{
			//pc->applyForce(throttle, false, glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 0.f, -2.f), true);
			prc->setJetActive(2, true);
		}
		if (bde->getInputName().compare("YawRight") == 0)
		{
			//pc->applyForce(throttle, false, glm::vec3(-1.f, 0.f, 0.f), glm::vec3(0.f, 0.f, -2.f), true);
			prc->setJetActive(3, true);
		}
		if (bde->getInputName().compare("PitchUp") == 0)
		{
			//pc->applyForce(throttle, false, glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.f, 0.f, -2.f), true);
			prc->setJetActive(4, true);
		}
		if (bde->getInputName().compare("PitchDown") == 0)
		{
			//pc->applyForce(throttle, false, glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, -2.f), true);
			prc->setJetActive(5, true);
		}
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

        if (bue->getInputName().compare("Accelerate") == 0)
        {
            //pc->applyForce(throttle, false, glm::vec3(0.f, 0.f, 1.f), glm::vec3(), true);
			prc->setJetActive(0, false);
        }
        if (bue->getInputName().compare("Brake") == 0)
        {
			//pc->applyForce(throttle, false, glm::vec3(0.f, 0.f, -1.f), glm::vec3(), true);
			prc->setJetActive(1, false);
		}
		if (bue->getInputName().compare("YawLeft") == 0)
		{
			//pc->applyForce(throttle, false, glm::vec3(-1.f, 0.f, 0.f), glm::vec3(0.f, 0.f, -2.f), true);
			prc->setJetActive(2, false);
		}
		if (bue->getInputName().compare("YawRight") == 0)
		{
			//pc->applyForce(throttle, false, glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 0.f, -2.f), true);
			prc->setJetActive(3, false);
		}
		if (bue->getInputName().compare("PitchUp") == 0)
		{
			//pc->applyForce(throttle, false, glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, -2.f), true);
			prc->setJetActive(4, false);
		}
		if (bue->getInputName().compare("PitchDown") == 0)
		{
			//pc->applyForce(throttle, false, glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.f, 0.f, -2.f), true);
			prc->setJetActive(5, false);
		}
    }
}
