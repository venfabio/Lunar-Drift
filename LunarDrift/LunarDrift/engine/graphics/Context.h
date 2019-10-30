#pragma once

#include "OpenGLConfig.h"
#include <memory>

/**
 * @defgroup Graphics
 * @brief Back-end classes pertaining to the encapsulating of OpenGL graphics data and processes
 */

/**
 * @brief OpenGL context wrapper and provider
 *
 * This is practically empty due to the fact OpenGL's functions and context
 * states are passed around through the thread stack, unlike Direct3D, which
 * has ID3D11DeviceContext and related objects for accessing graphical API
 * states and calls. However, this should be <i>treated</i> in a similar manner:
 * do <i>not</i> call OpenGL functions unless a Context object is present.
 * An exception can be made for Destructors, where parameters cannot be passed 
 * in, of course (just be sure to terminate objects on a thread with an
 * OpenGL context available or OpenGL delete functions will crash!).
 * Essentially: does the function or procedure have a non-null Context?
 * If yes: safe to use OpenGL functions! If no: do so at your own risk.
 *
 * @author Hayley Hatton
 * @date   20/02/2016
 * @see    Renderer
 * @ingroup Graphics
 */
class Context
{
public:
    /** @brief Default constructor */
    Context();

    /** @brief Default destructor */
    virtual ~Context();


    /**
     * @brief Set the dimensions of the full rendering context
     * @param width Width in pixels
     * @param height Height in pixels
     */
    void setDimensions(GLsizei width, GLsizei height);

    /**
     * @brief Access the dimensions of the full rendering context
     * @param width Pointer to store width in pixels
     * @param height Pointer to store height in pixels
     */
    void getDimensions(GLsizei* width, GLsizei* height) const;

protected:

private:
    GLsizei m_Width, m_Height;
};
