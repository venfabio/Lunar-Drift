#include "IndexBufferObject.h"
#include "../../exceptions/OpenGLException.h"
#include <cassert>

IndexBufferObject::IndexBufferObject(
	Context* context,
	const std::vector<unsigned short>& indices)
    : m_Indices(indices)
{
    GLenum errCode;

    assert(context);
    assert((errCode = glGetError()) == GL_NO_ERROR);

    glGenBuffers(1, &m_IBO);
    if((errCode = glGetError()) != GL_NO_ERROR)
        throw OpenGLException("Could not generate index buffer", errCode);

    bind(context);

	const GLsizeiptr szBuffer = indices.size() * sizeof(unsigned short);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		szBuffer,
		static_cast<const GLvoid*>(&indices[0]),
		GL_STATIC_DRAW);
	if ((errCode = glGetError()) != GL_NO_ERROR)
		throw OpenGLException("Could not buffer index data", errCode);
}

IndexBufferObject::~IndexBufferObject()
{
    glDeleteBuffers(1, &m_IBO);
}


void IndexBufferObject::bind(Context* context)
{
    GLenum errCode;

    assert(context);
    assert((errCode = glGetError()) == GL_NO_ERROR);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    if((errCode = glGetError()) != GL_NO_ERROR)
        throw OpenGLException("Could not bind index buffer object", errCode);
}
