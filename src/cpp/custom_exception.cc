#include <custom_exception.h>

// constructor
CustomException::CustomException(const std::string &message) : message_(message) {}

// FILE NOT FOUND: exception
const char *CustomException::what() const noexcept
{
    return message_.c_str();
}

// constructor
// FILE NOT FOUND: exception
FileNotFoundException::FileNotFoundException(const std::string &message) : CustomException(message) {}

// constructor
// CAPTURE ALREADY OPENED: exception
CaptureOpenedException::CaptureOpenedException(const std::string &message) : CustomException(message) {}

// constructor
// CAPTURE EMPTY: exception
CaptureEmptyException::CaptureEmptyException(const std::string &message) : CustomException(message) {}
