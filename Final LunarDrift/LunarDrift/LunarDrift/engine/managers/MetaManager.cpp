#include "MetaManager.h"
#include <cassert>


MetaManager::MetaManager()
{
}

MetaManager::~MetaManager()
{
}


void MetaManager::add(std::shared_ptr<Manager> manager)
{
	assert(manager);
	m_Managers[manager->getManagerName()] = manager;
}

void MetaManager::remove(const std::string& name)
{
	m_Managers.erase(name);
}

std::weak_ptr<Manager> MetaManager::get(const std::string& name)
{
	std::map<std::string, std::shared_ptr<Manager>>::iterator itr;
	if ((itr = m_Managers.find(name)) == m_Managers.end())
		return std::weak_ptr<Manager>();
	else
		return m_Managers.at(name);
}
