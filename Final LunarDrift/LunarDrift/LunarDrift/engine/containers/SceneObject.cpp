#include "SceneObject.h"
#include "../graphics/IDeferredLoadable.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cassert>

SceneObject::SceneObject(Context* context, std::weak_ptr<MetaManager> managers)
    : m_Managers(managers),
      m_State(State::NeedsDeferredLoading)
{
    if (context != nullptr)
        setReady();
}

SceneObject::~SceneObject()
{
    //dtor
}


bool SceneObject::addComponent(
	std::shared_ptr<SceneObjectComponent> component)
{
	if (m_Components.find(component->getName()) == m_Components.end())
	{
		m_Components[component->getName()] = component;
		return true;
	}

	return false;
}

std::shared_ptr<SceneObjectComponent> SceneObject::getComponent(
	const std::string& name)
{
	auto itr = m_Components.find(name);
	if (itr == m_Components.end())
		return nullptr;
	return (*itr).second;
}

void SceneObject::step(double dt)
{
	for (auto itr = m_Components.begin(); itr != m_Components.end(); ++itr)
		(*itr).second->step(dt);
}

void SceneObject::predraw(Context* context)
{
	for (auto itr = m_Components.begin(); itr != m_Components.end(); ++itr)
		(*itr).second->predraw(context);
}

void SceneObject::getModelMatrix(glm::mat4* mm) const
{
    *mm = glm::translate(*mm, m_Position);			// Apply pos translate
	*mm = glm::rotate(*mm, m_Rotation.z, glm::vec3(0, 0, 1));	// Apply roll
	*mm = glm::rotate(*mm, m_Rotation.y, glm::vec3(0, 1, 0));	// Apply yaw
	*mm = glm::rotate(*mm, m_Rotation.x, glm::vec3(1, 0, 0));	// Apply pitch
}

void SceneObject::updateModelMatrixState(
	Context* context,
	std::shared_ptr<ShaderProgram> shader) const
{
	assert(context);
	assert(shader);

	shader->use(context);

	GLint mmPtr = shader->getUniformLocation(context, "u_ModelMatrix");

	if (mmPtr != -1)
	{
		glm::mat4 mm;
        getModelMatrix(&mm);
		glUniformMatrix4fv(mmPtr, 1, GL_FALSE, glm::value_ptr(mm));
	}

	GLenum errCode;
	assert((errCode = glGetError()) == GL_NO_ERROR);
}
