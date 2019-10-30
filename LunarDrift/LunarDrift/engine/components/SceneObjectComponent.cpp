#include "SceneObjectComponent.h"
#include "../containers/SceneObject.h"


SceneObjectComponent::SceneObjectComponent(
	const std::string& name,
	SceneObject* parent,
	std::weak_ptr<MetaManager> managers) 
  : BaseComponent(name),
	m_Managers(managers),
	m_Parent(parent)
{
    //m_Parent->addComponent(std::shared_ptr<SceneObjectComponent>(this));
}

SceneObjectComponent::~SceneObjectComponent()
{
	//unload();
}

