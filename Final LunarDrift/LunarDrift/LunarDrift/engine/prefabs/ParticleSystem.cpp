#include "ParticleSystem.h"
#include "../managers/ShaderManager.h"
#include "../managers/SceneObjectManager.h"
#include "../MathHelper.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ctime>


ParticleSystem::ParticleSystem(
	Context* context,
	std::weak_ptr<MetaManager> managers,
	std::unique_ptr<PARTICLE_SYSTEM_DESCRIPTION> desc) 
    : SceneObject(context, managers),
	  m_Position(desc->position),
	  m_EmissionDirection(desc->emissionDirection),
	  m_EmissionRadius(desc->emissionRadius),
	  m_EmissionRate(desc->emissionRate),
	  m_Speed(desc->speed),
	  m_SpeedVariance(desc->speedVariance),
	  m_LifeTime(desc->lifeTime),
	  m_LifeTimeVariance(desc->lifeTimeVariance),
	  m_ParticleInterval(1.f / static_cast<float>(desc->emissionRate)),
	  m_NextParticle(m_ParticleInterval),
	  m_Active(desc->active)
{
    setUniforms(std::move(desc));

    std::vector<VertexAttribute> attribs;
    getLayoutAttribs(&attribs);

    createParticles(
        context,
        static_cast<unsigned int>(m_EmissionRate * m_LifeTime),
        &m_Particles);

    std::shared_ptr<VertexBufferObjectBase> vbo =
        std::make_shared<VertexBufferObject<PARTICLE>>(context, m_Particles, false);
    m_VAO = std::make_unique<VertexArrayObject>(context, vbo, attribs);
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::step(double dt)
{
	updateParticles(dt);
	SceneObject::step(dt);
}

void ParticleSystem::predraw(Context* context)
{
	commitUpdates(context);
	SceneObject::predraw(context);
}

void ParticleSystem::draw(
	Context* context,
	std::shared_ptr<ShaderManager> shaders)
{
	assert(context);
	assert(shaders.get());

	std::shared_ptr<ShaderProgram> shader = shaders->get("particle_system");
	shader->use(context);
	applyUniforms(context, shader);

	m_VAO->bind(context, shader);
	glDrawArrays(
		static_cast<GLenum>(PrimitiveMode::Points),
		0,
		m_VAO->getVertexBuffer()->size());
}

void ParticleSystem::setUniforms(
    std::unique_ptr<PARTICLE_SYSTEM_DESCRIPTION> desc)
{
	m_Uniforms.startColor = desc->startColor;
	m_Uniforms.decayTime = desc->decayTime;
	m_Uniforms.decay = desc->decay;
	m_Uniforms.timestamp = 0.f;
}

void ParticleSystem::applyUniforms(
	Context* context,
	std::shared_ptr<ShaderProgram> shader) const
{
	GLint startColor = shader->getUniformLocation(context, "u_StartColor");
	GLint decayTime = shader->getUniformLocation(context, "u_DecayTime");
	GLint decay = shader->getUniformLocation(context, "u_Decay");
	GLint timestamp = shader->getUniformLocation(context, "u_Timestamp");

	if (startColor != -1)
		glUniform4fv(startColor, 1, glm::value_ptr(m_Uniforms.startColor));
	if (decayTime != -1)
		glUniform1f(decayTime, m_Uniforms.decayTime);
	if (decay != -1)
		glUniform4fv(decay, 1, glm::value_ptr(m_Uniforms.decay));
	if (timestamp != -1)
		glUniform1f(timestamp, m_Uniforms.timestamp);
}

void ParticleSystem::renewParticle(PARTICLE& particle)
{
	if (m_Active)
	{
		glm::vec3 offset;
		getPointInSphere(&offset, m_EmissionRadius);

		glm::vec3 dir = m_EmissionDirection;

		float varianceStep = m_LifeTimeVariance * 0.5f;
		float life = m_LifeTime + randf(-varianceStep, varianceStep);

		float speedStep = m_SpeedVariance * 0.5f;
		float speed = m_Speed + randf(-speedStep, speedStep);

		particle.position = m_Position + offset;
		particle.motionVector = dir;
		particle.birthStamp = m_Uniforms.timestamp;
		particle.lifeTime = life;
		particle.speed = speed;
		particle.decayModifier = 1.f;
	}
}

void ParticleSystem::createParticles(
	Context* context,
	unsigned int numParticles,
    std::vector<PARTICLE>* particles) 
{
	particles->resize(numParticles);

	for (PARTICLE& particle : *particles)
		particle.birthStamp = -1.f;
}

void ParticleSystem::updateParticles(double dt)
{
	const float dtf = static_cast<float>(dt);
	m_Uniforms.timestamp += dtf;

	unsigned int numNewParticles = 0;
	m_NextParticle -= dtf;
	if (m_NextParticle <= 0.f)
	{
		++numNewParticles;
		while (m_NextParticle < -m_ParticleInterval)
		{
			++numNewParticles;
			m_NextParticle += m_ParticleInterval;
		}

		m_NextParticle = m_ParticleInterval;
	}


	for (PARTICLE& particle : m_Particles)
	{
		if ((particle.birthStamp + particle.lifeTime) < m_Uniforms.timestamp
			|| particle.birthStamp < 0.f)
		{
			if (numNewParticles > 0)
			{
				renewParticle(particle);
				--numNewParticles;
			}
			else {
				particle.birthStamp = -1.f;
			}
		}

		glm::vec3& pos = particle.position;
		pos.x += particle.motionVector.x * particle.speed * dtf;
		pos.y += particle.motionVector.y * particle.speed * dtf;
		pos.z += particle.motionVector.z * particle.speed * dtf;
	}
}

void ParticleSystem::commitUpdates(Context* context)
{
    std::shared_ptr<VertexBufferObject<PARTICLE>> vbo =
        std::static_pointer_cast<VertexBufferObject<PARTICLE>>(m_VAO->getVertexBuffer());
    vbo->setVertices(m_Particles);
    vbo->resynchronize(context);
}

void ParticleSystem::getLayoutAttribs(
    std::vector<VertexAttribute>* attribs) const
{
    attribs->push_back(VertexAttribute(
        "Position",
        offsetof(PARTICLE, position),
        sizeof(PARTICLE),
        VertexAttribute::Type::Float,
        3));
    attribs->push_back(VertexAttribute(
        "Direction",
        offsetof(PARTICLE, motionVector),
        sizeof(PARTICLE),
        VertexAttribute::Type::Float,
        3));
    attribs->push_back(VertexAttribute(
        "BirthStamp",
        offsetof(PARTICLE, birthStamp),
        sizeof(PARTICLE),
        VertexAttribute::Type::Float,
        1));
    attribs->push_back(VertexAttribute(
        "Speed",
        offsetof(PARTICLE, speed),
        sizeof(PARTICLE),
        VertexAttribute::Type::Float,
        1));
    attribs->push_back(VertexAttribute(
        "Life",
        offsetof(PARTICLE, lifeTime),
        sizeof(PARTICLE),
        VertexAttribute::Type::Float,
        1));
    attribs->push_back(VertexAttribute(
        "DecayRate",
        offsetof(PARTICLE, decayModifier),
        sizeof(PARTICLE),
        VertexAttribute::Type::Float,
        1));
}

void ParticleSystem::getPointInSphere(glm::vec3* point, float radius) const
{
    // http://math.stackexchange.com/questions/87230/picking-random-points-in-the-volume-of-sphere-with-uniform-probability
    const double pi = 3.14159265359;
    const double tau = 6.28318530718;

    float theta = randf(0.f, static_cast<float>(tau));
    float v = randf(0.f, 1.f);
    float phi = acos(2 * v - 1);
    float r = radius * pow(randf(0.f, 1.f), 1.f / 3.f);

    *point = glm::vec3(
        r * cos(theta) * sin(phi),
        r * sin(theta) * sin(phi),
        r * cos(phi));
}
