#include <custom_exception.h>

// constructor
FileNotFoundException::FileNotFoundException(const std::string &message) : message_(message) {}

// FILE NOT FOUND: exception
const char *FileNotFoundException::what() const noexcept
{
    return message_.c_str();
}