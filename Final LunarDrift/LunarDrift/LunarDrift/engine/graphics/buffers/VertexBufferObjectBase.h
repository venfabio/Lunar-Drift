#pragma once

#include "../Context.h"

/**
 * @brief Abstract base class for specialized VertexBufferObjects
 *
 * This class represents VBOs in the abstract, managing their representation
 * in OpenGL in a way that's agnostic with regards to the data stored in the VBO
 * themselves. In most cases, this class will only be used when passing around
 * VBOs as an abstract concept; to work with VBOs, one should use the
 * VertexBufferObject class template with the vertex type specified.
 *
 * @pre    A valid OpenGL context must be present to the program
 * @author Hayley Hatton
 * @date   20/02/2016
 * @see    VertexBufferObject
 * @ingroup Graphics
 */
class VertexBufferObjectBase
{
public:
    /**
     * @brief Default constructor
     * @param context Graphics context
     * @param isConstant Should buffer be updateable (at cost of speed)?
     */
    VertexBufferObjectBase(Context* context, bool isConstant);

    /** @brief Default destructor */
    virtual ~VertexBufferObjectBase();


    /**
     * @brief Test if buffer is constant (i.e. not updateable)
     *
     * If true, this means the buffer was made in GL_STATIC_DRAW, and
     * no updates are able to be made.
     */
    bool isConstant() const { return m_IsConstant; }

    /**
     * @brief Retrieve the size of the individual vertices in this object
     * @return Size of a vertex, in GL-casted format
     */
    virtual GLsizei getVertexSize() const = 0;

    /**
     * @brief Retrieve the size of the vertex buffer
     * @return Number of vertices in the CPU-side buffer
     */
    virtual unsigned int size() const = 0;

    /**
     * @brief Bind the VBO to the OpenGL state pipeline
     * @param context Graphics context
     */
    void bind(Context* context);


private:
    GLuint m_VBO;   //!< OpenGL-provided VBO handle
    const bool m_IsConstant;  //!< If true, no updates can be made!
};
