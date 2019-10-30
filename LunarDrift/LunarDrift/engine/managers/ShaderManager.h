#pragma once

#include "../graphics/shaders/ShaderProgram.h"
#include <map>
#include <vector>

/**
 * @brief Container for associating shader programs with shading class key names
 *
 * This class thus encapsulates shader management away from the renderers, and
 * allows a map of shader programs associated with shading classes to be passed
 * around easily.
 *
 * @author Hayley Hatton
 * @date   07/03/2016
 * @see    ShaderProgram
 * @see    Renderer
 * @ingroup Managers
 */
class ShaderManager
{
public:
    /** @brief Default constructor */
    ShaderManager();

    /** @brief Default destructor */
    virtual ~ShaderManager();


    /**
     * @brief Associate a shader program with a shading class key
     * @param key Shading class name
     * @param shader Shader program to associate
     */
    void set(const std::string& key, std::shared_ptr<ShaderProgram> shader);

    /**
     * @brief Return an associated shader program from a given shading class
     * @param key Shading class name
     * @return Associated shader program
     */
    std::shared_ptr<ShaderProgram> get(const std::string& key);

    /**
     * @brief Get a list of all the shader programs in the manager
     * @param shaders Pointer to a vector class to hold the shader programs
     */
    void get(std::vector<std::shared_ptr<ShaderProgram>>* shaders) const;

private:
    std::map<std::string,std::shared_ptr<ShaderProgram>> m_ShaderMap;
};
