#pragma once

#include "../containers/SceneObject.h"
#include "../containers/Model.h"

/**
 * @brief A visual aid for debugging collision spheres in CollisionComponent
 *
 * @author Hayley Hatton
 * @date   13/03/2016
 * @see    CollisionComponent
 */
class DebuggingSphere : public SceneObject
{
public:
	/** @brief Default constructor */
	DebuggingSphere(
		Context* context,
		std::weak_ptr<MetaManager> managers,
		const glm::vec3& offset,
		float radius,
		bool isFarSphere);

	/** @brief Default destructor */
	~DebuggingSphere();


	/**
	 * @brief Draw the object in its current state
	 * @param context Graphics context
	 * @param shaders Shader manager containing shader programs for use
	 */
	void draw(Context* context, std::shared_ptr<ShaderManager> shaders) override;

	void setModelMatrix(const glm::mat4& mm);

private:
	std::shared_ptr<Model> m_Model;
	glm::mat4 m_ModelMatrix;
	glm::vec3 m_Offset;
	float m_Radius;
	bool m_IsFarSphere;
};
