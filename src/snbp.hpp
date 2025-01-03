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
    // Version of SNBP
    std::string version = "Currently not defined, please run SNBP::init() to initialize the package";

    // The author of SNBP
    std::string author = "Currently not defined, please run SNBP::init() to initialize the package";

    // The date when this version of SNBP was created
    std::string date = "Currently not defined, please run SNBP::init() to initialize the package";

    // The license that SNBP is currently under
    std::string license = "Currently not defined, please run SNBP::init() to initialize the package";

    /**
     * @brief Splits a string into a vector of substrings based on a specified delimiter.
     *
     * This function takes an input string and a delimiter, and splits the input
     * string into multiple substrings wherever the delimiter is found. The
     * resulting substrings are stored in a vector and returned.
     *
     * @param s The input string to be split.
     * @param delimiter The string that serves as the delimiter for splitting.
     *
     * @return std::vector<std::string> A vector containing the substrings resulting
     *         from splitting the input string.
     *
     * @note The delimiter itself is not included in any of the resulting substrings.
     * @note If the delimiter is not found in the input string, the entire input
     *       string will be returned as a single element in the vector.
     * @note Empty substrings are included in the result if multiple delimiters
     *       are found consecutively.
     *
     * @see std::string::find
     * @see std::string::substr
     *
     * @par Example:
     * @code
     * std::string input = "apple,banana,cherry";
     * std::vector<std::string> result = SNBP::split_string(input, ",");
     * // result now contains {"apple", "banana", "cherry"}
     * @endcode
     */
    std::vector<std::string> splitString(const std::string &s, const std::string &delimiter = " ")
    {
        std::vector<std::string> tokens;
        size_t start = 0, end = 0;
        while ((end = s.find(delimiter, start)) != std::string::npos)
        {
            tokens.push_back(s.substr(start, end - start));
            start = end + delimiter.length();
        }
        tokens.push_back(s.substr(start));
        return tokens;
    }

    /**
     * @brief Trims leading whitespace from a string.
     *
     * This function removes all leading whitespace characters from the input string.
     * Whitespace characters include spaces, tabs, line feeds, carriage returns, etc.
     *
     * @param s The string to be trimmed. This parameter is passed by reference and modified in-place.
     *
     * @return std::string& A reference to the modified string, allowing for method chaining.
     *
     * @note This function modifies the original string; it does not create a copy.
     * @note The function uses the standard C++ isspace() function to determine what constitutes whitespace.
     *
     * @see std::isspace
     * @see rtrim
     * @see trim
     *
     * @par Example:
     * @code
     * std::string str = "   Hello, World!";
     * SNBP::ltrim(str);
     * std::cout << str; // Outputs: "Hello, World!"
     * @endcode
     */
    std::string &ltrim(std::string &s)
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
                                        { return !std::isspace(ch); }));
        return s;
    }

    /**
     * @brief Trims trailing whitespace from a string.
     *
     * This function removes all trailing whitespace characters from the input string.
     * Whitespace characters include spaces, tabs, line feeds, carriage returns, etc.
     *
     * @param s The string to be trimmed. This parameter is passed by reference and modified in-place.
     *
     * @return std::string& A reference to the modified string, allowing for method chaining.
     *
     * @note This function modifies the original string; it does not create a copy.
     * @note The function uses the standard C++ isspace() function to determine what constitutes whitespace.
     * @note The function uses reverse iterators for efficient trimming from the end of the string.
     *
     * @see std::isspace
     * @see ltrim
     * @see trim
     *
     * @par Example:
     * @code
     * std::string str = "Hello, World!   ";
     * SNBP::rtrim(str);
     * std::cout << str; // Outputs: "Hello, World!"
     * @endcode
     */
    std::string &rtrim(std::string &s)
    {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch)
                             { return !std::isspace(ch); })
                    .base(),
                s.end());
        return s;
    }

    /**
     * @brief Trims both leading and trailing whitespace from a string.
     *
     * This function removes all leading and trailing whitespace characters from the input string.
     * It combines the functionality of ltrim() and rtrim() into a single function call.
     *
     * @param s The string to be trimmed. This parameter is passed by reference and modified in-place.
     *
     * @return std::string& A reference to the modified string, allowing for method chaining.
     *
     * @note This function modifies the original string; it does not create a copy.
     * @note The function uses the standard C++ isspace() function to determine what constitutes whitespace.
     * @note This function is equivalent to calling ltrim() followed by rtrim().
     *
     * @see std::isspace
     * @see ltrim
     * @see rtrim
     *
     * @par Example:
     * @code
     * std::string str = "   Hello, World!   ";
     * SNBP::trim(str);
     * std::cout << str; // Outputs: "Hello, World!"
     * @endcode
     */
    std::string &trim(std::string &s)
    {
        return ltrim(rtrim(s));
    }

    /**
     * @brief Enables ANSI escape sequences for console output.
     *
     * This function enables ANSI escape sequence processing for the console output,
     * allowing the use of colored text and other formatting in the console.
     * It's typically used in Windows environments where ANSI support is not enabled by default.
     *
     * @throw std::runtime_error if unable to get or set console mode.
     *
     * @note This function uses Windows API functions GetStdHandle, GetConsoleMode, and SetConsoleMode.
     * @note ANSI escape sequences are used for text formatting, colors, cursor movement, etc.
     * @note This function should be called once at the beginning of the program if ANSI support is needed.
     *
     * @see GetStdHandle (Windows API)
     * @see GetConsoleMode (Windows API)
     * @see SetConsoleMode (Windows API)
     *
     * @par Example:
     * @code
     * try {
     *     SNBP::enableAnsiInConsole();
     *     std::cout << "\033[31mThis text is red\033[0m" << std::endl;
     * } catch (const std::runtime_error& e) {
     *     std::cerr << "Error: " << e.what() << std::endl;
     * }
     * @endcode
     */
    int enableAnsiInConsole()
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD mode = 0;
        if (!GetConsoleMode(hConsole, &mode))
        {
            return 1;
        }
        mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        if (!SetConsoleMode(hConsole, mode))
        {
            return 1;
        }

        return 0;
    }

    /**
     * @brief Checks if ANSI escape sequences are enabled in the console.
     *
     * This function determines whether ANSI escape sequence processing is currently
     * enabled for the console output. It's useful for conditional formatting or
     * for deciding whether to use ANSI escape sequences in console output.
     *
     * @return bool Returns true if ANSI escape sequences are enabled, false otherwise.
     *
     * @note This function uses Windows API functions GetStdHandle and GetConsoleMode.
     * @note The function checks for the ENABLE_VIRTUAL_TERMINAL_PROCESSING flag in the console mode.
     *
     * @see GetStdHandle (Windows API)
     * @see GetConsoleMode (Windows API)
     * @see ENABLE_VIRTUAL_TERMINAL_PROCESSING
     *
     * @par Example:
     * @code
     * if (SNBP::isAnsiEnabledInConsole()) {
     *     std::cout << "\033[32mANSI sequences are supported!\033[0m" << std::endl;
     * } else {
     *     std::cout << "ANSI sequences are not supported." << std::endl;
     * }
     * @endcode
     */
    bool isAnsiEnabledInConsole()
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD mode = 0;
        if (!GetConsoleMode(hConsole, &mode))
        {
            return false;
        }
        return (mode & ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0;
    }

    /**
     * @brief Validates a hex color code.
     *
     * This function checks if a given string represents a valid hexadecimal color code.
     * A valid hex color code starts with a '#' followed by exactly 6 hexadecimal digits.
     *
     * @param colorCode The string to be validated as a hex color code.
     *
     * @return bool Returns true if the color code is a valid hex color, false otherwise.
     *
     * @note The function is case-insensitive, so both uppercase and lowercase letters are accepted.
     * @note The function uses a regular expression for validation.
     *
     * @see std::regex
     * @see std::regex_match
     *
     * @par Example:
     * @code
     * std::cout << SNBP::isValidHexColor("#FF00FF") << std::endl; // Outputs: 1 (true)
     * std::cout << SNBP::isValidHexColor("#G12345") << std::endl; // Outputs: 0 (false)
     * std::cout << SNBP::isValidHexColor("FF00FF") << std::endl;  // Outputs: 0 (false, missing #)
     * @endcode
     */
    bool isValidHexColor(const std::string &colorCode)
    {
        static const std::regex hexPattern("^#([A-Fa-f0-9]{6})$");
        return std::regex_match(colorCode, hexPattern);
    }

    /**
     * @brief Applies a hex color code to a given text string using ANSI escape sequences.
     *
     * This function takes a text string and a hex color code, and returns the text
     * formatted with ANSI escape sequences to display the text in the specified color.
     *
     * @param text The text to be colored.
     * @param colorCode The hex color code to apply (e.g., "#FF0000" for red).
     *
     * @return std::string The text formatted with ANSI escape sequences for the specified color.
     *
     * @note The function first validates the hex color code using isValidHexColor().
     * @note If the color code is invalid, the function returns an error message instead of the formatted text.
     * @note The function converts the hex color to RGB values for use in the ANSI escape sequence.
     * @note The returned string includes a reset sequence (\033[0m) at the end to prevent color bleeding.
     *
     * @see isValidHexColor()
     * @see std::stoi()
     *
     * @par Example:
     * @code
     * std::string coloredText = SNBP::colorText("Hello, World!", "#00FF00");
     * std::cout << coloredText << std::endl; // Outputs "Hello, World!" in green
     * @endcode
     */
    std::string colorText(const std::string &text, const std::string &colorCode)
    {
        if (!isValidHexColor(colorCode))
        {
            return "Invalid color code!";
        }

        int r = std::stoi(colorCode.substr(1, 2), nullptr, 16);
        int g = std::stoi(colorCode.substr(3, 2), nullptr, 16);
        int b = std::stoi(colorCode.substr(5, 2), nullptr, 16);

        return "\033[38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m" + text + "\033[0m";
    }

    /**
     * @brief Formats text as bold using ANSI escape sequences.
     *
     * This function takes a text string and returns it formatted as bold
     * using ANSI escape sequences.
     *
     * @param text The text to be formatted as bold.
     *
     * @return std::string The bold-formatted text.
     *
     * @throw std::runtime_error if ANSI is not enabled in the console.
     *
     * @note The function checks if ANSI is enabled in the console before applying formatting.
     * @note The returned string includes a reset sequence (\033[0m) at the end to prevent style bleeding.
     *
     * @see isAnsiEnabledInConsole()
     *
     * @par Example:
     * @code
     * try {
     *     std::string boldText = SNBP::boldText("Important Message");
     *     std::cout << boldText << std::endl; // Outputs "Important Message" in bold
     * } catch (const std::runtime_error& e) {
     *     std::cerr << "Error: " << e.what() << std::endl;
     * }
     * @endcode
     */
    std::string boldText(const std::string &text)
    {
        if (!isAnsiEnabledInConsole())
        {
            throw std::runtime_error("ANSI is not enabled in the console. Call enableAnsiInConsole() first.");
        }
        return "\033[1m" + text + "\033[0m";
    }

    /**
     * @brief Formats text as italic using ANSI escape sequences.
     *
     * This function takes a text string and returns it formatted as italic
     * using ANSI escape sequences.
     *
     * @param text The text to be formatted as italic.
     *
     * @return std::string The italic-formatted text.
     *
     * @throw std::runtime_error if ANSI is not enabled in the console.
     *
     * @note The function checks if ANSI is enabled in the console before applying formatting.
     * @note The returned string includes a reset sequence (\033[0m) at the end to prevent style bleeding.
     * @note Not all console fonts support italic text. The result may vary depending on the console used.
     *
     * @see isAnsiEnabledInConsole()
     *
     * @par Example:
     * @code
     * try {
     *     std::string italicText = SNBP::italicText("Emphasized text");
     *     std::cout << italicText << std::endl; // Outputs "Emphasized text" in italic
     * } catch (const std::runtime_error& e) {
     *     std::cerr << "Error: " << e.what() << std::endl;
     * }
     * @endcode
     */
    std::string italicText(const std::string &text)
    {
        if (!isAnsiEnabledInConsole())
        {
            throw std::runtime_error("ANSI is not enabled in the console. Call enableAnsiInConsole() first.");
        }
        return "\033[3m" + text + "\033[0m";
    }

    /**
     * @brief Formats text with an underline using ANSI escape sequences.
     *
     * This function takes a text string and returns it formatted with an underline
     * using ANSI escape sequences.
     *
     * @param text The text to be underlined.
     *
     * @return std::string The underlined text.
     *
     * @throw std::runtime_error if ANSI is not enabled in the console.
     *
     * @note The function checks if ANSI is enabled in the console before applying formatting.
     * @note The returned string includes a reset sequence (\033[0m) at the end to prevent style bleeding.
     *
     * @see isAnsiEnabledInConsole()
     *
     * @par Example:
     * @code
     * try {
     *     std::string underlinedText = SNBP::underlineText("Important link");
     *     std::cout << underlinedText << std::endl; // Outputs "Important link" with an underline
     * } catch (const std::runtime_error& e) {
     *     std::cerr << "Error: " << e.what() << std::endl;
     * }
     * @endcode
     */
    std::string underlineText(const std::string &text)
    {
        if (!isAnsiEnabledInConsole())
        {
            throw std::runtime_error("ANSI is not enabled in the console. Call enableAnsiInConsole() first.");
        }
        return "\033[4m" + text + "\033[0m";
    }

    /**
     * @brief Applies multiple text formatting options using ANSI escape sequences.
     *
     * This function takes a text string and applies multiple formatting options
     * (bold, italic, underline, and color) using ANSI escape sequences.
     *
     * @param text The text to be formatted.
     * @param bold Apply bold formatting if true.
     * @param italic Apply italic formatting if true.
     * @param underline Apply underline formatting if true.
     * @param colorCode Hex color code to apply (e.g., "#FF0000" for red).
     *
     * @return std::string The formatted text.
     *
     * @throw std::runtime_error if ANSI is not enabled in the console.
     *
     * @note The function checks if ANSI is enabled in the console before applying formatting.
     * @note The color is applied first, followed by bold, italic, and underline formatting.
     * @note The returned string includes a reset sequence (\033[0m) at the end to prevent style bleeding.
     * @note If the color code is invalid or empty, no color will be applied.
     *
     * @see isAnsiEnabledInConsole()
     * @see colorText()
     *
     * @par Example:
     * @code
     * try {
     *     std::string formattedText = SNBP::formatText("Formatted Text", true, false, true, "#00FF00");
     *     std::cout << formattedText << std::endl; // Outputs "Formatted Text" in green, bold, and underlined
     * } catch (const std::runtime_error& e) {
     *     std::cerr << "Error: " << e.what() << std::endl;
     * }
     * @endcode
     */
    std::string formatText(const std::string &text, bool bold, bool italic, bool underline, const std::string &colorCode)
    {
        if (!isAnsiEnabledInConsole())
        {
            throw std::runtime_error("ANSI is not enabled in the console. Call enableAnsiInConsole() first.");
        }

        std::string result;
        if (!colorCode.empty())
        {
            result += colorText("", colorCode); // Apply color
        }
        if (bold)
            result += "\033[1m";
        if (italic)
            result += "\033[3m";
        if (underline)
            result += "\033[4m";

        result += text + "\033[0m"; // Add text and reset formatting
        return result;
    }

    /**
     * @brief Removes all ANSI escape sequences from a string.
     *
     * This function takes a string that may contain ANSI escape sequences and
     * removes all such sequences, returning the plain text content.
     *
     * @param text The text containing ANSI escape sequences.
     *
     * @return std::string The text with all ANSI escape sequences removed.
     *
     * @note This function uses a regular expression to identify and remove ANSI escape sequences.
     * @note The function will remove all sequences starting with '\033[' and ending with a letter.
     *
     * @see std::regex
     * @see std::regex_replace
     *
     * @par Example:
     * @code
     * std::string coloredText = "\033[31mRed Text\033[0m \033[1mBold Text\033[0m";
     * std::string plainText = SNBP::stripAnsiEscapeCodes(coloredText);
     * std::cout << plainText << std::endl; // Outputs: "Red Text Bold Text"
     * @endcode
     */
    std::string stripAnsiEscapeCodes(const std::string &text)
    {
        static const std::regex ansiEscapePattern("\033\\[[0-9;]*[A-Za-z]");
        return std::regex_replace(text, ansiEscapePattern, "");
    }

    /**
     * @brief Truncates a string to a specified length, adding an ellipsis if truncated.
     *
     * This function takes an input string and a maximum length, and returns a string
     * that is no longer than the specified length. If the input string is longer than
     * the maximum length, it is truncated and an ellipsis (...) is appended.
     *
     * @param text The input text to truncate.
     * @param maxLength The maximum length of the truncated string, including the ellipsis.
     *
     * @return std::string The truncated string.
     *
     * @note If the input string is shorter than or equal to maxLength, it is returned unchanged.
     * @note The ellipsis (...) counts towards the maxLength.
     * @note If maxLength is less than 4, the function will still attempt to add the ellipsis,
     *       which may result in a string longer than maxLength.
     *
     * @par Example:
     * @code
     * std::string longText = "This is a very long string that needs truncating";
     * std::string truncated = SNBP::truncateString(longText, 20);
     * std::cout << truncated << std::endl; // Outputs: "This is a very lon..."
     * @endcode
     */
    std::string truncateString(const std::string &text, size_t maxLength)
    {
        if (text.length() <= maxLength)
        {
            return text;
        }
        return text.substr(0, maxLength - 3) + "...";
    }

    /**
     * @brief Centers a string within a field of a given width.
     *
     * This function takes a string and centers it within a field of a specified width.
     * If the string is shorter than the specified width, padding characters are added
     * on both sides to center the string. If the string is longer than or equal to the
     * specified width, it is returned unchanged.
     *
     * @param text The text to be centered.
     * @param width The width of the field in which to center the text.
     * @param fillChar The character to use for padding (default is space).
     *
     * @return std::string The centered string.
     *
     * @note If the difference between the field width and the string length is odd,
     *       the extra padding character is added to the right side.
     * @note If the string is longer than the specified width, no truncation occurs.
     *
     * @par Example:
     * @code
     * std::string centered = SNBP::centerString("Hello", 11, '-');
     * std::cout << centered << std::endl; // Outputs: "---Hello---"
     * @endcode
     */
    std::string centerString(const std::string &text, size_t width, char fillChar)
    {
        if (text.length() >= width)
        {
            return text;
        }
        size_t padding = width - text.length();
        size_t leftPadding = padding / 2;
        size_t rightPadding = padding - leftPadding;
        return std::string(leftPadding, fillChar) + text + std::string(rightPadding, fillChar);
    }

    /**
     * @brief Wraps text to a specified line length.
     *
     * This function takes an input text and wraps it to ensure that no line exceeds
     * the specified maximum line length. It attempts to break lines at word boundaries
     * to maintain readability.
     *
     * @param text The input text to wrap.
     * @param lineLength The maximum length of each line.
     *
     * @return std::string The wrapped text.
     *
     * @note The function does not break words that are longer than the specified line length.
     * @note The function preserves existing line breaks in the input text.
     * @note Consecutive spaces between words are collapsed to a single space.
     *
     * @see std::istringstream
     * @see std::ostringstream
     *
     * @par Example:
     * @code
     * std::string longText = "This is a long string that needs to be wrapped to multiple lines for better readability.";
     * std::string wrapped = SNBP::wrapText(longText, 20);
     * std::cout << wrapped << std::endl;
     * // Outputs:
     * // This is a long
     * // string that needs to
     * // be wrapped to
     * // multiple lines for
     * // better readability.
     * @endcode
     */
    std::string wrapText(const std::string &text, size_t lineLength)
    {
        std::istringstream words(text);
        std::ostringstream wrapped;
        std::string word;

        size_t currentLineLength = 0;
        while (words >> word)
        {
            if (currentLineLength + word.length() > lineLength)
            {
                wrapped << '\n';
                currentLineLength = 0;
            }
            if (currentLineLength > 0)
            {
                wrapped << ' ';
                currentLineLength++;
            }
            wrapped << word;
            currentLineLength += word.length();
        }

        return wrapped.str();
    }

    std::string leftPad(std::string str, int amount)
    {
        if (amount <= 0)
        {
            std::exit(1);
            return "Exit code 1";
        }

        return std::string(amount, ' ') + str;
    }

    std::string rightPad(std::string str, int amount)
    {
        if (amount <= 0)
        {
            std::exit(1);
            return "Exit code 1";
        }

        return str + std::string(amount, ' ');
    }

    std::string pad(std::string str, int amount)
    {
        if (amount <= 0)
        {
            std::exit(1);
            return "Exit code 1";
        }

        return rightPad(leftPad(str, amount), amount);
    }
} // namespace SNBP