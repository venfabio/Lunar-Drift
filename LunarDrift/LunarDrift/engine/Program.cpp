#include "Program.h"
#include "comms/MessageSystem.h"
#include "comms/predefined/ButtonDownEvent.h"

Program::Program(const std::string& name, Context* context)
    : m_Name(name),
      m_Renderer(nullptr),
      m_ActiveScene(nullptr),
	  m_Invalidated(false)
{
}

Program::~Program()
{
}


void Program::step(double dt)
{
    if(m_ActiveScene != nullptr)
		m_ActiveScene->step(dt);
}

void Program::frame(Context* context)
{
    if(m_Renderer == nullptr)
        setRenderer(context, getRenderer(context));

    if((m_ActiveScene != nullptr) && (m_Renderer != nullptr))
    {
		if (m_Invalidated)
			m_Renderer->invalidate();

        m_Renderer->preFrame(context, m_ActiveScene);
        m_Renderer->frame(context, m_ActiveScene);
        m_Renderer->postFrame(context, m_ActiveScene);
    }
}

void Program::setSize(Context* context, GLsizei width, GLsizei height)
{
	m_Renderer->resize(width, height);
}

void Program::setRenderer(
    Context* context,
    std::shared_ptr<Renderer> renderer)
{
    m_Renderer = renderer;
}

void Program::setActiveScene(std::shared_ptr<Scene> scene)
{
    m_ActiveScene = scene;
}
