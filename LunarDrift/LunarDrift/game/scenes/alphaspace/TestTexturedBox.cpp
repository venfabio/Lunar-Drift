#include "TestTexturedBox.h"
#include "../../../engine/components/CollisionComponent.h"

TestTexturedBox::TestTexturedBox(
    Context* context,
    std::weak_ptr<MetaManager> managers,
    const glm::vec3& position)
	: SceneObject(context, managers)
{
    setPosition(position);

    m_Texture = std::make_unique<Texture>(context, "test.dds");

	std::vector<CollisionSphere> points;
	points.push_back({ 1, glm::vec3(0.25, 0.25, 0.25), 0.35 });
	points.push_back({ 2, glm::vec3(0.25, 0.75, 0.25), 0.35 });
	points.push_back({ 3, glm::vec3(-0.25, 0.25, 0.25), 0.35 });
	points.push_back({ 4, glm::vec3(-0.25, 0.75, 0.25), 0.35 });
	points.push_back({ 5, glm::vec3(0.25, 0.25, 0.75), 0.35 });
	points.push_back({ 6, glm::vec3(0.25, 0.75, 0.75), 0.35 });
	points.push_back({ 7, glm::vec3(-0.25, 0.25, 0.75), 0.35 });
	points.push_back({ 8, glm::vec3(-0.25, 0.75, 0.75), 0.35 });
	addComponent(std::make_shared<CollisionComponent>(this, managers, 1.5f, points));

    m_Model = std::make_shared<Model>(context, "test2");
}

TestTexturedBox::~TestTexturedBox()
{
}


void TestTexturedBox::step(double dt)
{
    SceneObject::step(dt);
}

void TestTexturedBox::predraw(Context* context)
{
    SceneObject::predraw(context);
}

void TestTexturedBox::draw(Context* context, std::shared_ptr<ShaderManager> shaders)
{
    std::shared_ptr<ShaderProgram> shader = shaders->get("texbox");
    updateModelMatrixState(context, shader);

    m_Texture->bind(context);

    m_Model->draw(context, shader);
}
