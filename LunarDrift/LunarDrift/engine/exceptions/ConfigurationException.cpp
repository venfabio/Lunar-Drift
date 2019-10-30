#include "ConfigurationException.h"

ConfigurationException::ConfigurationException(
    const std::string& setting,
    const std::string& err)
    : std::runtime_error("ConfigurationException: " + setting + " " + err),
      m_Setting(setting),
      m_Error(err)
{
    //ctor
}
