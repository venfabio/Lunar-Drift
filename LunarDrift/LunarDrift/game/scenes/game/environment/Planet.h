#pragma once
#include "../../../../engine/containers/SceneObject.h"
#include "../../../../engine/containers/Model.h"
#include "../../../../engine/graphics/Texture.h"

/**
 * @brief Spinning planet for decorating scenes. Technically the moon at the moment. 
 *
 * @author Mariana
 * @date   26/05/2016
 * @ingroup Game
 */
class Planet : public SceneObject
{
public:
	/** @brief Default constructor */
	Planet(
		Context* context,
		std::weak_ptr<MetaManager> managers,
		const glm::vec3& position);

	/** @brief Default destructor */
	~Planet();

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

private:
	std::shared_ptr<Model> m_Model;
	std::unique_ptr<Texture> m_Texture;
};

