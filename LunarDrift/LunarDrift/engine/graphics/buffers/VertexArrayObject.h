#pragma once

#include "VertexAttribute.h"
#include "VertexBufferObject.h"
#include "IndexBufferObject.h"

/**
 * @brief A buffer object that aggregates VBOs, Vertex Attributes and IBOs
 *
 * @pre    A valid OpenGL context must be present to the program
 * @author Hayley Hatton
 * @date   20/02/2016
 * @see    IndexBufferObject
 * @see    VertexBufferObject
 * @see    VertexAttribute
 * @ingroup Graphics
 */
class VertexArrayObject
{
public:
    /**
     * @brief Constructs a VAO and associates it with the VBO and/or IBO
     * @param context Graphics context
     * @param vbo Vertex buffer object ptr to associate with VAO
     * @param attributes Attribute map for describing vertex input to shaders
     * @param ibo Index buffer object ptr to associate with VAO
     */
    VertexArrayObject(
        Context* context,
        std::shared_ptr<VertexBufferObjectBase> vbo,
        const std::vector<VertexAttribute>& attributes,
        std::shared_ptr<IndexBufferObject> ibo = nullptr);

    /** @brief Default destructor */
    virtual ~VertexArrayObject();


    /**
     * @brief Associate a VertexBufferObject with the VAO
     * @param vbo VertexBufferObject ptr to associate with the VAO
     */
    void setVertexBuffer(std::shared_ptr<VertexBufferObjectBase> vbo);

    /**
     * @brief Access the VertexBufferObject associated with the VAO
     * @return Smart ptr to the underlying VertexBufferObject
     */
    std::shared_ptr<VertexBufferObjectBase> getVertexBuffer() {
        return m_VertexBuffer;
    }

    /**
     * @brief Set a new map of attributes to describe the associated vertex data
     * @param attributes Attribute map for describing vertex input to shaders
     */
    void setAttributeMap(const std::vector<VertexAttribute>& attributes);

    /**
     * @brief Access the map of attributes that describe the vertex data
     * @return Vector of attribute descriptors
     */
    const std::vector<VertexAttribute>& getAttributeMap() const {
        return m_Attributes;
    }

    /**
     * @brief Associate or disassociate an IndexBufferObject with the VAO
     * @param ibo IndexBufferObject ptr (or nullptr) to associate with the VAO
     */
    void setIndexBuffer(std::shared_ptr<IndexBufferObject> ibo);

    /**
     * @brief Access the IndexBufferObject associated with the VAO
     * @return Smart ptr (or nullptr) to the underlying IndexBufferObject
     */
    std::shared_ptr<IndexBufferObject> getIndexBuffer() {
        return m_IndexBuffer;
    }

    /**
     * @brief Bind the VAO to the OpenGL state pipeline
     * @param context Graphics context
     * @param shader Shader program to use with the VAO
     */
    void bind(Context* context, std::shared_ptr<ShaderProgram> shader);

private:
    std::vector<VertexAttribute> m_Attributes;  //!< Attribute map for shaders
    std::shared_ptr<VertexBufferObjectBase> m_VertexBuffer; //!< VBO ptr
    std::shared_ptr<IndexBufferObject> m_IndexBuffer;       //!< IBO ptr
    GLuint m_VAO;   //!< OpenGL-provided VAO handle
};
