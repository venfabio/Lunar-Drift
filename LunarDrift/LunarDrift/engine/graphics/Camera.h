#pragma once

#include "../graphics/shaders/ShaderProgram.h"
#include "../graphics/shaders/ShaderUniformBlock.h"
#include <glm/glm.hpp>

/**
 * @brief Describes a camera for viewing a scene
 *
 * @author Hayley Hatton
 * @date   07/03/2016
 * @ingroup Graphics
 */
class Camera
{
public:
    /**
     * @brief Create a camera with an orthogonal lens
     * @param width Width of the viewport
     * @param height Height of the viewport
     * @param nearZ Near-plane Z coordinate
     * @param farZ Far-plane Z coordinate
     */
    Camera(GLuint width, GLuint height, GLfloat nearZ, GLfloat farZ);

    /**
     * @brief Create a camera with a perspective lens
     * @param width Width of the viewport
     * @param height Height of the viewport
     * @param nearZ Near-plane Z coordinate
     * @param farZ Far-plane Z coordinate
     * @param fovy Field-of-view, in degrees
     */
    Camera(
        GLuint width, GLuint height,
        GLfloat nearZ, GLfloat farZ,
        GLfloat fovy);

    /** @brief Default destructor */
    virtual ~Camera();


    /**
     * @brief Set an orthogonal lens for the camera
     * @param width Width of the viewport
     * @param height Height of the viewport
     * @param nearZ Near-plane Z coordinate
     * @param farZ Far-plane Z coordinate
     */
    void setOrthogonal(
        GLuint width, GLuint height,
        GLfloat nearZ, GLfloat farZ);

    /**
     * @brief Set a perspective lens for the camera
     * @param width Width of the viewport
     * @param height Height of the viewport
     * @param nearZ Near-plane Z coordinate
     * @param farZ Far-plane Z coordinate
     * @param fovy Field-of-view, in degrees
     */
    void setPerspective(
        GLuint width, GLuint height,
        GLfloat nearZ, GLfloat farZ,
        GLfloat fovy);

    /**
     * @brief Set the camera in the scene using classic "look-at" descriptors
     * @param eye Position of the camera in the scene
     * @param focus Where the camera is looking at in the scene
     * @param up Up-vector defining the orientation of the camera
     */
    void lookAt(
        const glm::vec3& eye,
        const glm::vec3& focus,
        const glm::vec3& up);

    /**
     * @brief Pan the camera
     * @param dp Delta-position
     */
	void pan(const glm::vec3& dp);

	/**
	 * @brief Rotate through the Y-Z plane ("X-axis"/"pitch")
	 * @param theta Angle in degrees through which to rotate the camera
	 */
	void rotateX(GLfloat theta);

    /**
     * @brief Rotate through the X-Z plane ("Y-axis"/"yaw")
     * @param theta Angle in degrees through which to rotate the camera
     */
    void rotateY(GLfloat theta);

    /**
     * @brief Rotate through the X-Y plane ("Z-axis"/"roll")
     * @param theta Angle in degrees through which to rotate the camera
     */
    void rotateZ(GLfloat theta);

    /**
     * @brief Access the position of the camera
     * @return 3-component vector of the position in space
     */
    const glm::vec3& getPosition() const;

    /**
     * @brief Access the current view matrix state
     * @return View matrix (4x4)
     */
    const glm::mat4& getViewMatrix() const { return m_ViewMatrix; }

    /**
     * @brief Access the current projection matrix state
     * @return Projection matrix (4x4)
     */
    const glm::mat4& getProjectionMatrix() const { return m_ProjMatrix; }

    /**
     * @brief Upload the camera matrices to a given shader program's uniforms
     * @param context Graphics context
     * @param shader Ptr to the shader program to upload matrices to
     */
    void apply(Context* context, ShaderProgram* shader) const;

    /**
     * @brief Upload the camera matrices to a uniform buffer across all shaders
     * @param context Graphics context
     * @param buffer Ptr to the uniform buffer to upload matrices to
     */
    void apply(Context* context, ShaderUniformBlock* buffer) const;

private:
    glm::mat4 m_ViewMatrix; //!< Current view matrix state
    glm::mat4 m_ProjMatrix; //!< Current projection matrix state
};
