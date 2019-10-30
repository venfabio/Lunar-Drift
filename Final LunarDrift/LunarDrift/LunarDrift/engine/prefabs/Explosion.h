#pragma once

#include "ParticleSystem.h"
#include "../comms/IMessageListener.h"
#include "../graphics/IDeferredLoadable.h"

/**
 * @brief Structure for parameterizing the construction of the Explosion
 */
struct EXPLOSION_DESCRIPTION
{
	glm::vec3 offset;           //!< Position of explosion in scene
	unsigned int numParticles;  
	float originRadius;         //!< Radius of initial particle cloud
	float speed;        //!< Average distance travelled of a particle per second
	float speedVariance;        //!< Variance in generated speeds
	float lifeTime;     //!< Average lifespan of a particle in seconds
	float lifeTimeVariance;     //!< Variance in particle lifespans
	glm::vec4 startColor;   //!< Initial color of particles
	glm::vec4 decay;        //!< Decay rate of particles
	float decayTime;        //!< Time over which to interpolate color decay
};

/**
 * @defgroup Prefabs
 * @brief Prefabricated objects for use in the game; prevents having to reinvent the wheel for things like text and particle effects
 */

/**
 * @brief Prefabricated explosion object
 *
 * @author Hayley Hatton
 * @date   18/03/2016
 * @see    ExplodableComponent
 * @ingroup Prefabs
 */
class Explosion : public SceneObject, 
                  public IMessageListener,
                  public IDeferredLoadable
{
public:
    /**
     * @brief managers Weakly-managed pointer to the scene's metamanager
     * @param desc Unique_ptr to struct for describing the explosion
     */
	Explosion(
        std::weak_ptr<MetaManager> managers, 
        std::unique_ptr<EXPLOSION_DESCRIPTION> desc);

    /** @brief Default destructor */
	virtual ~Explosion();


    ///////////////////////////////////////////////////////////////////////////
    // IDeferredLoadable
    ///////////////////////////////////////////////////////////////////////////
    /**
     * @brief Called to load the object
     * @param context Graphics context
     */
    void load(Context* context) override;

    /**
     * @brief Called to destroy the object
     * @param context Graphics context
     */
    void unload(Context* context) override;

    ///////////////////////////////////////////////////////////////////////////
    // SceneObject
    ///////////////////////////////////////////////////////////////////////////
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

    ///////////////////////////////////////////////////////////////////////////
    // IMessageListener
    ///////////////////////////////////////////////////////////////////////////
    /**
     * @brief Called when a message is sent to this entity
     * @param msg Smart pointer to the received message object
     */
	void onMessage(std::shared_ptr<Message> msg) override;

private:
	PARTICLE_UNIFORMS m_Uniforms;

    std::unique_ptr<VertexArrayObject> m_VAO;

	glm::vec3 m_Offset;             //!< Position of explosion in scene
	unsigned int m_NumParticles;
	float m_OriginRadius;           //!< Radius of initial particle cloud
	float m_Speed;  //!< Average distance travelled of a particle per second
	float m_SpeedVariance;          //!< Variance in particle lifespans
	float m_LifeTime;           //!< Average lifespan of a particle in seconds
	float m_LifeTimeVariance;       //!< Variance in particle lifespans
	glm::vec4 m_StartColor;         //!< Initial color of particles
	glm::vec4 m_Decay;              //!< Decay rate of particles
	float m_DecayTime;              //!< Time over which to interpolate color decay

    /**
     * @brief Set the uniform state from initializer
     * @param Initializer buffer; will die after this function!
     */
	void setUniforms(std::unique_ptr<EXPLOSION_DESCRIPTION> desc);

    /**
     * @brief Apply the uniforms to the shader pipeline
     * @param context Graphics context
     * @param shader Shader to apply uniforms to
     */
    void applyUniforms(
        Context* context,
        std::shared_ptr<ShaderProgram> shader) const;

    /**
     * @brief Create exploding particles
     * @param particles Pointer to buffer to push particles into
     */
	void createParticles(std::vector<PARTICLE>* particles);

    /**
     * @brief Get vertex attributes for mapping vertex data to shader pipeline
     * @param layout Pointer to buffer to push vertex attributes into
     */
	void getLayoutAttribs(std::vector<VertexAttribute>* layout) const;

    /**
     * @brief Generate a random point in a uniform sphere of given radius
     * @param point Pointer to point position
     * @param radius Radius in GL units
     */
	void getPointInSphere(glm::vec3* point, float radius) const;
};
