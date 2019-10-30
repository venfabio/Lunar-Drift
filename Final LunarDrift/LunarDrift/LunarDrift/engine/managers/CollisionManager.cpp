#include "CollisionManager.h"
#include "../comms/predefined/CollisionMessage.h"
#include "../comms/MessageSystem.h"
#include "../containers/SceneObject.h"


CollisionManager::CollisionManager() : Manager("collision")
{
}

CollisionManager::~CollisionManager()
{
}


void CollisionManager::registerCollidable(
    std::weak_ptr<CollisionComponent> collidable)
{
	m_Collidables.push_back(collidable);
}

void CollisionManager::unregisterCollidable(
    std::weak_ptr<CollisionComponent> collidable)
{
	m_Collidables.remove_if([collidable](std::weak_ptr<CollisionComponent> c){
		std::shared_ptr<CollisionComponent> c1 = collidable.lock();
		std::shared_ptr<CollisionComponent> c2 = c.lock();
		if (c1 && c2)
			return c1 == c2;
		return false;
	});
}

void CollisionManager::doCollisions()
{
    for (std::weak_ptr<CollisionComponent> collidableA : m_Collidables)
    {
        SceneObject* parentA = collidableA.lock()->getParent();
        if (parentA->getState() != SceneObject::State::Ready)
            continue;

		glm::mat4 mmA;
		parentA->getModelMatrix(&mmA);
        float farRadiusA = collidableA.lock()->getFarRadius();
        glm::vec4 positionA = mmA * glm::vec4(0, 0, 0, 1);
        const std::vector<CollisionSphere>& spheresA =
            collidableA.lock()->getCollisionSpheres();

        for (std::weak_ptr<CollisionComponent> collidableB : m_Collidables)
        {
			if (collidableB.lock() == collidableA.lock())
				continue;

            SceneObject* parentB = collidableB.lock()->getParent();
            if (parentB->getState() != SceneObject::State::Ready)
                continue;

			glm::mat4 mmB;
			parentB->getModelMatrix(&mmB);
            float farRadiusB = collidableB.lock()->getFarRadius();
			glm::vec4 positionB = mmB * glm::vec4(0, 0, 0, 1);
            const std::vector<CollisionSphere>& spheresB =
                collidableB.lock()->getCollisionSpheres();

            // If broad spheres overlap, do more intensive narrow collision detection
			if (glm::length(positionB - positionA) < (farRadiusB + farRadiusA))
            {
                for (const CollisionSphere& sphereA : spheresA)
                {
                    glm::vec4 offsetA = mmA * glm::vec4(sphereA.offset, 1.0);
                    for (const CollisionSphere& sphereB : spheresB)
                    {
                        glm::vec4 offsetB = mmB * glm::vec4(sphereB.offset, 1.0);
                        if (glm::length(offsetB - offsetA) < (sphereA.radius + sphereB.radius))
                        {
                            IMessageListener* listenerA =
                                dynamic_cast<IMessageListener*>(parentA);
                            IMessageListener* listenerB =
                                dynamic_cast<IMessageListener*>(parentB);

                            std::shared_ptr<CollisionMessage> msg1 = 
                                std::make_shared<CollisionMessage>(parentB, sphereA.id);
                            std::shared_ptr<CollisionMessage> msg2 = 
                                std::make_shared<CollisionMessage>(parentA, sphereB.id);

                            if (listenerA != nullptr)
                                listenerA->onMessage(msg1);
                            if (listenerB != nullptr)
                                listenerB->onMessage(msg2);
                        }
                    }
                }
            }
		}
	}
}
