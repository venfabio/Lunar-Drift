#pragma once

#include "../components/SceneObjectComponent.h"
#include "../graphics/Context.h"
#include "../managers/ShaderManager.h"

/**
 * @brief Base class for encapsulating self-management of scene entities
 *
 * @pre    A valid OpenGL context should be present to the program; else, IDeferredLoadable
 * @author Hayley Hatton
 * @date   06/03/2016
 * @ingroup TopLevelContainers
 */
class SceneObject
{
public:
    /**
     * @brief Default constructor
     * @param context Graphics context
     * @param managers Weak reference to scene's metamanager
     */
    SceneObject(Context* context, std::weak_ptr<MetaManager> managers);

    /** @brief Default destructor */
    virtual ~SceneObject();


    /** @brief Enumeration for scene object lifetime states */
    enum class State
    {
        NeedsDeferredLoading,   //!< @see IDeferredLoadable
        Ready,          //!< Loaded and ready for use
        Dying,          //!< Loaded, but marked as dead and needing cleanup
        Dead            //!< Cleaned up and finished; pending dtor
    };

    /**
     * @brief Retrieve the current lifetime stage the object is in
     * @return Current life-stage state
     */
    State getState() const { return m_State; }

    /** @brief Set the scene object to begin rendering */
    void setReady() { m_State = State::Ready; }

    /** @brief Set the scene object to begin the dying process */
	void setDying() { m_State = State::Dying; }

	/**
	 * @brief Add a component to the scene object
	 * @param component Smart pointer to the component
	 * @return True if added; false if already exists
	 */
	bool addComponent(std::shared_ptr<SceneObjectComponent> component);

	/**
	 * @brief Access a component attached to the scene object
	 * @param name Identifying name of the component type
	 * @return Smart pointer to the component; nullptr if none of that type
	 */
	std::shared_ptr<SceneObjectComponent> getComponent(const std::string& name);

	/**
	 * @brief Set the position of the entity within the scene
	 * @param pos New position
	 */
	void setPosition(const glm::vec3& pos) { m_Position = pos; }

	/**
	 * @brief Access the current position of the entity in the scene
	 * @return Current position
	 */
	const glm::vec3& getPosition() const { return m_Position; }

	/**
	 * @brief Set the pitch angle of the entity ("X-Axis Rotation")
	 * @param angle New pitch angle
	 */
	void setPitch(GLfloat angle) { m_Rotation.x = angle; }

	/**
	 * @brief Get the pitch angle of the entity ("X-Axis Rotation")
	 * @return Current pitch angle
	 */
	GLfloat getPitch() const { return m_Rotation.x; }

	/**
	 * @brief Set the yaw angle of the entity ("Y-Axis Rotation")
	 * @param angle New yaw angle
	 */
	void setYaw(GLfloat angle) { m_Rotation.y = angle; }

	/**
	 * @brief Get the yaw angle of the entity ("Y-Axis Rotation")
	 * @return Current yaw angle
	 */
	GLfloat getYaw() const { return m_Rotation.y; }

	/**
	 * @brief Set the roll angle of the entity ("Z-Axis Rotation")
	 * @param angle New roll angle
	 */
	void setRoll(GLfloat angle) { m_Rotation.z = angle; }

	/**
	 * @brief Get the roll angle of the entity ("Z-Axis Rotation")
	 * @return Current roll angle
	 */
	GLfloat getRoll() const { return m_Rotation.z; }

	/**
	 * @brief Set all the rotation states of the entity at once
	 * @param angles New rotation angles as a vector
	 */
	void setRotations(const glm::vec3& angles) { m_Rotation = angles; }

	/**
	 * @brief Get all the rotation states of the entity at once
	 * @return Current rotation angles as a vector
	 */
	const glm::vec3& getRotations() const { return m_Rotation; }

    /**
     * @brief Step the simulation state
     * @param dt Delta-time since last step call in seconds
     */
    virtual void step(double dt);

    /**
     * @brief Called before the renderer draws the scene
     * This allows the scene objects to do updates that require a
     * graphics context before the graphical state is drawn
     * @param context Graphics context
     */
    virtual void predraw(Context* context);

    /**
     * @brief Access the model matrix of the scene object
     * @param Pointer to a 4x4 matrix to store the model matrix
     */
    virtual void getModelMatrix(glm::mat4* mm) const;

	/**
	 * @brief Use the standard position and rotation states as a model matrix
	 * This constructs a model matrix representing the position and rotation
	 * state, and tries to upload it into the u_ModelMatrix uniform in the
	 * presented shader, taking away the effort of doing this on every object
	 * from the programmers
	 * @param context Graphics context
	 * @param shader Shader program to upload model matrix to
	 */
	virtual void updateModelMatrixState(
		Context* context,
		std::shared_ptr<ShaderProgram> shader) const;

    /**
     * @brief Draw the object in its current state
     * @param context Graphics context
     * @param shaders Shader manager containing shader programs for use
     */
    virtual void draw(
        Context* context,
        std::shared_ptr<ShaderManager> shaders) = 0;

protected:
    std::weak_ptr<MetaManager> m_Managers;

private:
    std::map<std::string, std::shared_ptr<SceneObjectComponent>> m_Components;
	glm::vec3 m_Position;
	glm::vec3 m_Rotation;
    State m_State;  //!< Current life-stage state
};
