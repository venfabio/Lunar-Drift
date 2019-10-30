#pragma once

#include "Manager.h"
#include "../graphics/Texture.h"
#include <map>

/**
 * @brief A manager class for handling and passing around texture data
 *
 * @author Hayley Hatton
 * @date   13/03/2016
 * @see    Manager
 * @ingroup Managers
 */
class TextureManager : public Manager
{
public:

    /** @brief Default constructor */
	TextureManager();

    /** @brief Default destructor */
	~TextureManager();

	
    /**
     * @brief Access a texture, or otherwise load it
     * @param context Graphics context
     * @param file Name of the texture to load
     */
    std::weak_ptr<Texture> getTex(Context* context, const std::string& file);

    /**
     * @brief Access a texture, or otherwise throw an exception
     * @param file Name of the texture to load
     */
    std::weak_ptr<Texture> getTex(const std::string& file);

private:
	std::map<std::string, std::shared_ptr<Texture>> m_Textures;
};
