#include "VertexBufferObjectBase.h"
#include "../../exceptions/OpenGLException.h"
#include <cassert>

VertexBufferObjectBase::VertexBufferObjectBase(
    Context* context, 
    bool isConstant)
    : m_IsConstant(isConstant)
{
    GLenum errCode;

    assert(context);
    assert((errCode = glGetError()) == GL_NO_ERROR);

    glGenBuffers(1, &m_VBO);
    if((errCode = glGetError()) != GL_NO_ERROR)
        throw OpenGLException("Could not generate vertex buffer", errCode);

    bind(context);

    assert((errCode = glGetError()) == GL_NO_ERROR);
}

VertexBufferObjectBase::~VertexBufferObjectBase()
{
    glDeleteBuffers(1, &m_VBO);
}


void VertexBufferObjectBase::bind(Context* context)
{
    GLenum errCode;

    assert(context);
    assert((errCode = glGetError()) == GL_NO_ERROR);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    if((errCode = glGetError()) != GL_NO_ERROR)
        throw OpenGLException("Could not bind vertex buffer object", errCode);
}
