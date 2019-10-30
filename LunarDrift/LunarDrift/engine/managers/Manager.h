#pragma once

#include <string>

/**
 * @defgroup Managers
 * @brief List of managers for encapsulating behaviour and managing resources effectively
 */

/**
 * @brief Parent class for managers to derive from
 *
 * @author Hayley Hatton
 * @date   07/03/2016
 * @ingroup Managers
 */
class Manager
{
public:
    /**
     * @brief Default constructor
     * @param name Unique identification name of the manager's class
     */
	Manager(const std::string& name) : m_Name(name) {};

    /** @brief Default destructor */
	virtual ~Manager() {};


    /**
     * @brief Retrieve the unique identification name for identifying the manager class
     * @return String representation of manager's identifying name
     */
	const std::string& getManagerName() { return m_Name; }

private:
	const std::string m_Name;   //!< String representation of manager's identifying name
};
