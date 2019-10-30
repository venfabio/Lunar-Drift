#include "SceneObjectManager.h"
#include "../graphics/IDeferredLoadable.h"


SceneObjectManager::SceneObjectManager() : Manager("scene")
{
}

SceneObjectManager::~SceneObjectManager()
{
}


void SceneObjectManager::addSceneObject(
    std::shared_ptr<SceneObject> object)
{
	m_SceneObjects.push_back(object);
}

void SceneObjectManager::addSceneObject(
    const std::string& id,
    std::shared_ptr<SceneObject> object)
{
    addSceneObject(object);
    m_RelationsTable[id] = object;
}

std::shared_ptr<SceneObject> SceneObjectManager::getSceneObject(
    const std::string& id)
{
    auto itr = m_RelationsTable.find(id);
    if (itr == m_RelationsTable.end())
        return nullptr;
    return m_RelationsTable[id].lock();
}

void SceneObjectManager::removeSceneObject(
    std::shared_ptr<SceneObject> object)
{
	m_SceneObjects.remove(object);
}

void SceneObjectManager::getObjects(
	std::list<std::shared_ptr<SceneObject>>* objects)
{
	*objects = m_SceneObjects;
}

void SceneObjectManager::step(double dt)
{
    std::list<std::shared_ptr<SceneObject> >::iterator itr;
    for (itr = m_SceneObjects.begin(); itr != m_SceneObjects.end(); ++itr)
    {
        std::shared_ptr<SceneObject> object = *itr;
        object->step(dt);
    }
}

void SceneObjectManager::cleanupDyingObjects(Context* context)
{
    std::vector<std::shared_ptr<SceneObject> > graveyard;

    std::list<std::shared_ptr<SceneObject> >::iterator itr;
    for (itr = m_SceneObjects.begin(); itr != m_SceneObjects.end(); ++itr)
    {
        std::shared_ptr<SceneObject> object = *itr;
        if (object->getState() == SceneObject::State::Dying)
        {
            graveyard.push_back(object);

            IDeferredLoadable* objDeferred =
                dynamic_cast<IDeferredLoadable*>(object.get());
            if (objDeferred != nullptr)
                objDeferred->unload(context);
        }
    }

    for (std::shared_ptr<SceneObject>& object : graveyard)
        m_SceneObjects.remove(object);
}

void SceneObjectManager::predraw(Context* context)
{
    std::list<std::shared_ptr<SceneObject> >::iterator itr;
    for (itr = m_SceneObjects.begin(); itr != m_SceneObjects.end(); ++itr)
    {
        std::shared_ptr<SceneObject> object = *itr;
        if (object->getState() == SceneObject::State::NeedsDeferredLoading)
        {
            IDeferredLoadable* objDeferred = 
                dynamic_cast<IDeferredLoadable*>(object.get());
            if (objDeferred != nullptr)
                objDeferred->load(context);
        }
        if (object->getState() == SceneObject::State::Ready)
            object->predraw(context);
    }

    // Must do this on the graphics thread;
    // OpenGL context required for clean destructor calls
    cleanupDyingObjects(context);
}
