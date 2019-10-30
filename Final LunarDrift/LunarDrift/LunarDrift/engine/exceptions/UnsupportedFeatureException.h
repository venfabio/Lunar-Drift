#pragma once

#include <stdexcept>
#include <string>

/**
 * @brief Exception raised whenever a core feature is unsupported
 *
 * @author Hayley Hatton
 * @date   20/02/2016
 * @ingroup Exceptions
 */
class UnsupportedFeatureException : public std::logic_error
{
public:
    /**
     * @brief Default constructor
     * @param feature Engine feature that's not supported.
     * @param reason The reason behind why they're not supported (optional).
     */
    UnsupportedFeatureException(
        const std::string& feature,
        const std::string& reason);


    const std::string& getFeature() const { return m_Feature; }

    const std::string& getReason() const { return m_Reason; }

private:
    const std::string m_Feature;
    const std::string m_Reason;
};
