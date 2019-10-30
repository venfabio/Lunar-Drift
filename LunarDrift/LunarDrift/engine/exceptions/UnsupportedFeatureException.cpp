#include "UnsupportedFeatureException.h"

UnsupportedFeatureException::UnsupportedFeatureException(
    const std::string& feature,
    const std::string& reason)
    : std::logic_error(feature + " unsupported! " + reason),
      m_Feature(feature),
      m_Reason(reason)
{
    //ctor
}
