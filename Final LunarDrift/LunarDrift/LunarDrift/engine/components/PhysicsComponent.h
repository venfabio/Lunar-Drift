#pragma once

#include "SceneObjectComponent.h"
#include <glm/glm.hpp>

/**
 * @brief Representation of a force
 */
struct Force
{
	glm::vec3 dir;		//!< Direction of the force
	glm::vec3 offset;	//!< Offset of the force from centre of gravity
	float force;		//!< Magnitude of the force in Newtons
};

/**
 * @brief Attach to a scene object to allow it to have physics enacted on it
 * @ingroup Components
 *
 * @author Hayley Hatton
 * @date   11/03/2016
 */
class PhysicsComponent : public SceneObjectComponent
{
public:
    /**
     * @brief Default constructor
     * @param parent Pointer to the scene object to attach component to
     * @param managers Metamanager for accessing scene's managers
     * @param mass Mass of the object in kg
     * @param initialVelocity Optional initial velocity (optional)
	 * @param disableTorque Emergency "pull to stop all torque" button! Use sparingly!
     */
	PhysicsComponent(
		SceneObject* parent, 
		std::weak_ptr<MetaManager> managers,
        float mass,
		const glm::vec3& initialVelocity = glm::vec3(0.f, 0.f, 0.f),
		bool disableTorque = false);

    /** @brief Default destructor */
	virtual ~PhysicsComponent();


    /**
     * @brief Override all previous physics and replace with a set velocity
     * @param v New velocity to set scene object on
     */
	void setVelocity(const glm::vec3& v) { m_Velocity = v; }

    /**
     * @brief Get the current velocity as a result of physical forces applied
     * @return Resultant velocity (current state)
     */
	const glm::vec3& getVelocity() const { return m_Velocity; }

	/**
	* @brief Override all previous physics and replace with a set angular velocity
	* @param v New angular velocity to set scene object on
	*/
	void setAngularVelocity(const glm::vec3& v) { m_AngularVelocity = v; }

	/**
	* @brief Get the current angular velocity as a result of physical forces applied
	* @return Resultant angular velocity (current state)
	*/
	const glm::vec3& getAngularVelocity() const { return m_AngularVelocity; }

    /**
    * @brief Override acceleration with one of your own
    * @param a New acceleration of object
    */
    void setAcceleration(const glm::vec3& a) { m_Acceleration = a; }

    /**
    * @brief Get the current acceleration as a result of summed physical forces
    * @return Current acceleration of object
    */
    const glm::vec3& getAcceleration() const { return m_Acceleration; }

    /**
     * @brief Apply a force to the object
     * @param force Force in Newtons
     * @param impulse True applies force for 1s; false applies until countered or removed
     * @param dir Direction to apply force
     * @param offset Where to apply the force (for torque calculations)
     * @param useLocalOffset If false, force applies relative to scene; if true, force applies relative to object
	 * @param id Identifier of the force for amending later (optional; -1 transforms into acceleration)
     */
    void applyForce(
        float force,
        bool impulse,
        const glm::vec3& dir,
        const glm::vec3& offset = glm::vec3(0.f, 0.f, 0.f),
        bool useLocalOffset = false,
		int id = -1);

	/**
	 * @brief Access a currently-acting, identified force
	 * @param id Pre-specified non-negative identification number of force
	 * @return Constant reference to force structure
	 */
	const Force& getForce(int id) const { return m_Forces.at(id); }

	/**
	 * @brief Manually set a currently-acting, identified force
	 * @param force Force description
	 * @param id Non-negative identification number for accessing force
	 */
	void setForce(const Force& force, int id) { m_Forces[id] = force; }

	/**
	 * @brief Remove a currently-acting, identified force
	 * @param id Non-negative identification number of force to remove
	 */
	void removeForce(int id) { m_Forces.erase(id); }

    /**
     * @brief Step the simulation state
     * @param dt Delta-time since last step call in seconds
     */
    void step(double dt) override;

    /**
     * @brief Called before the renderer draws the scene
     * This allows the scene objects to do updates that require a
     * graphics context before the graphical state is drawn
     * @param context Graphics context
     */
    void predraw(Context* context) override {};


private:
	std::map<int, Force> m_Forces;		//!< Currently-acting forces on the component
	glm::vec3 m_Velocity;				//!< Current resultant velocity (ms^-1)
    glm::vec3 m_Acceleration;			//!< Current enacting acceleration (ms^-2)
	glm::vec3 m_AngularVelocity;		//!< Current angular velocity (rads^-1)
	glm::vec3 m_AngularAcceleration;	//!< Current angular acceleration (rads^-2)
    float m_Mass;						//!< Mass of the object in kg
	bool m_DisableTorque;	//!< If true, goodbye torque! Only linear motion allowed now!

	/**
	 * @brief Get appropriate components of sum m_Forces
	 * @param linearAcceleration Pointer to vec3 to receive sum of linear forces
	 * @param angularAcceleration Pointer to vec3 to receive sum of angular forces
	 */
	void aggregateForces(
		glm::vec3* linearAcceleration, 
		glm::vec3* angularAcceleration) const;

};
