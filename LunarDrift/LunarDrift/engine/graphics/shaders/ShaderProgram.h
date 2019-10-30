#pragma once

#include "../Context.h"
#include <string>

/**
 * @brief Container for a GLSL shader program and its constituent shaders.
 *
 * This is composed of two to three shaders (if using geometry shaders)
 * compiled and linked into a "shader program".
 *
 * This supports Vertex/Fragment Shaders and Vertex/Geometry/Fragment Shaders.
 * Version 4 is not yet supported, and so Tessellation Control and Evaluator
 * shaders are not presently available.
 *
 * @pre    A valid OpenGL context must be present to the program.
 * @author Hayley Hatton
 * @date   06/03/2016
 * @ingroup Graphics
 */
class ShaderProgram
{
public:
    /**
     * @brief Normal shader program constructor
     * @param context Graphics context
     * @param vertex Vertex shader filename, minus .vert extension
     * @param fragment Fragment shader filename, minus .frag extension
     * @throw FileIOException
     * @throw ShaderException
     * @throw OpenGLException
     */
    ShaderProgram(
        Context* context,
        const std::string& vertex,
        const std::string& fragment
    );

    /**
     * @brief Shader program w/ geometry shader constructor
     * @param context Graphics context
     * @param vertex Vertex shader filename, minus .vert extension
     * @param geometry Geometry shader filename
     * @param fragment Fragment shader filename, minus .frag extension
     * @throw FileIOException
     * @throw ShaderException
     * @throw OpenGLException
     */
    ShaderProgram(
        Context* context,
        const std::string& vertex,
        const std::string& geometry,
        const std::string& fragment
    );

    /** @brief Default destructor */
    virtual ~ShaderProgram();

    /**
     * @brief Loads a shader program
     * @param context Graphics context
     * @param vertex Vertex shader filename, minus .vert extension
     * @param fragment Fragment shader filename, minus .frag extension
     * @throw FileIOException
     * @throw ShaderException
     * @throw OpenGLException
     */
    void load(
        Context* context,
        const std::string& vertex,
        const std::string& fragment
    );

    /**
     * @brief Loads a shader program w/ geometry shader (OpenGL 3.2 required)
     * @param context Graphics context
     * @param vertex Vertex shader filename, minus .vert extension
     * @param geometry Geometry shader filename, minus .geom extension
     * @param fragment Fragment shader filename, minus .frag extension
     * @throw FileIOException
     * @throw ShaderException
     * @throw OpenGLException
     */
    void load(
        Context* context,
        const std::string& vertex,
        const std::string& geometry,
        const std::string& fragment
    );

    /** @brief Unloads the shader program and its children */
    void unload();

    /**
     * @brief Returns the program handle of the shader program
     * @return Program handle
     */
    GLuint getProgramHandle() const { return m_ProgramHandle; };

    /**
     * @brief Obtains the location of an attribute in the shader program
     * @param context Graphics context
     * @param name Name of the attribute
     * @return Handle to the attribute; -1 if error
     */
    GLint getAttributeLocation(
        Context* context,
        const std::string & name) const;

    /**
     * @brief Obtains the location of a uniform in the shader program
     * @param context Graphics context
     * @param name Name of the uniform
     * @return Handle to the uniform; -1 if error
     */
    GLint getUniformLocation(
        Context* context, 
        const std::string & name) const;

    /**
     * @brief Get the identifier of a given uniform block
     * @param name Name of the uniform block
     * @return Identifier of the uniform block
     */
    int getUniformBlockLocation(
        Context* context,
        const std::string& name);

    /**
     * @brief Bind a given uniform block at an index
     * @param name Name of the uniform block
     * @param bindingPoint Index of uniform block to bind at
     */
    void bindUniformBlock(const std::string& name, int bindingPoint) const {
        int id = glGetUniformBlockIndex(m_ProgramHandle, name.c_str());
        glUniformBlockBinding(m_ProgramHandle, id, bindingPoint);
    }

    /**
     * @brief Bind the shader program to the OpenGL state pipeline
     * @param context Graphics context
     */
    void use(Context* context) { glUseProgram(m_ProgramHandle); }

protected:
    /**
     * @brief Load and compile shader code from a file
     * @param context Graphics context
     * @param name Shader filename
     * @param stage Shader stage (e.g. GL_VERTEX_SHADER)
     * @return Handle to the shader code on the GPU
     * @throw FileIOException
     * @throw ShaderException
     */
    GLint loadAndCompileFile(
        Context* context, 
        const std::string& name, 
        GLenum stage);

    /**
     * @brief Reads a shader from a file and returns its source code
     * @param filename Location of the shader file
     * @param data Ptr to string to be filled with the shader file's data
     * @throw FileIOException
     */
    void readShaderFile(
        const std::string& filename,
        std::string* data);

private:
    /// @{ @brief Shader handlers.
    GLuint m_ProgramHandle;
    GLint m_VertexShaderHandle;
    GLint m_GeometryShaderHandle;
    GLint m_FragmentShaderHandle;
    /// @}
};
