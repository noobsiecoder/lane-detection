// MIT License

// Copyright (c) 2024 Abhishek Sriram

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

/// @file
/// @brief This file contains all the custom exceptions definitions

#ifndef CUSTOM_EXCEPTION_H
#define CUSTOM_EXCEPTION_H

#include <stdexcept>

/// @brief Class to raise custom exception
class CustomException : public std::exception
{
public:
    /// @brief Default constructor to store error message
    /// @param message error message to throw
    CustomException(const std::string &message) : message_("Error: " + message) {}

    /// @brief Implementation of custom exception
    /// @return custom error message
    const char *what() const noexcept override;

private:
    // store exception value
    std::string message_;
};

/// @brief Class to raise an error when required arguments are not passed
class ArgsNotFoundException : public CustomException
{
public:
    /// @brief Arguments not passed
    /// @param message error message
    ArgsNotFoundException(const std::string &message);
};

/// @brief Class to raise exception if path not found in the directory system
class PathNotFoundException : public CustomException
{
public:
    /// @brief Path not found in directory
    /// @param message error message
    PathNotFoundException(const std::string &message);
};

/// @brief Class to raise exception if video capture object is empty after loading
class CaptureEmptyException : public CustomException
{
public:
    /// @brief Capture object is empty
    /// @param message error message
    CaptureEmptyException(const std::string &message);
};

/// @brief Class to raise exception if video capture object is already opened
class CaptureOpenedException : public CustomException
{
public:
    /// @brief Capture object
    /// @param message error message
    CaptureOpenedException(const std::string &message);
};

#endif // CUSTOM_EXCEPTION_H
