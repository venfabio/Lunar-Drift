#pragma once

#include <stdexcept>
#include <string>

/**
 * @brief Exception raised whenever a shader operation fails
 *
 * @author Hayley Hatton
 * @date   20/02/2016
 * @ingroup Exceptions
 */
class ShaderException : public std::logic_error
{
public:
    /**
     * @brief Default constructor
     * @param file File where the error was raised
     * @param component Shader component of the program where it went wrong
     * @param stage Stage of the shader pipeline where the error was raised
     * @param errorLog Generated error log
     */
    ShaderException(
        const std::string& file,
        const std::string& component,
        const std::string& stage,
        const std::string& errorLog);


    const std::string& getFile() const { return m_File; }

    const std::string& getComponent() const { return m_Component; }

    const std::string& getStage() const { return m_Stage; }

    const std::string& getErrorLog() const { return m_ErrorLog; }


private:
    const std::string m_File;
    const std::string m_Component;
    const std::string m_Stage;
    const std::string m_ErrorLog;
};
