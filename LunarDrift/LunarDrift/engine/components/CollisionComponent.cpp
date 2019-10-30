#include "CollisionComponent.h"
#include "../containers/SceneObject.h"
#include "../managers/SceneObjectManager.h"
#include "../managers/CollisionManager.h"
#include "../comms/MessageSystem.h"
#include "../Config.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


CollisionComponent::CollisionComponent(
	SceneObject* parent, 
    std::weak_ptr<MetaManager> managers,
    float farRadius,
    const std::vector<CollisionSphere> points)
    : SceneObjectComponent("collision", parent, managers),
      m_CollisionSpheres(points),
      m_FarRadius(farRadius),
      m_Registered(false),
	  m_Debugging(false)
{
    IMessageListener* listening = dynamic_cast<IMessageListener*>(getParent());
    if (listening != nullptr)
        MessageSystem::getInstance().subscribe("CollisionMessage", listening);
}

CollisionComponent::~CollisionComponent()
{
    IMessageListener* parent = dynamic_cast<IMessageListener*>(getParent());
    if (parent != nullptr)
        MessageSystem::getInstance().unsubscribe("CollisionMessage", parent);

    if (m_Managers.expired())
        return;
    std::weak_ptr<Manager> cmRaw = m_Managers.lock()->get("collision");
    if (cmRaw.expired())
        return;
    std::shared_ptr<CollisionManager> cm =
        std::dynamic_pointer_cast<CollisionManager>(cmRaw.lock());
    cm->unregisterCollidable(shared_from_this());
}

void CollisionComponent::step(double dt)
{
    if (!m_Registered)
    {
        std::weak_ptr<Manager> cmRaw = m_Managers.lock()->get("collision");
        assert(!cmRaw.expired());
        std::shared_ptr<CollisionManager> cm =
            std::dynamic_pointer_cast<CollisionManager>(cmRaw.lock());
        cm->registerCollidable(shared_from_this());

        m_Registered = true;
    }
}


void CollisionComponent::predraw(Context* context)
{
	bool debugging = Config::get().getBool("_engine", "debugCollisions");
	if (debugging && !m_Debugging)
	{
		std::shared_ptr<MetaManager> mgr = getMetaManager().lock();
		if (mgr == nullptr)
			return;

		std::shared_ptr<SceneObjectManager> som =
			std::static_pointer_cast<SceneObjectManager>(
				mgr->get("scene").lock());
		if (som == nullptr)
			return;


		for (const CollisionSphere& sphere : m_CollisionSpheres)
		{
			std::shared_ptr<DebuggingSphere> dSphere =
				std::make_shared<DebuggingSphere>(
					context, mgr, sphere.offset, sphere.radius, false);

			m_DebuggingSpheres.push_back(dSphere);
			som->addSceneObject(dSphere);
		}

		// Add sphere that does broad collision testing
		std::shared_ptr<DebuggingSphere> dSphere =
			std::make_shared<DebuggingSphere>(
			context, mgr, glm::vec3(), m_FarRadius, true);

		m_DebuggingSpheres.push_back(dSphere);
		som->addSceneObject(dSphere);
	}
	else if (!debugging && m_Debugging)
	{
		std::shared_ptr<MetaManager> mgr = getMetaManager().lock();
		if (mgr == nullptr)
			return;

		std::shared_ptr<SceneObjectManager> som =
			std::static_pointer_cast<SceneObjectManager>(
				mgr->get("scene").lock());
		if (som == nullptr)
			return;

		for (std::weak_ptr<DebuggingSphere> sphere : m_DebuggingSpheres)
			som->removeSceneObject(sphere.lock());
		m_DebuggingSpheres.clear();
	}

	m_Debugging = debugging;

	if (debugging)
	{
		glm::mat4 mm;
		getParent()->getModelMatrix(&mm);

		for (std::weak_ptr<DebuggingSphere> sphere : m_DebuggingSpheres)
			sphere.lock()->setModelMatrix(mm);
	}
}

const std::vector<CollisionSphere>& CollisionComponent::getCollisionSpheres() const
{
    return m_CollisionSpheres;
}
