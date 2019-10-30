#pragma once

#include "Ship.h"
#include "../../../../engine/containers/SceneObject.h"
#include "../../../../engine/containers/Model.h"
#include "../../../../engine/graphics/Texture.h"
#include "../../../../engine/comms/IMessageListener.h"

/**
 * @brief Pink, smaller, faster and more agile ship
 *
 * @author Fabio
 * @date   20/05/2016
 * @ingroup Game
 */
class PinkiePie : public Ship
{
public:
	/** @brief Default constructor */
	PinkiePie(
		Context* context,
		std::weak_ptr<MetaManager> managers,
		const glm::vec3& position,
		bool isHardMode,
		bool isPlayer2 = false);

	/** @brief Default destructor */
	~PinkiePie();


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

	/**
	* @brief Called when a subscribed event is raised
	* @param event Smart pointer to the raised event object
	*/
	void onMessage(std::shared_ptr<Message> event) override;

protected:

private:
	std::shared_ptr<Model> m_Model;
	std::shared_ptr<Model> m_Outline;
	std::unique_ptr<Texture> m_Texture;

	void setupOutline(Context* context);

	void setupJets(Context* context, std::weak_ptr<MetaManager> managers);
};
