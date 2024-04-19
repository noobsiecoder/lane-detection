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
/// @brief This file contains all the custom exceptions declarations

#include <custom_exception.h>

/// @brief Raise custom exception error
/// @return
const char *CustomException::what() const noexcept
{
    return message_.c_str();
}

/// @brief Raise an error when required arguments are not passed
/// @param message error message
ArgsNotFoundException::ArgsNotFoundException(const std::string &message) : CustomException(message) {}

/// @brief Raise an error when path given does not exist
/// @param message error message
PathNotFoundException::PathNotFoundException(const std::string &message) : CustomException(message) {}

/// @brief Raise an error when capture object is empty
/// @param message error message
CaptureEmptyException::CaptureEmptyException(const std::string &message) : CustomException(message) {}

/// @brief Raise an error when capture object is already opened
/// @param message error message
CaptureOpenedException::CaptureOpenedException(const std::string &message) : CustomException(message) {}
