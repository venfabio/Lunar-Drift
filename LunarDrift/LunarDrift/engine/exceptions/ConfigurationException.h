#pragma once

#include <stdexcept>
#include <string>

/**
 * @defgroup Exceptions
 * @brief Custom exception classes for handling error paths in a programmer-friendly, RAII manner
 */

/**
 * @brief Exception raised whenever a configuration setting is exceptional
 *
 * @author Hayley Hatton
 * @date   20/02/2016
 * @ingroup Exceptions
 */
class ConfigurationException : public std::runtime_error
{
public:
    /**
     * @brief Default constructor
     * @param setting Configuration setting at fault
     * @param err Error string
     */
    ConfigurationException(const std::string& setting,
                           const std::string& err);

    const std::string& getSetting() const { return m_Setting; }

    const std::string& getError() const { return m_Error; }

protected:
    const std::string m_Setting;
    const std::string m_Error;
};
