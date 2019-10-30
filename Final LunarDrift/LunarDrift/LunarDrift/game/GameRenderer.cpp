#include "GameRenderer.h"

GameRenderer::GameRenderer(
    Context* context,
    GLsizei width, GLsizei height)
    : Renderer(context, width, height)
{
    setClearColor(glm::vec4(0.f, 0.f, 0.f, 0.f));
    load(context);
}

GameRenderer::~GameRenderer()
{
    //dtor
}


std::shared_ptr<ShaderManager> GameRenderer::loadStandardShaders(
    Context* context)
{
    std::shared_ptr<ShaderManager> shaders = std::make_shared<ShaderManager>();

    std::shared_ptr<ShaderProgram> passthrough =
        std::make_shared<ShaderProgram>(context, "passthrough.vert", "passthrough.frag");
    shaders->set("passthrough", passthrough);

    std::shared_ptr<ShaderProgram> textured =
        std::make_shared<ShaderProgram>(context, "passthrough.vert", "passthrough_tex.frag");
    shaders->set("textured", textured);

    std::shared_ptr<ShaderProgram> raw =
        std::make_shared<ShaderProgram>(context, "raw.vert", "passthrough.frag");
	shaders->set("raw", raw);

	std::shared_ptr<ShaderProgram> collider =
		std::make_shared<ShaderProgram>(context, "collider.vert", "collider.frag");
	shaders->set("collider", collider);

    std::shared_ptr<ShaderProgram> ship =
        std::make_shared<ShaderProgram>(context, "ship.vert", "passthrough.frag");
    shaders->set("ship", ship);

    std::shared_ptr<ShaderProgram> texturedBox =
        std::make_shared<ShaderProgram>(context, "box.vert", "passthrough_tex.frag");
	shaders->set("texbox", texturedBox);

	std::shared_ptr<ShaderProgram> texturedBox2D =
		std::make_shared<ShaderProgram>(context, "box2d.vert", "passthrough_tex.frag");
	shaders->set("texbox2D", texturedBox2D);

	std::shared_ptr<ShaderProgram> skybox =
		std::make_shared<ShaderProgram>(context, "skybox.vert", "skybox.frag");
	shaders->set("skybox", skybox);

    std::shared_ptr<ShaderProgram> text =
        std::make_shared<ShaderProgram>(context, "text.vert", "text.frag");
    shaders->set("text", text);

    std::shared_ptr<ShaderProgram> particleSystem =
        std::make_shared<ShaderProgram>(context, "particle_system.vert", "passthrough.frag");
	shaders->set("particle_system", particleSystem);

	std::shared_ptr<ShaderProgram> explosion =
		std::make_shared<ShaderProgram>(context, "explosion.vert", "passthrough.frag");
	shaders->set("explosion", explosion);

    return shaders;
}
