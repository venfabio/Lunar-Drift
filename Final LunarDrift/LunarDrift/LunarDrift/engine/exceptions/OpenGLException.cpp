#include "OpenGLException.h"
#include <sstream>

OpenGLException::OpenGLException(
    const std::string& report,
    GLenum glErrorCode)
    : std::runtime_error("OpenGLException: " + report + " (" +
                         glErrorCodeToString(glErrorCode) + ")"),
      m_Report(report),
      m_GLErrorCode(glErrorCode)
{
    //ctor
}

/** @brief Replacement for std::to_string, which is unavailable at present. */
template <typename T>
std::string to_string(T val)
{
    std::stringstream stream;
    stream << val;
    return stream.str();
}

const std::string OpenGLException::glErrorCodeToString(
    GLenum glErrorCode) const
{
    switch(glErrorCode)
    {
        case GL_NO_ERROR:
            return std::string("GL_NO_ERROR");

        case GL_INVALID_ENUM:
            return std::string("GL_INVALID_ENUM");

        case GL_INVALID_VALUE:
            return std::string("GL_INVALID_VALUE");

        case GL_INVALID_OPERATION:
            return std::string("GL_INVALID_OPERATION");

        case GL_INVALID_FRAMEBUFFER_OPERATION:
            return std::string("GL_INVALID_FRAMEBUFFER_OPERATION");

        case GL_OUT_OF_MEMORY:
            return std::string("GL_OUT_OF_MEMORY");

        case GL_STACK_UNDERFLOW:
            return std::string("GL_STACK_UNDERFLOW");

        case GL_STACK_OVERFLOW:
            return std::string("GL_STACK_OVERFLOW");

        default:
            return std::string("GL_UNKNOWN_ERROR(" +
                               to_string(glErrorCode) +
                               ")");
    }
}
