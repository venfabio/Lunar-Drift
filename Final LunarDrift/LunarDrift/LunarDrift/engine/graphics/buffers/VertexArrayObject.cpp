#include "VertexArrayObject.h"
#include "../../exceptions/OpenGLException.h"
#include <cassert>

VertexArrayObject::VertexArrayObject(
    Context* context,
    std::shared_ptr<VertexBufferObjectBase> vbo,
    const std::vector<VertexAttribute>& attributes,
    std::shared_ptr<IndexBufferObject> ibo)
    : m_Attributes(attributes),
      m_VertexBuffer(vbo),
      m_IndexBuffer(ibo)
{
    GLenum errCode;

    assert(context);
    assert(vbo.get());
    assert(attributes.size() > 0);
    assert((errCode = glGetError()) == GL_NO_ERROR);

    glGenVertexArrays(1, &m_VAO);
    if((errCode = glGetError()) != GL_NO_ERROR)
        throw OpenGLException("Could not generate VAO", errCode);

    glBindVertexArray(m_VAO);
    if((errCode = glGetError()) != GL_NO_ERROR)
        throw OpenGLException("Could not bind vertex array object", errCode);
}

VertexArrayObject::~VertexArrayObject()
{
    glDeleteVertexArrays(1, &m_VAO);
}


void VertexArrayObject::setAttributeMap(
    const std::vector<VertexAttribute>& attributes)
{
    m_Attributes = attributes;
}

void VertexArrayObject::setVertexBuffer(
    std::shared_ptr<VertexBufferObjectBase> vbo)
{
    m_VertexBuffer = vbo;
}

void VertexArrayObject::setIndexBuffer(
    std::shared_ptr<IndexBufferObject> ibo)
{
    m_IndexBuffer = ibo;
}

void VertexArrayObject::bind(
    Context* context,
    std::shared_ptr<ShaderProgram> shader)
{
    GLenum errCode;

    assert(context);
    assert((errCode = glGetError()) == GL_NO_ERROR);

    shader->use(context);

    glBindVertexArray(m_VAO);
    if((errCode = glGetError()) != GL_NO_ERROR)
        throw OpenGLException("Could not bind vertex array object", errCode);

    m_VertexBuffer->bind(context);
    if (m_IndexBuffer.get() != nullptr)
        m_IndexBuffer->bind(context);
    else
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    for(VertexAttribute& attribute: m_Attributes)
        attribute.bind(context, shader);

    assert((errCode = glGetError()) == GL_NO_ERROR);
}
