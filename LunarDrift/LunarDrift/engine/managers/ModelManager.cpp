#include "ModelManager.h"


ModelManager::ModelManager() : Manager("model")
{
}

ModelManager::~ModelManager()
{
}


std::weak_ptr<Model> ModelManager::loadModel(
	Context* context,
	const std::string& id, 
	const std::string& path) 
{
	std::shared_ptr<Model> preloaded;
	std::map<std::string, std::shared_ptr<Model>>::iterator it;
	if ((it = m_Models.find(id)) == m_Models.end())
	{
		preloaded = std::make_shared<Model>(context, path);
		m_Models[id] = preloaded;
		return preloaded;
	}
	else {
		return (*it).second;
	}
}

void ModelManager::addModel(
    const std::string& id, 
    std::shared_ptr<Model> model)
{
	m_Models[id] = model;
}

std::weak_ptr<Model> ModelManager::getModel(const std::string& id)
{
	std::map<std::string, std::shared_ptr<Model>>::const_iterator itr;
	if ((itr = m_Models.find(id)) == m_Models.end())
		return std::weak_ptr<Model>();

	return m_Models.at(id);
}

void ModelManager::removeModel(const std::string& id)
{
	m_Models.erase(id);
}

void ModelManager::removeAllModels()
{
	m_Models.clear();
}
