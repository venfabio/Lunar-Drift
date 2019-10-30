#pragma once

#include "../containers/SceneObject.h"
#include "../containers/Model.h"
#include <vector>

/**
 * @brief Describes the particle system in terms of particle variables
 *
 * @see      ParticleSystem
 * @author   Hayley Hatton
 */
struct PARTICLE_SYSTEM_DESCRIPTION
{
	glm::vec3 position;
	glm::vec3 emissionDirection;
	float emissionRadius;
	unsigned int emissionRate;
	float speed;
	float speedVariance;
	float lifeTime;
	float lifeTimeVariance;
	glm::vec4 startColor;
	glm::vec4 decay;
	float decayTime;
	bool active;
};

/**
 * @brief Uniform state for the particle engine shaders
 *
 * @see      ParticleSystem
 * @author   Hayley Hatton
 */
struct PARTICLE_UNIFORMS
{
	glm::vec4 startColor;   //!< Color that particles start as
	glm::vec4 decay;        //!< Color changing rate over time
	float decayTime;        //!< Time over which the color decays
	float timestamp;        //!< Current time of the system
};

/**
 * @brief State of an individual particle in the particle system
 *
 * @see      SceneObject
 * @author   Hayley Hatton
 */
struct PARTICLE
{
	glm::vec3 position;     //!< Starting position
	glm::vec3 motionVector; //!< Where the particle is going
	float birthStamp;       //!< Time when the particle was born
	float speed;            //!< Speed of motion
	float lifeTime;         //!< How long the particle lives for
	float decayModifier;    //!< 
};

/**
 * @brief A generic particle system instance
 *
 * @see      PARTICLE
 * @see      PARTICLE_UNIFORMS
 * @see      PARTICLE_SYSTEM_DESCRIPTION
 * @author   Hayley Hatton
 * @ingroup Prefabs
 */
class ParticleSystem : public SceneObject
{
public:
	ParticleSystem(
		Context* context,
		std::weak_ptr<MetaManager> managers,
		std::unique_ptr<PARTICLE_SYSTEM_DESCRIPTION> desc);

	virtual ~ParticleSystem();


	void step(double dt) override;

	void predraw(Context* context) override; 
	
	void draw(Context* context, std::shared_ptr<ShaderManager> shaders) override;

	const PARTICLE_UNIFORMS& getUniforms() const { return m_Uniforms; }

	void setUniforms(const PARTICLE_UNIFORMS& uforms) { m_Uniforms = uforms; }

	void setPosition(const glm::vec3& pos) { m_Position = pos; }

	void setDirection(const glm::vec3& dir) { m_EmissionDirection = dir; }

	void setActive(bool active) { m_Active = active; }

	void toggleActive() { m_Active = !m_Active; }

private:
	PARTICLE_UNIFORMS m_Uniforms;

    std::vector<PARTICLE> m_Particles;
    std::unique_ptr<VertexArrayObject> m_VAO;

	glm::vec3 m_Position;       //!< Center of particle system origin
	glm::vec3 m_EmissionDirection;
	float m_EmissionRadius;      //!< 
	unsigned int m_EmissionRate;    //!< Particles emitted per second
	float m_Speed;              //!< Average speed of particles
	float m_SpeedVariance;      //!< Individual variation in speed
	float m_LifeTime;           //!< Average life span of particles (secs)
	float m_LifeTimeVariance;   //!< Individual variation in life durations
	float m_ParticleInterval;   //!< Time between particles (secs)
	float m_NextParticle;       //!< Time until next particle emits (secs)
	bool m_Active;

	void setUniforms(std::unique_ptr<PARTICLE_SYSTEM_DESCRIPTION> desc); 
	
	void applyUniforms(
		Context* context,
		std::shared_ptr<ShaderProgram> shader) const;

	void renewParticle(PARTICLE& particle);

	void createParticles(
        Context* context, 
        unsigned int numParticles,
        std::vector<PARTICLE>* particles);

	void updateParticles(double dt);

	void commitUpdates(Context* context);

	void getLayoutAttribs(std::vector<VertexAttribute>* attribs) const;

    void getPointInSphere(glm::vec3* point, float radius) const;
};
