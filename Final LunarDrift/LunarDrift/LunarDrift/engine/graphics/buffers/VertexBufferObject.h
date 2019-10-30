#pragma once

#include "VertexBufferObjectBase.h"
#include "../../exceptions/OpenGLException.h"
#include <vector>
#include <cassert>

/**
 * @brief Specialized VertexBufferObject template, for handling generic vertices
 * @tparam V Generic vertex parameter to use for the buffer
 *
 * Whereas the VertexBufferObjectBase allows VBOs to be represented generically,
 * this class acts as a template for providing vertices to the VBO.
 * Consequently, when making a VBO, this class template should be used and
 * parameterized with the vertex type used. When using VBOs in the abstract,
 * consider passing around VertexBufferObjectBase objects instead.
 *
 * @pre    A valid OpenGL context must be present to the program
 * @author Hayley Hatton
 * @date   20/02/2016
 * @see    VertexBufferObjectBase
 * @ingroup Graphics
 */
template <typename V>
class VertexBufferObject : public VertexBufferObjectBase
{
public:
    /**
     * @brief Constructor
     * @param context Graphics context
     * @param vertices Array of vertices to initially initialize buffer with
     */
    VertexBufferObject(
        Context* context,
        const std::vector<V>& vertices,
        bool isConstant = true)
        : VertexBufferObjectBase(context, isConstant),
          m_Vertices(vertices)
    {
        assert(context);

        GLenum errCode;

        GLenum usage = isConstant ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;

        const GLsizeiptr szBuffer = vertices.size() * sizeof(V);
        glBufferData(
            GL_ARRAY_BUFFER,
            szBuffer,
            static_cast<const GLvoid*>(&vertices[0]),
            usage);
        if((errCode = glGetError()) != GL_NO_ERROR)
            throw OpenGLException("Could not buffer vertex data", errCode);
    }

    /** @brief Default destructor */
    virtual ~VertexBufferObject() {};


    /**
     * @brief Access the vertex at a given offset/index in the buffer
     * @param index Zero-based index in the buffer to access vertex
     * @return Accessed vertex
     */
    const V& getVertex(unsigned int index) const { return m_Vertices[index]; }

    /**
    * @brief Access the vertices of the internal vertex buffer
    * @param vertices Pointer to buffer to copy vertices into
    */
    void getVertices(std::vector<V>* vertices) const { *vertices = m_Vertices; }

    /**
    * @brief Set the vertex at a given offset/index in the buffer
    * @param index Zero-based index in the buffer to set vertex
    * @param v New vertex state to set as
    */
    void setVertex(unsigned int index, const V& v)
    {
        assert(!isConstant());  // Useless if not updateable
        if (index > (m_Vertices.size() + 1))
            addVertex(v);
        else
            m_Vertices[index] = v;
    }

    /**
     * @brief Reset vertex buffer with a new one
     * @param vertices Pointer to source of new vertex buffer
     */
    void setVertices(const std::vector<V>& vertices)
    {
        m_Vertices = vertices;
    }

    /**
     * @brief Add a new vertex to the end of the buffer
     * @param v New vertex to push onto buffer
     */
    void addVertex(const V& v) 
    {
        assert(!isConstant());  // Useless if not updateable
        m_Vertices.push_back(v); 
    }

	/**
	* @brief Clear the vertex buffer so it's completely clean and fresh
	*/
	void clearVertices()
	{
		assert(!isConstant());	// Useless if not updateable
		m_Vertices.clear();
	}

    /**
     * @brief Resynchronize the CPU buffer state to the GPU ("commit updates")
     * @param context Graphics context
     */
    void resynchronize(Context* context)
    {
        assert(context);
        assert(!isConstant());  // Undefined behaviour if not updateable

        bind(context);

        glBufferSubData(
            GL_ARRAY_BUFFER,
            0,
            static_cast<GLsizeiptr>(m_Vertices.size() * sizeof(V)),
            static_cast<const GLvoid*>(&m_Vertices[0]));
    }

    /**
     * @brief Retrieve the size of the individual vertices in this object
     * @return Size of a vertex, in GL-casted format
     */
    GLsizei getVertexSize() const override { return sizeof(V); }

    /**
     * @brief Retrieve the size of the vertex buffer
     * @return Number of vertices in the CPU-side buffer
     */
    unsigned int size() const override { return m_Vertices.size(); }

private:
    std::vector<V> m_Vertices;  //!< CPU-side buffer of vertices
};
