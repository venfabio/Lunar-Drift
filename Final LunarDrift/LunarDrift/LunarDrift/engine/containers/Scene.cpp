#include "Scene.h"
#include "../managers/SceneObjectManager.h"
#include "../managers/CollisionManager.h"
#include <vector>

Scene::Scene(Context* context)
    : m_ActiveCamera(nullptr),
	  m_NumRenderPasses(1),
	  m_RenderPass(0)
{
    m_Managers = std::make_shared<MetaManager>();
    m_Managers->add(std::make_shared<SceneObjectManager>());
    m_Managers->add(std::make_shared<CollisionManager>());
}

Scene::~Scene()
{
    //dtor
}


void Scene::getObjects(std::list<std::shared_ptr<SceneObject> >* objects)
{
    std::shared_ptr<SceneObjectManager> som =
        std::static_pointer_cast<SceneObjectManager>(
            m_Managers->get("scene").lock());
    som.get()->getObjects(objects);
}

std::shared_ptr<SceneObject> Scene::getObject(const std::string& id)
{
    std::shared_ptr<SceneObjectManager> som =
        std::static_pointer_cast<SceneObjectManager>(
            m_Managers->get("scene").lock());
    return som->getSceneObject(id);
}

void Scene::addObject(std::shared_ptr<SceneObject> object)
{
    std::shared_ptr<SceneObjectManager> som =
        std::static_pointer_cast<SceneObjectManager>(
            m_Managers->get("scene").lock());
    som->addSceneObject(object);
}

void Scene::addObject(
    const std::string& id,
    std::shared_ptr<SceneObject> object)
{
    std::shared_ptr<SceneObjectManager> som =
        std::static_pointer_cast<SceneObjectManager>(
            m_Managers->get("scene").lock());
    som->addSceneObject(id, object);
}

void Scene::step(double dt)
{
    std::shared_ptr<SceneObjectManager> som =
        std::static_pointer_cast<SceneObjectManager>(
            m_Managers->get("scene").lock());
    som->step(dt);

	std::shared_ptr<CollisionManager> cm =
        std::static_pointer_cast<CollisionManager>(
			m_Managers->get("collision").lock());
	cm->doCollisions();
}

void Scene::predraw(Context* context)
{
    std::shared_ptr<SceneObjectManager> som =
        std::static_pointer_cast<SceneObjectManager>(
            m_Managers->get("scene").lock());
    som->predraw(context);
}

void Scene::setActiveCamera(std::shared_ptr<Camera> camera)
{
    m_ActiveCamera = camera;
}

const std::weak_ptr<Camera> Scene::getActiveCamera() const
{
    return m_ActiveCamera;
}

void Scene::setSize(GLsizei width, GLsizei height)
{

}
