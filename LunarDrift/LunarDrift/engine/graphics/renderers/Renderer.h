#pragma once

#include "../Context.h"
#include "../../containers/Scene.h"
#include "../../managers/ShaderManager.h"

/**
 * @brief Base renderer for drawing scenes within a given graphics context
 *
 * Renderers are applied at the Program level to draw scenes as abstract
 * notions of "scene graphs" composed of SceneObjects. This allows for a
 * relative amount of decoupling of rendering techniques and processes away
 * from the scene management itself. Although each object draws itself,
 * the modification of OpenGL state upon which the drawing operates in
 * allows for various techniques to be applied without the objects needing
 * to know. For controlling <i>how</i> objects render themselves, the objects
 * use passed-in ShaderManagers to acquire the ShaderProgram assigned to the
 * shading class name, which allows for passing of different ShaderPrograms
 * into the draw calls to draw the objects in different ways.
 *
 * @author Hayley Hatton
 * @date   20/02/2016
 * @see    Program
 * @see    Scene
 * @see    SceneObject
 * @see    ShaderManager
 * @ingroup Graphics
 */
class Renderer
{
public:
    /**
     * @brief Default constructor
     * @param context Graphics context
     * @param width Width of context in pixels
     * @param height Height of context in pixels
     */
    Renderer(Context* context, GLsizei width, GLsizei height);

    /** @brief Default destructor */
    virtual ~Renderer();


    /**
     * @brief Access the dimensions of the full rendering context
     * @param width Pointer to store width in pixels
     * @param height Pointer to store height in pixels
     */
    void getDimensions(GLsizei* width, GLsizei* height) const;

    /**
     * @brief Called before the frame is drawn; useful for setting up the frame
     * @param context Graphics context
     * @param scene Smart pointer to the scene to render
     */
    virtual void preFrame(Context* context, std::shared_ptr<Scene> scene);

    /**
     * @brief Called to draw the frame
     * @param context Graphics context
     * @param scene Smart pointer to the scene to render
     */
    virtual void frame(Context* context, std::shared_ptr<Scene> scene);

    /**
     * @brief Called after the frame is drawn; useful for posting the frame
     * @param context Graphics context
     * @param scene Smart pointer to the scene to render
     */
    virtual void postFrame(Context* context, std::shared_ptr<Scene> scene);

    void setClearColor(const glm::vec4& color) { m_ClearColor = color; }

	/**
	 * @brief Resize the renderer
	 * @param width New width of GL context in pixels
	 * @param height New height of GL context in pixels
	 */
	void resize(GLsizei width, GLsizei height);

	/** 
	 * @brief Force a dynamic resize
	 */
	void invalidate() { m_Resized = true; }

protected:
	/**
	 * @brief Update the viewport and echo changes to the scene
	 * @param context Graphics context
	 * @param scene Smart pointer to the scene to resize
	 */
	void setSize(Context* context, std::shared_ptr<Scene> scene);

    /**
     * @brief Create and return a shader manager containing the standard shaders
     * @return Managed pointer of a default shader manager
     */
    virtual std::shared_ptr<ShaderManager> loadStandardShaders(
		Context* context) = 0;

	/**
	* @brief Load the renderer
	* @param context Graphics context
	*/
	void load(Context* context);

    /**
     * @brief Update the shader states of all the shaders in a shader manager
     * @param context Graphics context
     * @param scene Smart pointer to the scene instance to use
     * @param shaders Smart pointer to the shader manager to update states with
     */
    void updateShaderStates(
        Context* context,
        std::shared_ptr<Scene> scene,
        std::shared_ptr<ShaderManager> shaders);

    /**
     * @brief Draw a scene using a given shader manager
     * @param context Graphics context
     * @param scene Smart pointer to the scene instance to draw
     * @param shaders Smart pointer to the shader manager to draw scene with
     */
    void drawScene(
        Context* context,
        std::shared_ptr<Scene> scene,
        std::shared_ptr<ShaderManager> shaders);

private:
    GLsizei m_WidthPx, m_HeightPx;
    glm::mat4 m_ScreenMatrix;
	bool m_Resized;
	glm::vec4 m_ClearColor;
	std::shared_ptr<ShaderManager> m_StandardShaders;
};
