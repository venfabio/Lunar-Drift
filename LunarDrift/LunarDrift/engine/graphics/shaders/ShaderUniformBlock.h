#pragma once

/**
 * @brief Representation of a shader uniform block
 *
 * Shader uniform blocks in OpenGL are basically uniforms that are uniform
 * across all shaders. This is reeeaaally useful for stuff like scene matrices,
 * where potentially hundreds of duplicate projection and view matrix calls are
 * performed; instead of hundreds, upload once to a shader uniform buffer and
 * that's it.
 *
 * @author   Hayley Hatton
 * @date     02/03/2016
 * @ingroup Graphics
 */
class ShaderUniformBlock
{
public:
    /** Default constructor */
    ShaderUniformBlock();
    /** Default destructor */
    virtual ~ShaderUniformBlock();

protected:

private:
};
