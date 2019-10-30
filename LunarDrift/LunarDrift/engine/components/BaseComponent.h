#pragma once

#include <string>

/**
 * @defgroup Components
 * @brief List of components for plugging into SceneObject children
 */

/**
 * @brief Abstract base class for deriving components from
 *
 * @author Hayley Hatton
 * @date   06/03/2016
 * @ingroup Components
 */
class BaseComponent
{
public:
	/**
	 * @brief Default constructor
	 * @param name Identifying name of the component class
	 */
	BaseComponent(const std::string& name) : m_Name(name) {};

	/** @brief Default destructor */
	virtual ~BaseComponent() {};


	/**
	 * @brief Access the identifying name of the component's class
	 * @return Identifying name of the component class
	 */
	const std::string& getName() const { return m_Name; }

private:
	const std::string m_Name;	//!< Identifying name of the component class
};

