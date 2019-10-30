#pragma once

#include "Context.h"
#include <string>

/**
 * @brief Handles and represents textures
 *
 * @author Hayley Hatton
 * @date   09/05/2016
 * @pre Requires use of GLI, and as such requires R8G8B8A8 DDS textures
 * @ingroup Graphics
 */
class Texture
{
public:
    /**
     * @brief Default constructor 
     * @param context Graphics context
     * @param file Name of the texture to load from the filesystem
     */
    Texture(Context* context, const std::string& file);

    /** Default destructor */
    ~Texture();

    /**
     * @brief Bind the texture as current to the provided context
     * @param context Graphics context
     */
    void bind(Context* context);

protected:

private:
    GLuint m_TexID; //!< OpenGL identifier of texture buffer
};
