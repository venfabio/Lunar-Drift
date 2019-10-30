#include "PhysicsComponent.h"
#include "../containers/SceneObject.h"


PhysicsComponent::PhysicsComponent(
	SceneObject* parent,
    std::weak_ptr<MetaManager> managers,
    float mass,
	const glm::vec3& initialVelocity,
	bool disableTorque) 
    : SceneObjectComponent("physics", parent, managers),
	  m_Velocity(initialVelocity),
      m_Acceleration(0.f, 0.f, 0.f),
	  m_AngularVelocity(0.f, 0.f, 0.f),
	  m_AngularAcceleration(0.f, 0.f, 0.f),
      m_Mass(mass),
	  m_DisableTorque(disableTorque)
{
}

PhysicsComponent::~PhysicsComponent()
{
}


void PhysicsComponent::applyForce(
    float force,
    bool impulse,
    const glm::vec3& dir,
    const glm::vec3& offset,
    bool useLocalOffset,
	int id)
{
	if (id >= 0)
	{
		m_Forces[id] = Force{ dir, offset, force };
		return;
	}

	// Linear component
    if (impulse)
    {
        m_Velocity += (dir * force) / m_Mass;
    }
    else 
    {
        m_Acceleration += (dir * force) / m_Mass;
    }

	if (m_DisableTorque)	// Emergency "pull to disable torque" button! 
		// Due to branch prediction, this should not affect performance in normal circumstances :)
		return;

	// Rotational component
	glm::vec3 off;
	if (useLocalOffset)
		off = offset;
	else
		off = offset - getParent()->getPosition();

	glm::vec3 torque = glm::cross(dir * force, off);
	m_AngularAcceleration += torque / m_Mass;
}

void PhysicsComponent::step(double dt)
{
	// Establish accelerational data from state and forces
	glm::vec3 acceleration(m_Acceleration);
	glm::vec3 angularAcceleration(m_AngularAcceleration);
	aggregateForces(&acceleration, &angularAcceleration);
	angularAcceleration.z = 0.f;

	// Linear motion
    m_Velocity += acceleration * static_cast<float>(dt);

	glm::vec3 pos = getParent()->getPosition();
	pos += m_Velocity * static_cast<float>(dt);
	getParent()->setPosition(pos);

	if (m_DisableTorque)	// Emergency "pull to disable torque" button! 
		// Due to branch prediction, this should not affect performance in normal circumstances :)
		return;

	// Angular motion
	m_AngularVelocity += angularAcceleration * static_cast<float>(dt);

	glm::vec3 rotation = getParent()->getRotations();
	rotation += m_AngularVelocity * static_cast<float>(dt);
	getParent()->setRotations(rotation);
}


void PhysicsComponent::aggregateForces(
	glm::vec3* linearAcceleration,
	glm::vec3* angularAcceleration) const
{
	std::map<int, Force>::const_iterator itr;
	for (itr = m_Forces.begin(); itr != m_Forces.end(); ++itr)
	{
		if (linearAcceleration != nullptr)
		{
			if (glm::length(itr->second.offset) < 0.1f)
				*linearAcceleration += (itr->second.dir * itr->second.force) / m_Mass;
		}

		if (angularAcceleration != nullptr)
		{
			glm::vec3 torque = glm::cross(
				itr->second.dir * itr->second.force, 
				itr->second.offset);
			*angularAcceleration += torque / m_Mass;
		}
	}
}