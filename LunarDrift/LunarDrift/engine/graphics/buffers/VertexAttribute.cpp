#include "VertexAttribute.h"
#include <cassert>

VertexAttribute::VertexAttribute(
    const std::string& semantic,
    GLsizeiptr offset,
    GLsizei stride,
    Type type,
    GLint count)
    : m_Semantic(semantic),
      m_Offset(offset),
      m_Stride(stride),
      m_Type(type),
      m_Count(count)
{
    assert(semantic.length() > 0);
    assert(stride > 0);
    assert(count > 0);
}

VertexAttribute::~VertexAttribute()
{
    //dtor
}

void VertexAttribute::bind(
    Context* context,
    std::shared_ptr<ShaderProgram> shader)
{
    GLenum errCode;

    assert(context);
    assert((errCode = glGetError()) == GL_NO_ERROR);

    const std::string prefix("in_");
    const std::string postfix("");
    const std::string shaderInput(prefix + m_Semantic + postfix);

    GLint index = shader->getAttributeLocation(context, shaderInput);
    if(index == -1)
        return;

    if((m_Type == Type::HalfFloat) || (m_Type == Type::Float))
        glVertexAttribPointer(
            index,
            m_Count,
            static_cast<GLenum>(m_Type),
            false,
            m_Stride,
            (const void*)(m_Offset));
    else if(m_Type == Type::Double)
        glVertexAttribLPointer(
            index,
            m_Count,
            static_cast<GLenum>(m_Type),
            m_Stride,
            (const void*)(m_Offset));
    else
        glVertexAttribIPointer(
            index,
            m_Count,
            static_cast<GLenum>(m_Type),
            m_Stride,
            (const void*)(m_Offset));

    glEnableVertexAttribArray(index);

    assert((errCode = glGetError()) == GL_NO_ERROR);
}
