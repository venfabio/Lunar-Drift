#pragma once

#include "../shaders/ShaderProgram.h"
#include <memory>

/**
 * @brief Describes an attribute in a given vertex type for shader inputting
 *
 * Vertices can take many forms, yet describe similar data. Likewise, shaders
 * can take the same data in different ways, depending on the index generated.
 * Consequently, this class acts to map a given attribute as a "semantic",
 * which is translated trivially into the shaders' input's name.
 *
 * @pre    A valid OpenGL context must be present to the program
 * @author Hayley Hatton
 * @date   20/02/2016
 * @see    VertexBufferObject
 * @see    ShaderProgram
 * @see    VertexArrayObject
 * @ingroup Graphics
 */
class VertexAttribute
{
public:
    /**
     * @brief Type of the sub-components of this vertex attribute
     */
    enum class Type
    {
        Byte = GL_BYTE, UByte = GL_UNSIGNED_BYTE,
        Short = GL_SHORT, UShort = GL_UNSIGNED_SHORT,
        Int = GL_INT, UInt = GL_UNSIGNED_INT,
        HalfFloat = GL_HALF_FLOAT, Float = GL_FLOAT,
        Double = GL_DOUBLE, Fixed = GL_FIXED
    };

    /**
     * @brief Constructor
     * @param semantic Shader input name (e.g. position, which becomes a_Position)
     * @param offset Offset into the vertex structure where this attribute begins
     * @param stride Size of the surrounding vertex
     * @param type Type of attribute
     * @param count Number of said types per attribute (e.g. vec4 = type float, count 4)
     */
    VertexAttribute(
        const std::string& semantic,
        GLsizeiptr offset,
        GLsizei stride,
        Type type,
        GLint count);

    /** @brief Default destructor */
    ~VertexAttribute();

    /**
     * @brief Enable use of this vertex attribute in the VAO
     * @param context Graphics context
     * @param shader Smart pointer of a shader program to connect attribute to
     */
    void bind(Context* context, std::shared_ptr<ShaderProgram> shader);

private:
    std::string m_Semantic; //!< Associated shader input name
    GLsizeiptr m_Offset;    //!< Vertex offset
    GLsizei m_Stride;       //!< Size of the vertex
    Type m_Type;            //!< Attribute type
    GLint m_Count;          //!< Quantity of consecutive types in vertex
};
