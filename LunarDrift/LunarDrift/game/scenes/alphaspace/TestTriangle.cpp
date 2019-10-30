#include "TestTriangle.h"
#include "../../../engine/components/LifeTimeComponent.h"
#include "../../../engine/components/ExplodableComponent.h"
#include "../../../engine/comms/MessageSystem.h"
#include "../../../engine/comms/predefined/DeathMessage.h"

TestTriangle::TestTriangle(
	Context* context, 
	std::weak_ptr<MetaManager> managers) 
	: SceneObject(context, managers)
{
    MessageSystem::getInstance().subscribe("Death", this);

    std::unique_ptr<EXPLOSION_DESCRIPTION> desc = 
        std::make_unique<EXPLOSION_DESCRIPTION>();
    desc->offset = glm::vec3(0.f, 0.f, 0.f);
    desc->numParticles = 5000;
    desc->originRadius = 1.f;
    desc->speed = 10.f;
    desc->speedVariance = 5.f;
    desc->lifeTime = 0.1f;
    desc->lifeTimeVariance = 3.5f;
    desc->startColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
    desc->decay = glm::vec4(0.0f, 0.8f, 2.f, 0.01f);
    desc->decayTime = 5.0f;
    addComponent(std::make_shared<ExplodableComponent>(this, managers, std::move(desc)));
	addComponent(std::make_shared<LifeTimeComponent>(this, managers, 5.0));

    Model::Vertex vertex;
    std::vector<Model::Vertex> vertices;
    // Top
    vertex.position = glm::vec3(0.f, 0.1f, 0.f);
    vertex.color = glm::vec4(0.75f, 0.75f, 0.75f, 1.f);
    vertices.push_back(vertex);
    // Bottom Left
    vertex.position = glm::vec3(-0.1f, -0.1f, -0.1f);
    vertex.color = glm::vec4(0.75f, 0.75f, 0.75f, 1.f);
    vertices.push_back(vertex);
    // Bottom Right
    vertex.position = glm::vec3(0.1f, -0.1f, -0.1f);
    vertex.color = glm::vec4(0.75f, 0.75f, 0.75f, 1.f);
    vertices.push_back(vertex);
    // Bottom Front
    vertex.position = glm::vec3(0.f, -0.1f, 0.1f);
    vertex.color = glm::vec4(0.75f, 0.75f, 0.75f, 1.f);
    vertices.push_back(vertex);

    std::vector<unsigned short> indices({0, 1, 3, 3, 2, 0});

    m_Model = std::make_shared<Model>(context, vertices, indices);

    setupOutline(context);
}

void TestTriangle::setupOutline(Context* context)
{
    Model::Vertex vertex;
    std::vector<Model::Vertex> vertices;
    // Top
    vertex.position = glm::vec3(0.f, 0.1f, 0.f);
    vertex.color = glm::vec4(1.f, 1.f, 1.f, 1.f);
    vertices.push_back(vertex);
    // Bottom Left
	vertex.position = glm::vec3(-0.1f, -0.1f, -0.1f);
	vertex.color = glm::vec4(1.f, 1.f, 1.f, 1.f);
    vertices.push_back(vertex);
    // Bottom Right
	vertex.position = glm::vec3(0.1f, -0.1f, -0.1f);
	vertex.color = glm::vec4(1.f, 1.f, 1.f, 1.f);
    vertices.push_back(vertex);
    // Bottom Front
	vertex.position = glm::vec3(0.f, -0.1f, 0.1f);
	vertex.color = glm::vec4(1.f, 1.f, 1.f, 1.f);
    vertices.push_back(vertex);

    std::vector<unsigned short> indices({0, 1, 1, 3, 3, 2, 2, 0, 0, 3});

    m_Outline = std::make_shared<Model>(context, vertices, indices);
    m_Outline->setPrimitiveMode(PrimitiveMode::Lines);
}

TestTriangle::~TestTriangle()
{
    MessageSystem::getInstance().unsubscribe("Death", this);
}


void TestTriangle::step(double dt)
{
    SceneObject::step(dt);
}

void TestTriangle::predraw(Context* context)
{
    SceneObject::predraw(context);
}

void TestTriangle::draw(Context* context,std::shared_ptr<ShaderManager> shaders)
{
    std::shared_ptr<ShaderProgram> shader = shaders->get("passthrough");
    m_Model->draw(context, shader);
    m_Outline->draw(context, shader);
}

void TestTriangle::onMessage(std::shared_ptr<Message> event)
{
    if(event->getType().compare("Death") == 0)
    {
        std::shared_ptr<DeathMessage> dMsg =
            std::static_pointer_cast<DeathMessage>(event);
        
        if (dMsg->getDoomed() == this)
            setDying();
    }
}
