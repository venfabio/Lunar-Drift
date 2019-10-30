#pragma once

#include <stdexcept>
#include <string>

/**
 * @brief Exception raised whenever a core filesystem operation fails
 *
 * @author Hayley Hatton
 * @date   20/02/2016
 * @ingroup Exceptions
 */
class FileIOException : public std::runtime_error
{
public:
    /**
     * @brief Default constructor
     * @param file File name of the failed file.
     * @param err Error string
     */
    FileIOException(const std::string& file,
                    const std::string& err);


    const std::string& getFile() const { return m_File; }

    const std::string& getError() const { return m_Error; }

private:
    const std::string m_File;
    const std::string m_Error;
};
