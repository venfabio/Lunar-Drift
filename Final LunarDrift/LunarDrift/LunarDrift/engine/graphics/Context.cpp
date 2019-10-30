#include "Context.h"

Context::Context()
{
    //ctor
}

Context::~Context()
{
    //dtor
}


void Context::setDimensions(GLsizei width, GLsizei height)
{
    m_Width = width;
    m_Height = height;
}

void Context::getDimensions(GLsizei* width, GLsizei* height) const
{
    *width = m_Width;
    *height = m_Height;
}
