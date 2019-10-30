#pragma once
// Note by Fabio: should it become a prefab?
#include "../../../../engine/containers/SceneObject.h"
#include "../../../../engine/containers/Model.h"
#include "../../../../engine/graphics/Texture.h"
#include "../../../../engine/comms/IMessageListener.h"
#include "../ships/Ship.h"

/**
 * @brief Blue hoop; bonus to scores
 *
 * @author Fabio
 * @date   26/05/2016
 * @ingroup Game
 */
class HoopBlue : public SceneObject, public IMessageListener
{
public:
	HoopBlue(
		Context* context,
		std::weak_ptr<MetaManager> managers,
		const glm::vec3& position,
		const glm::vec2& rotation);

	~HoopBlue();

	void predraw(Context* context) override;
	virtual void draw(Context* context, std::shared_ptr<ShaderManager> shaders) override;
	virtual void onMessage(std::shared_ptr<Message> event) override;

protected:
	void setModel(std::shared_ptr<Model> model) { m_Model = model; }

private:
	std::shared_ptr<Model> m_Model;
	std::unique_ptr<Texture> m_Texture;
	std::unique_ptr<Texture> m_TextureAlt;

	bool crossed;
};