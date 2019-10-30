#include "ShaderManager.h"
#include <cassert>

ShaderManager::ShaderManager()
{
    //ctor
}

ShaderManager::~ShaderManager()
{
    //dtor
}


void ShaderManager::set(
    const std::string& key,
    std::shared_ptr<ShaderProgram> shader)
{
    m_ShaderMap[key] = shader;
}

std::shared_ptr<ShaderProgram> ShaderManager::get(const std::string& key)
{
    if(m_ShaderMap.find(key) == m_ShaderMap.end())
        throw std::out_of_range("Shading class key not found in manager");

    return m_ShaderMap[key];
}

void ShaderManager::get(
    std::vector<std::shared_ptr<ShaderProgram>>* shaders) const
{
    assert(shaders);

    std::map<std::string,std::shared_ptr<ShaderProgram>>::const_iterator itr;
    for(itr = m_ShaderMap.begin(); itr != m_ShaderMap.end(); ++itr)
    {
        std::pair<std::string,std::shared_ptr<ShaderProgram>> entry = *itr;
        shaders->push_back(entry.second);
    }
}
