#include "Explosion.h"
#include "../managers/ShaderManager.h"
#include "../managers/SceneObjectManager.h"
#include "../components/LifeTimeComponent.h"
#include "../comms/MessageSystem.h"
#include "../comms/predefined/DeathMessage.h"
#include "../MathHelper.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ctime>


Explosion::Explosion(
    std::weak_ptr<MetaManager> managers, 
    std::unique_ptr<EXPLOSION_DESCRIPTION> desc) 
  : SceneObject(nullptr, managers),
	m_Offset(desc->offset),
	m_NumParticles(desc->numParticles),
	m_OriginRadius(desc->originRadius),
	m_Speed(desc->speed),
	m_SpeedVariance(desc->speedVariance),
	m_LifeTime(desc->lifeTime),
	m_LifeTimeVariance(desc->lifeTimeVariance),
	m_StartColor(desc->startColor),
	m_Decay(desc->decay),
	m_DecayTime(desc->decayTime)
{
    setUniforms(std::move(desc));

	MessageSystem::getInstance().subscribe("Death", this);
}

Explosion::~Explosion()
{
    MessageSystem::getInstance().unsubscribe("Death", this);
}


void Explosion::load(Context* context)
{
    std::vector<PARTICLE> particles;
    createParticles(&particles);

    std::vector<VertexAttribute> attribs;
    getLayoutAttribs(&attribs);

    std::shared_ptr<VertexBufferObjectBase> vbo =
        std::make_shared<VertexBufferObject<PARTICLE>>(context, particles);
    m_VAO = std::make_unique<VertexArrayObject>(context, vbo, attribs);

    double life = m_LifeTime + (m_LifeTimeVariance * 0.5f);
    addComponent(std::make_shared<LifeTimeComponent>(this, m_Managers, life));

    setReady();
}

void Explosion::unload(Context* context)
{
    // Empty
}

void Explosion::step(double dt)
{
    m_Uniforms.timestamp += static_cast<float>(dt);
	//updateParticles(dt);
	SceneObject::step(dt);
}

void Explosion::predraw(Context* context)
{
	//commitUpdates(context);
    SceneObject::predraw(context);
}

void Explosion::draw(
    Context* context, 
    std::shared_ptr<ShaderManager> shaders)
{
    assert(context);
    assert(shaders.get());

    std::shared_ptr<ShaderProgram> shader = shaders->get("explosion");
    shader->use(context);
    applyUniforms(context, shader);

    m_VAO->bind(context, shader);
    glDrawArrays(
        static_cast<GLenum>(PrimitiveMode::Points),
        0,
        m_VAO->getVertexBuffer()->size());
}

void Explosion::onMessage(std::shared_ptr<Message> msg)
{
	if (msg->getType().compare("Death") == 0)
	{
		std::shared_ptr<DeathMessage> dMsg = 
            std::static_pointer_cast<DeathMessage>(msg);
		if (dMsg->getDoomed() == this)
			setDying();
	}
}

void Explosion::setUniforms(std::unique_ptr<EXPLOSION_DESCRIPTION> desc)
{
	m_Uniforms.startColor = desc->startColor;
	m_Uniforms.decayTime = desc->decayTime;
	m_Uniforms.decay = desc->decay;
	m_Uniforms.timestamp = 0.f;
}

void Explosion::applyUniforms(
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

void Explosion::createParticles(std::vector<PARTICLE>* particles) 
{
    particles->resize(m_NumParticles);

	for (PARTICLE& particle : *particles)
	{
		glm::vec3 pos;
		getPointInSphere(&pos, m_OriginRadius);
		pos.x += getPosition().x + m_Offset.x;
        pos.y += getPosition().y + m_Offset.y;
        pos.z += getPosition().z + m_Offset.z;

		float angleLong = randf(0.f, 360.f);
		float angleLongR = DEG2RAD(angleLong);
        float angleLat = randf(-90.f, 90.f);
        float angleLatR = DEG2RAD(angleLat);
        glm::vec3 motionVector;
        motionVector.x = sin(angleLongR) * cos(angleLat);
        motionVector.y = sin(angleLat);
        motionVector.z = cos(angleLongR) * cos(angleLat);

		float varianceStep = m_LifeTimeVariance * 0.5f;
		float life = m_LifeTime + randf(-varianceStep, varianceStep);

		float speedStep = m_SpeedVariance * 0.5f;
		float speed = m_Speed + randf(-speedStep, speedStep);

		particle.position = pos;
        particle.motionVector = motionVector;
		particle.birthStamp = m_Uniforms.timestamp;
		particle.lifeTime = life;
		particle.speed = speed;
		particle.decayModifier = randf(0.3f, 1.f);
	}
}

void Explosion::getLayoutAttribs(std::vector<VertexAttribute>* attribs) const
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

void Explosion::getPointInSphere(glm::vec3* point, float radius) const
{
	// http://math.stackexchange.com/questions/87230/picking-random-points-in-the-volume-of-sphere-with-uniform-probability
    const double pi = 3.14159265359;
    const double tau = 6.28318530718;

    float theta = randf(0.f, static_cast<float>(tau));
    float v = randf(0.f, 1.f);
    float phi = acos(2 * v - 1);
    float r = radius * pow(randf(0.f, 1.f), 1.f / 3.f);

    *point =  glm::vec3(
        r * cos(theta) * sin(phi),
        r * sin(theta) * sin(phi),
        r * cos(phi));
}
