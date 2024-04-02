#ifndef CUSTOM_EXCEPTION_H
#define CUSTOM_EXCEPTION_H

#include <stdexcept>

// throw if file not found in the directory system
class CustomException : public std::exception
{
public:
    // default constructor
    CustomException(const std::string &message);

    // implementation of custom exception throw
    const char *what() const noexcept override;

private:
    // store exception value
    std::string message_;
};

// throw if file not found in the directory system
class FileNotFoundException : public CustomException
{
public:
    FileNotFoundException(const std::string &message);
};

// throw if video capture object is already opened
class CaptureOpenedException : public CustomException
{
public:
    CaptureOpenedException(const std::string &message);
};

// throw if video capture object is empty after loading
class CaptureEmptyException : public CustomException
{
public:
    CaptureEmptyException(const std::string &message);
};

#endif