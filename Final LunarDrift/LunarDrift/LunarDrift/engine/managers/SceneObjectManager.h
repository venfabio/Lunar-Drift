#pragma once

#include "Manager.h"
#include "../containers/SceneObject.h"
#include <list>

/**
 * @brief A manager class for handling and passing around scene objects
 *
 * @author Hayley Hatton
 * @date   11/03/2016
 * @ingroup Managers
 */
class SceneObjectManager : public Manager
{
public:
	/** @brief Default constructor */
	SceneObjectManager();

	/** @brief Default destructor */
	~SceneObjectManager();


	/**
	 * @brief Add a scene object to the scene
	 * @param object Smart ptr to the scene object
	 */
    void addSceneObject(std::shared_ptr<SceneObject> object);

    /**
    * @brief Add a scene object to the scene with a given identifier
    * @param id Identifier name (must be unique to object)
    * @param object Smart ptr to the scene object
    */
    void addSceneObject(
        const std::string& id, 
        std::shared_ptr<SceneObject> object);

    /**
    * @brief Get a scene object in the scene with a given identifier
    * @param id Unique dentifier name of the object
    * @return Smart ptr to the scene object
    */
    std::shared_ptr<SceneObject> getSceneObject(const std::string& id);

	/**
	 * @brief Remove a scene object from the scene (and destroy if applicable)
	 * @param object Smart ptr to the scene object to remove
	 */
    void removeSceneObject(std::shared_ptr<SceneObject> object);

    /**
     * @brief Retrieve the list of objects in the scene
     * @param objects Pointer to list to fill with scene objects
     */
    void getObjects(std::list<std::shared_ptr<SceneObject>>* objects);

    /**
     * @brief Step the scene's simulation state
     * @param dt Delta-time since last step call in seconds
     */
    void step(double dt);

    /**
     * @brief Called before the renderer draws the scene
     *
     * This allows the scene and its constituent objects to do updates that
     * require a graphics context before the graphical state is drawn
     * @param context Graphics context
     */
    void predraw(Context* context);

private:
    std::list<std::shared_ptr<SceneObject>> m_SceneObjects;
    std::map<std::string, std::weak_ptr<SceneObject>> m_RelationsTable;

    /** 
     * @brief Finalize the death and cleanup of objects marked as dying 
     * @param context Graphics context
     */
    void cleanupDyingObjects(Context* context);
};
