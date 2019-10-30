#include "FileIOException.h"

FileIOException::FileIOException(
    const std::string& file,
    const std::string& err)
    : std::runtime_error("FileIOException: " + file + " " + err),
      m_File(file),
      m_Error(err)
{
    //ctor
}
