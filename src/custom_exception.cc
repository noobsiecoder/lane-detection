#include <custom_exception.h>

// FILE NOT FOUND: exception

const char *CustomException::what() const noexcept
{
    return message_.c_str();
}

// throw if required arguments are not given

ArgsNotFoundException::ArgsNotFoundException(const std::string &message) : CustomException(message) {}

// constructor
// FILE NOT FOUND: exception

PathNotFoundException::PathNotFoundException(const std::string &message) : CustomException(message) {}

// constructor
// CAPTURE EMPTY: exception

CaptureEmptyException::CaptureEmptyException(const std::string &message) : CustomException(message) {}

// constructor
// CAPTURE ALREADY OPENED: exception

CaptureOpenedException::CaptureOpenedException(const std::string &message) : CustomException(message) {}
