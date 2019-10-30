#include "Texture.h"
#include "../Config.h"
#include <gli/gli.hpp>

Texture::Texture(Context* context, const std::string& file)
{
    GLenum errCode;

    assert(context);
    assert((errCode = glGetError()) == GL_NO_ERROR);

    const std::string texPath = Config::get().get("_engine", "texturePath");
    gli::texture2D texture(gli::load(texPath + "/" + file));

    glGenTextures(1, &m_TexID);
    glBindTexture(GL_TEXTURE_2D, m_TexID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLsizei szX = texture.dimensions().x;
    GLsizei szY = texture.dimensions().y;

    uint8_t* data = static_cast<uint8_t*>(texture.data());
    uint8_t d[4] {data[0], data[1], data[2], data[3]};

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        szX,
        szY,
        0,
        GL_BGRA,
        GL_UNSIGNED_BYTE,
        texture.data());

    assert((errCode = glGetError()) == GL_NO_ERROR);
}

/** Default destructor */
Texture::~Texture()
{

}

void Texture::bind(Context* context)
{
    assert(context);

    glBindTexture(GL_TEXTURE_2D, m_TexID);
}
