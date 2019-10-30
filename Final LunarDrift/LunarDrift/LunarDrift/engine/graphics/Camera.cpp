#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(
    GLuint width, GLuint height,
    GLfloat nearZ, GLfloat farZ)
{
    setOrthogonal(width, height, nearZ, farZ);
}

Camera::Camera(
    GLuint width, GLuint height,
    GLfloat nearZ, GLfloat farZ,
    GLfloat fovy)
{
    setPerspective(width, height, nearZ, farZ, fovy);
}

Camera::~Camera()
{
    //dtor
}


void Camera::setOrthogonal(
    GLuint width, GLuint height,
    GLfloat nearZ, GLfloat farZ)
{
    m_ProjMatrix = glm::ortho(
        0.f, static_cast<GLfloat>(width),
        static_cast<GLfloat>(height), 0.f,
        nearZ, farZ);
}

void Camera::setPerspective(
    GLuint width, GLuint height,
    GLfloat nearZ, GLfloat farZ,
    GLfloat fovy)
{
    const GLfloat pi = 3.141592653589793f;
    const GLfloat fov = fovy / 360.f * pi;
    GLfloat h = tan(fov) * nearZ;
    GLfloat w = h * (width / static_cast<GLfloat>(height));
    m_ProjMatrix = glm::frustum(-w, w, -h, h, nearZ, farZ);
}

void Camera::lookAt(
    const glm::vec3& eye,
    const glm::vec3& focus,
    const glm::vec3& up)
{
    m_ViewMatrix = glm::lookAt(eye, focus, up);
}

void Camera::pan(const glm::vec3& dp)
{
    m_ViewMatrix = glm::translate(m_ViewMatrix, dp);
}

void Camera::rotateX(GLfloat theta)
{
	m_ViewMatrix = glm::rotate(m_ViewMatrix, theta, glm::vec3(1.f, 0.f, 0.f));
}

void Camera::rotateY(GLfloat theta)
{
    m_ViewMatrix = glm::rotate(m_ViewMatrix, theta, glm::vec3(0.f, 1.f, 0.f));
}

void Camera::rotateZ(GLfloat theta)
{
    m_ViewMatrix = glm::rotate(m_ViewMatrix, theta, glm::vec3(0.f, 0.f, 1.f));
}

void Camera::apply(Context* context, ShaderProgram* shader) const
{
    assert(context);
    assert(shader);

    shader->use(context);

    GLint vm = shader->getUniformLocation(context, "u_ViewMatrix");
    GLint pm = shader->getUniformLocation(context, "u_ProjMatrix");

    if(vm != -1)
        glUniformMatrix4fv(vm, 1, GL_FALSE, glm::value_ptr(m_ViewMatrix));
    if(pm != -1)
        glUniformMatrix4fv(pm, 1, GL_FALSE, glm::value_ptr(m_ProjMatrix));

    GLenum errCode;
    assert((errCode = glGetError()) == GL_NO_ERROR);
}

void Camera::apply(Context* context, ShaderUniformBlock* buffer) const
{
    assert(context);
}
