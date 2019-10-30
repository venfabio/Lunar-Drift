#pragma once

#include "graphics/renderers/Renderer.h"
#include "containers/Scene.h"
#include <memory>
#include <string>

/**
 * @brief Top-level program entry point class
 *
 * @author Hayley Hatton
 * @date   20/02/2016
 * @ingroup TopLevelContainers
 */
class Program
{
public:
    /**
     * @brief Object-oriented entry point to the program
     * @param name String name of the program
     * @param context Graphics context
     */
    Program(const std::string& name, Context* context);

    /** @brief Default destructor */
    virtual ~Program();


    /**
     * @brief Access the name of the program
     * @return Reference to name. Do not keep the reference; copy instead
     */
    const std::string& getName() const { return m_Name; }

    /**
     * @brief Called on the stepping of the simulation state
     * @param dt Delta time, in seconds
     */
    virtual void step(double dt);

    /**
     * @brief Called on the rendering of a frame
     * @param context Graphical context
     */
    virtual void frame(Context* context);

	/**
	 * @brief Called to update scene elements with the viewport information
     * @param context Graphical context
	 * @param width Width of the viewport in pixels
	 * @param height Height of the viewport in pixels
	 */
	virtual void setSize(Context* context, GLsizei width, GLsizei height);


protected:
    /**
     * @brief Set the active renderer to render the scenes with
     * @param context Graphics context
     * @param renderer Renderer to draw with
     */
    void setRenderer(Context* context, std::shared_ptr<Renderer> renderer);

    /**
     * @brief Retrieve a new renderer from the subclass
     * @param context Graphics context
     * @return Smart pointer to the new appropriate renderer
     */
    virtual std::shared_ptr<Renderer> getRenderer(Context* context) const = 0;

    /**
     * @brief Set the active scene to manage and render
     * @param scene Active scene
     */
    void setActiveScene(std::shared_ptr<Scene> scene);

	/**
	* @brief Get the active scene
	* @param scene Active scene
	*/
	std::shared_ptr<Scene> getActiveScene() { return m_ActiveScene; }

	/**
	* @brief Force a dynamic resize
	*/
	void invalidate() { m_Invalidated = true; }

private:
    const std::string m_Name;               //!< Name of the program
    std::shared_ptr<Renderer> m_Renderer;   //!< Active renderer
    std::shared_ptr<Scene> m_ActiveScene;   //!< Active scene
	bool m_Invalidated;						//!< Request resize!
};
