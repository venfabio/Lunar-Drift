#include "Renderer.h"
#include "../../../game/scenes/alphaspace/TestScene.h"
#include "../../../game/scenes/game/RaceScene.h"
#include "../../exceptions/OpenGLException.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

Renderer::Renderer(Context* context, GLsizei width, GLsizei height)
    : m_WidthPx(width),
      m_HeightPx(height),
      m_ClearColor(0.f, 0.f, 0.f, 0.f)
{
    //ctor
}

Renderer::~Renderer()
{
    //dtor
}


void Renderer::getDimensions(GLsizei* width, GLsizei* height) const
{
    *width = m_WidthPx;
    *height = m_HeightPx;
}

void Renderer::resize(GLsizei width, GLsizei height)
{
	m_WidthPx = width;
	m_HeightPx = height;
	m_Resized = true;
}

void Renderer::setSize(Context* context, std::shared_ptr<Scene> scene)
{
	GLenum errCode;
	assert((errCode = glGetError()) == GL_NO_ERROR);

	GLsizei w, h;
	getDimensions(&w, &h);
	glViewport(0, 0, w, h);
	scene->setSize(w, h);
	if ((errCode = glGetError()) != GL_NO_ERROR)
		throw OpenGLException("Could not set viewport", errCode);

    m_ScreenMatrix = glm::ortho(
        0.f, static_cast<GLfloat>(w), 
        static_cast<GLfloat>(h), 0.f);

	m_Resized = false;
}

void Renderer::preFrame(Context* context, std::shared_ptr<Scene> scene)
{
    glClearColor(
        m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glCullFace(GL_BACK);

    scene->predraw(context);
}

void Renderer::updateShaderStates(
    Context* context,
    std::shared_ptr<Scene> scene,
    std::shared_ptr<ShaderManager> shaders)
{
    std::vector<std::shared_ptr<ShaderProgram>> shaderList;
    shaders->get(&shaderList);

    std::shared_ptr<Camera> camera = scene->getActiveCamera().lock();
    for (std::shared_ptr<ShaderProgram> shader : shaderList)
    {
        camera->apply(context, shader.get());
        GLint sm = shader->getUniformLocation(context, "u_ScreenMatrix");
        if (sm != -1)
            glUniformMatrix4fv(sm, 1, GL_FALSE, glm::value_ptr(m_ScreenMatrix));
    }
}

void Renderer::drawScene(
    Context* context,
    std::shared_ptr<Scene> scene,
    std::shared_ptr<ShaderManager> shaders)
{
    std::list<std::shared_ptr<SceneObject>> objects;
    scene->getObjects(&objects);

    std::list<std::shared_ptr<SceneObject>>::iterator itr;
    for(itr = objects.begin(); itr != objects.end(); ++itr)
    {
        std::shared_ptr<SceneObject> object = *itr;
        if (object->getState() == SceneObject::State::Ready)
            object->draw(context, shaders);
    }
}

void Renderer::frame(Context* context, std::shared_ptr<Scene> scene)
{
	GLenum errCode;
	assert((errCode = glGetError()) == GL_NO_ERROR);

	if (m_Resized)
		setSize(context, scene);

	unsigned int numPasses = scene->getNumRenderPasses();
	if (numPasses == 1)
	{
		scene->setRenderPass(context, 0);

		updateShaderStates(context, scene, m_StandardShaders);
		drawScene(context, scene, m_StandardShaders);
	}
	else 
	{
		GLsizei w, h;
		getDimensions(&w, &h);

		h /= numPasses;

		for (unsigned int pass = 0; pass < numPasses; ++pass)
		{
			glViewport(0, pass * h, w, h);
			scene->setSize(w, h);
			if ((errCode = glGetError()) != GL_NO_ERROR)
				throw OpenGLException("Could not set viewport", errCode);

			m_ScreenMatrix = glm::ortho(
				0.f, static_cast<GLfloat>(w),
				static_cast<GLfloat>(h), 0.f);

			scene->setRenderPass(context, pass);

			updateShaderStates(context, scene, m_StandardShaders);
			drawScene(context, scene, m_StandardShaders);
		}
	}
}

void Renderer::postFrame(Context* context, std::shared_ptr<Scene> scene)
{

}

void Renderer::load(Context* context)
{
	m_StandardShaders = loadStandardShaders(context);
}
