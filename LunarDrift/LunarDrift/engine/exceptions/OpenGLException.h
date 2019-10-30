#pragma once

#include "../graphics/OpenGLConfig.h"
#include <stdexcept>
#include <string>

/**
 * @brief Exception raised whenever an OpenGL subsystem fails
 *
 * @author Hayley Hatton
 * @date   20/02/2016
 * @ingroup Exceptions
 */
class OpenGLException : public std::runtime_error
{
public:
    /**
     * @brief Default constructor
     * @param report Error string
     * @param glErrorCode OpenGL API error code
     */
    OpenGLException(
        const std::string& report,
        GLenum glErrorCode);


    const std::string& getReport() const { return m_Report; }

    GLenum getGLErrorCode() const { return m_GLErrorCode; }

    const std::string glErrorCodeToString(GLenum glErrorCode) const;

private:
    const std::string m_Report;
    const GLenum m_GLErrorCode;
};
