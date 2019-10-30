#include "DebuggingSphere.h"
#include "../containers/SceneObject.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

DebuggingSphere::DebuggingSphere(
	Context* context,
	std::weak_ptr<MetaManager> managers,
	const glm::vec3& offset,
	float radius,
	bool isFarSphere)
	: SceneObject(context, managers),
	  m_Offset(offset),
	  m_Radius(radius),
	  m_IsFarSphere(isFarSphere)
{
	m_Model = std::make_shared<Model>(context, "sphere");
}

DebuggingSphere::~DebuggingSphere()
{

}

void DebuggingSphere::draw(
	Context* context,
	std::shared_ptr<ShaderManager> shaders)
{
	std::shared_ptr<ShaderProgram> shader = shaders->get("collider");
	shader->use(context);

	GLint mmPtr = shader->getUniformLocation(context, "u_ModelMatrix");
	if (mmPtr != -1)
		glUniformMatrix4fv(mmPtr, 1, GL_FALSE, glm::value_ptr(m_ModelMatrix));

	glm::vec4 color;
	if (m_IsFarSphere)
		color = glm::vec4(0.f, 1.f, 0.f, 0.3f);
	else
		color = glm::vec4(1.f, 0.f, 0.f, 0.8f);
	GLint colorPtr = shader->getUniformLocation(context, "u_Color");
	if (colorPtr != -1)
		glUniform4fv(colorPtr, 1, glm::value_ptr(color));

	m_Model->draw(context, shader);
}

void DebuggingSphere::setModelMatrix(const glm::mat4& mm)
{
	m_ModelMatrix = glm::translate(mm, m_Offset);
	m_ModelMatrix = glm::scale(m_ModelMatrix, glm::vec3(m_Radius, m_Radius, m_Radius));
}