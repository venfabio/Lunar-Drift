#include "TestStarfield.h"
#include "../../../engine/MathHelper.h"

TestStarfield::TestStarfield(
	Context* context, 
	std::weak_ptr<MetaManager> managers) 
	: SceneObject(context, managers)
{
    std::vector<Model::Vertex> vertices;
    generateStars(&vertices);

    m_Stars = std::make_shared<Model>(context, vertices);
    m_Stars->setPrimitiveMode(PrimitiveMode::Points);
}


void TestStarfield::generateStars(std::vector<Model::Vertex>* vertices) const
{
    const glm::vec4 starColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
    const unsigned int numStars = 1000;
    const GLfloat starboxSize = 100.f;
    const GLfloat halfSbSize = starboxSize / 2.f;

    Model::Vertex vertex;
    vertex.color = starColor;
    for (unsigned int i = 0; i < numStars; ++i)
    {
        GLfloat xPos = randf(-halfSbSize, halfSbSize);
        GLfloat yPos = randf(-halfSbSize, halfSbSize);
        GLfloat zPos = randf(-halfSbSize, halfSbSize);
        vertex.position = glm::vec3(xPos, yPos, zPos);
        vertices->push_back(vertex);
    }
}

TestStarfield::~TestStarfield()
{
}


void TestStarfield::step(double dt)
{
    SceneObject::step(dt);
}

void TestStarfield::predraw(Context* context)
{
    SceneObject::predraw(context);
}

void TestStarfield::draw(Context* context, std::shared_ptr<ShaderManager> shaders)
{
    std::shared_ptr<ShaderProgram> shader = shaders->get("passthrough");
    m_Stars->draw(context, shader);
}
