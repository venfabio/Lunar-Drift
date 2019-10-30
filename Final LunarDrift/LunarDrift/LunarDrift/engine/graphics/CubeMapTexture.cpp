#include "CubeMapTexture.h"
#include "../Config.h"
#include <gli/gli.hpp>

CubeMapTexture::CubeMapTexture(Context* context, const std::string& dir)
{
    GLenum errCode;

    assert(context);
    assert((errCode = glGetError()) == GL_NO_ERROR);

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &m_TexID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_TexID);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);

	addFaceTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_X, dir + "/right.dds");
	addFaceTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, dir + "/left.dds");
	addFaceTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, dir + "/top.dds");
	addFaceTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, dir + "/bottom.dds");
	addFaceTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, dir + "/front.dds");
	addFaceTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, dir + "/back.dds");

    assert((errCode = glGetError()) == GL_NO_ERROR);
}

void CubeMapTexture::addFaceTexture(GLenum target, const std::string & filename)
{
	const std::string texPath = Config::get().get("_engine", "texturePath");
	gli::texture2D texture(gli::load(texPath + "/" + filename));

	GLsizei szX = texture.dimensions().x;
	GLsizei szY = texture.dimensions().y;

	uint8_t* data = static_cast<uint8_t*>(texture.data());
	uint8_t d[4] {data[0], data[1], data[2], data[3]};

	glTexImage2D(
		target, 
		0, 
		GL_RGBA,
		szX,
		szY,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		texture.data());
}

/** Default destructor */
CubeMapTexture::~CubeMapTexture()
{

}

void CubeMapTexture::bind(Context* context)
{
    assert(context);

	glBindTexture(GL_TEXTURE_CUBE_MAP, m_TexID);
}
