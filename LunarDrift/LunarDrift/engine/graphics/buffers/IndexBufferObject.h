#pragma once

#include "../Context.h"
#include <vector>

/**
 * @brief Buffer manager for model indices
 *
 * Index buffer objects (or IBOs) in OpenGL describe the order in which vertices
 * are drawn, which allow models to be drawn without repeating vertices.
 *
 * @pre    A valid OpenGL context must be present to the program
 * @author Hayley Hatton
 * @date   20/02/2016
 * @see    VertexArrayObject
 * @ingroup Graphics
 */
class IndexBufferObject
{
public:
    /**
     * @brief Default constructor. Creates the IBO buffer on the GPU
     * @param context Graphics context
     * @param indices Initial index buffer state
     */
	IndexBufferObject(
		Context* context,
		const std::vector<unsigned short>& indices);

    /** @brief Default destructor. Destroys the IBO buffer */
    virtual ~IndexBufferObject();


    /**
     * @brief Binds the IBO to the OpenGL pipeline
     * @param context Graphics context
     */
    void bind(Context* context);

    /**
     * @brief Access the amount of indices in the IBO
     * @return Number of elements in the IBO
     */
	unsigned int size() const { return m_Indices.size(); }

    /**
     * @brief Access the OpenGL integer class of the indices stored in the IBO
     * @return OpenGL enumerator describing the integer size
     */
	GLenum getOpenGLClass() const { return GL_UNSIGNED_SHORT; }

private:
	std::vector<unsigned short> m_Indices;	//!< Index buffer
    GLuint m_IBO;   //!< OpenGL-provided IBO handle
};
