/**
 * @file SNBP.cpp
 * @brief Comprehensive implementation of utility functions for the Bassil language project.
 *
 * This file contains a collection of utility functions that provide various
 * string manipulation, Windows API interaction, file handling, and console
 * output formatting capabilities. These utilities are designed to support
 * the core functionality of the Bassil language project.
 *
 * @note This file requires Windows-specific headers and may not be
 * compatible with non-Windows environments.
 *
 * @author Nerd Bear
 * @date 31 August 2024
 * @version 0.7.4
 *
 * @copyright Copyright (c) 2024 Bassil
 *
 * @see SNBP.h
 */

#include "C:/coding-projects/CPP-Dev/bassil/src/headers/SNBP.h"

#include <algorithm>
#include <stdexcept>
#include <fstream>
#include <limits>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <iomanip>
#include <string_view>
#include <regex>

#include <strsafe.h>
#include <io.h>
#include <fcntl.h>
#include <wbemidl.h>
#include <shlobj.h>
#include <comdef.h>

#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "shell32.lib")

/**
 * @brief Fallback implementation of StringCchCopyW if not available in the system headers.
 *
 * This macro provides a simple implementation of StringCchCopyW using wcscpy_s
 * if the function is not available in the included headers. It ensures
 * compatibility across different Windows environments.
 *
 * @param dest Destination wide-character string buffer.
 * @param destSize Size of the destination buffer in characters.
 * @param src Source wide-character string to be copied.
 *
 * @note This macro should only be used if StringCchCopyW is not defined.
 */
#ifndef StringCchCopyW
#define StringCchCopyW(dest, destSize, src) wcscpy_s(dest, destSize, src)
#endif

namespace SNBP
{
    std::vector<std::string> splitString(const std::string &s, const std::string &delimiter);

    std::string &ltrim(std::string &s);

    std::string &rtrim(std::string &s);

    std::string &trim(std::string &s);

    std::string centerString(const std::string &text, size_t width, char fillChar);

    std::string wrapText(const std::string &text, size_t lineLength);

    std::string leftPad(std::string str, int amount);

    std::string rightPad(std::string str, int amount);

    std::string pad(std::string str, int amount);
} // namespace SNBP