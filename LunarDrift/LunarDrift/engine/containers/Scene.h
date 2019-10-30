#pragma once

#include "../graphics/Context.h"
#include "SceneObject.h"
#include "../graphics/Camera.h"
#include <list>

/**
 * @defgroup TopLevelContainers
 * @brief The main backbone of architectural structuring in the engine; the "spine" of the project.
 */

/**
 * @brief Scene container for managing and drawing 3D environments
 *
 * @pre    A valid OpenGL context must be present to the program
 * @author Hayley Hatton
 * @date   06/03/2016
 * @ingroup TopLevelContainers
 */
class Scene
{
public:
    /**
     * @brief Default constructor
     * @param context Graphics context
     */
    Scene(Context* context);

    /** @brief Default destructor */
    virtual ~Scene();


    /**
     * @brief Retrieve the list of objects in the scene
     * @param objects Pointer to list to fill with scene objects
     */
    void getObjects(std::list<std::shared_ptr<SceneObject> >* objects);

    /**
    * @brief Get an object in the scene with a given identifier
    * @param id Unique dentifier name of the object
    * @return Smart ptr to the scene object
    */
    std::shared_ptr<SceneObject> getObject(const std::string& id);

    /**
     * @brief Access the currently active camera
     * @return Managed pointer to the currently active camera
     */
    const std::weak_ptr<Camera> getActiveCamera() const;

    /**
     * @brief Called to update scene elements with the viewport information
     * @param width Width of the viewport in pixels
     * @param height Height of the viewport in pixels
     */
    virtual void setSize(GLsizei width, GLsizei height);

    /**
     * @brief Step the scene's simulation state
     * @param dt Delta-time since last step call in seconds
     */
    virtual void step(double dt);

    /**
     * @brief Called before the renderer draws the scene
     * This allows the scene and its constituent objects to do updates that
     * require a graphics context before the graphical state is drawn
     * @param context Graphics context
     */
    virtual void predraw(Context* context);

	/**
	 * @brief Notify the scene that the renderer intends to perform a given pass
     * @param context Graphics context
	 * @param Pass number, with 0 being the first pass in the frame
	 */
	virtual void setRenderPass(Context* context, unsigned int pass) { m_RenderPass = pass; }

	/**
	 * @brief Get the currently-intended render pass
	 * @return Pass number, with 0 being the first pass in the frame
	 */
	unsigned int getRenderPass() const { return m_RenderPass; }

	/**
	* @brief Get the number of render passes the renderer should perform per frame
	* @return Number of passes
	*/
	unsigned int getNumRenderPasses() const { return m_NumRenderPasses; }


protected:
    /**
     * @brief Add an object to be managed by the scene
     * @param object Smart pointer to the scene object
     */
    void addObject(std::shared_ptr<SceneObject> object);

    /**
    * @brief Add an object to the scene with a given identifier
    * @param id Identifier name (must be unique to object)
    * @param object Smart ptr to the scene object
    */
    void addObject(const std::string& id, std::shared_ptr<SceneObject> object);

    /**
     * @brief Set the active camera for the scene
     * @param camera Smart pointer to the camera to make active
     */
    void setActiveCamera(std::shared_ptr<Camera> camera);

	/**
	 * @brief Flag to the renderer how many render passes to perform per frame
	 * @param passes Number of render passes to perform
	 */
	void setNumRenderPasses(unsigned int passes) { m_NumRenderPasses = passes; }

	/**
	 * @brief Access the metamanager for managing managers
	 * @return Weakly-managed pointer to metamanager
	 */
    std::weak_ptr<MetaManager> getManagers() { return m_Managers; }

private:
    std::shared_ptr<MetaManager> m_Managers;
    std::shared_ptr<Camera> m_ActiveCamera;   //!< Camera currently in use
	unsigned int m_NumRenderPasses;	//!< How many passes the renderer should perform
	unsigned int m_RenderPass;		//!< Which render pass is currently being performed
};
