#pragma once

#include "Context.h"
#include <string>

/**
 * @brief Handles and represents cube-map textures for skyboxes
 *
 * @author Hayley Hatton
 * @date   09/05/2016
 * @ingroup Graphics
 */
class CubeMapTexture
{
public:
    /**
     * @brief Default constructor 
     * @note dir is a directory! The cube map is specified by 6 files named back.dds, left.dds, etc.
     * @param context Graphics context
     * @param file Name of the directory to load files from the filesystem
     */
	CubeMapTexture(Context* context, const std::string& dir);

    /** Default destructor */
	~CubeMapTexture();

    /**
    * @brief Bind the texture as current to the provided context
    * @param context Graphics context
    */
    void bind(Context* context);

protected:
	/**
	 * @brief Adds a texture to a given face of the cube
	 * @param target Face identifier
	 * @param filename Location of the texture to load
	 */
	void addFaceTexture(GLenum target, const std::string & filename);

private:
    GLuint m_TexID; //!< OpenGL identifier of texture buffer
};
