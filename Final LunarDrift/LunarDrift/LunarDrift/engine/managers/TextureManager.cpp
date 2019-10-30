#include "TextureManager.h"


TextureManager::TextureManager() : Manager("texture")
{
}

TextureManager::~TextureManager()
{
}


std::weak_ptr<Texture> TextureManager::getTex(
    Context* context, 
    const std::string& file)
{
	std::shared_ptr<Texture> tex = nullptr;
	try
	{
        tex = std::make_shared<Texture>(context, file);
	}
	catch (std::exception ex)
	{
		return std::weak_ptr<Texture>();
	}

    m_Textures[file] = tex;

	return tex;
}

std::weak_ptr<Texture> TextureManager::getTex(const std::string& path)
{
	if (m_Textures.find(path) == m_Textures.end())
		return std::weak_ptr<Texture>();

	return m_Textures.at(path);
}
