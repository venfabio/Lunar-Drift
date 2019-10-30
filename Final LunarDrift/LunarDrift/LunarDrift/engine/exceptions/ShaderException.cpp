#include "ShaderException.h"

ShaderException::ShaderException(
    const std::string& file,
    const std::string& component,
    const std::string& stage,
    const std::string& errorLog)
    : std::logic_error("ShaderException: " + file + " (" + component + " " +
                       stage + " failed) " + errorLog),
      m_File(file),
      m_Component(component),
      m_Stage(stage),
      m_ErrorLog(errorLog)
{
    //ctor
}
