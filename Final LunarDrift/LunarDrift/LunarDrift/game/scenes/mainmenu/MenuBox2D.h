#pragma once

#include "../../../engine/containers/SceneObject.h"
#include "../../../engine/containers/Model.h"
#include "../../../engine/graphics/Texture.h"

/**
 * @brief Used for rendering GUI elements in 2D coordinates
 *
 * @author Mariana
 * @date   26/05/2016
 * @ingroup Game
 */
class MenuBox2D : public SceneObject
{
public:
	/** @brief Default constructor */
	MenuBox2D(
		Context* context,
		std::weak_ptr<MetaManager> managers,
		const glm::vec2& position,
		const glm::vec2& dims);

	/** @brief Default destructor */
	~MenuBox2D();

	/**
	* @brief Set the texture for the menu option
	* @param context Graphics context
	* @param file String texture file name
	*/
	void setTexture(Context* context, const std::string& file);

	/**
	* @brief Step the simulation state
	* @param dt Delta-time since last step call in seconds
	*/
	void step(double dt) override;

	/**
	* @brief Called before the renderer draws the scene
	*
	* This allows the scene objects to do updates that require a
	* graphics context before the graphical state is drawn
	* @param context Graphics context
	*/
	void predraw(Context* context) override;

	/**
	* @brief Draw the object in its current state
	* @param context Graphics context
	* @param shaders Shader manager containing shader programs for use
	*/
	void draw(Context* context, std::shared_ptr<ShaderManager> shaders) override;

	void setPosition2D(const glm::vec2& pos) { SceneObject::setPosition(glm::vec3(pos, 0.f)); }

private:
	std::unique_ptr<VertexArrayObject> m_VAO;
	std::unique_ptr<Texture> m_Texture;

	/**
	 * @brief Get vertex attributes for mapping vertex data to shader pipeline
	 * @param layout Pointer to buffer to push vertex attributes into
	 */
	void getLayoutAttribs(std::vector<VertexAttribute>* layout) const;
};

