#pragma once

#include "Manager.h"
#include <map>
#include <memory>

/**
 * @brief A manager class for managing and passing around managers
 *
 * @author Hayley Hatton
 * @date   13/03/2016
 * @see    Manager
 * @ingroup Managers
 */
class MetaManager
{
public:
    /** @brief Default constructor */
    MetaManager();

    /** @brief Default destructor */
    virtual ~MetaManager();

	
    /**
     * @brief Add a manager to be managed by the metamanager
     * @param manager Smart ptr to the manager to handle
     */
	void add(std::shared_ptr<Manager> manager);

    /**
     * @brief Remove a manager from the metamanager, if it exists
     * @param name Identifier name of the manager to remove
     */
	void remove(const std::string& name);

    /**
     * @brief Access a manager managed by the metamanager
     * @param name Identifier name of the manager to acquire
     * @return Weakly managed ptr to the manager; nullptr if not available
     */
	std::weak_ptr<Manager> get(const std::string& name);

private:
	std::map<std::string, std::shared_ptr<Manager>> m_Managers;
};
