#ifndef CUSTOM_EXCEPTION_H
#define CUSTOM_EXCEPTION_H

#include <stdexcept>

// throw if file not found in the directory system
class FileNotFoundException : public std::exception
{
public:
    // default constructor
    FileNotFoundException(const std::string &message);

    // implementation of custom exception throw
    const char *what() const noexcept override;

private:
    // store exception value
    std::string message_;
};

#endif