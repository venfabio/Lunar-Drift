#pragma once

#include "Manager.h"
#include "../components/CollisionComponent.h"
#include <list>

/**
 * @brief A manager class for encapsulating collision detection processes
 *
 * @author Hayley Hatton
 * @date   13/03/2016
 * @see    Scene
 * @ingroup Managers
 */
class CollisionManager : public Manager
{
public:
	/** @brief Default constructor */
	CollisionManager();

	/** @brief Default destructor */
	~CollisionManager();

	
	/** 
	 * @brief Set a collision component to be checked for collisions 
	 * @param collidable Reference to a component for describing collisions
	 */
	void registerCollidable(std::weak_ptr<CollisionComponent> collidable);

	/**
	 * @brief Remove a collision component from being checked for collisions
	 * @param collidable Reference to a component for describing collisions
	 */
	void unregisterCollidable(std::weak_ptr<CollisionComponent> collidable);

	//std::list<CollisionComponent*>& getCollidableList();

	/**
	 * @brief Check for collisions and dispatch appropriate messages
	 */
	void doCollisions();


private:
	std::list<std::weak_ptr<CollisionComponent>> m_Collidables;
};
