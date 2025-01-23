#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <filesystem>
#include <cstdlib>
#include <algorithm>

#include <boost/algorithm/string.hpp>

#ifdef _WIN32
const char PATH_DELIMITER = ';';
#else
const char PATH_DELIMITER = ':';
#endif

const std::string CLI_PREFIX_DELIMITER = ":";
const std::string SHELL_PREFIX = "SSO";

void printStartupMessage();
void clearTerminal();
void standardShellOutput(const std::string &content, const std::string &end = "\n", const std::string &prefix = SHELL_PREFIX);
std::string getDefaultPath();

std::string getDefaultPath()
{
#ifdef _WIN32
    return "C:/";
#elif defined(__unix__) || defined(__gnu_linux__) || defined(__linux__)
    return "/usr/";
#endif

    std::cout << "getDefaultPath() Unsupported OS" << std::endl;
    exit(-1);
    return "";
}

std::string getOsPlatformName()
{
#ifdef _WIN32
    return "windows";
#elif defined(__unix__) || defined(__gnu_linux__) || defined(__linux__)
    return "linux";
#endif

    return "getOsPlatformName() Unsupported OS";
}

void setConsoleTitle(const std::string title)
{
#ifdef _WIN32
    // SetConsoleTitleA(title.c_str());
#else
    // Linux/macOS: Use ANSI escape codes to set terminal title
    std::cout << "\033]0;" << title << "\007";
#endif
}

int executeFile(const std::string path)
{
#ifdef _WIN32
#include <windows.h>

    STARTUPINFO info = {sizeof(info)};
    PROCESS_INFORMATION processInfo;
    if (CreateProcess(path, cmd, NULL, NULL, TRUE, 0, NULL, NULL, &info, &processInfo))
        ;
    {
        WaitForSingleObject(processInfo.hProcess, INFINITE);
        CloseHandle(processInfo.hProcess);
        CloseHandle(processInfo.hThread);
    }
#else
    // Linux/macOS: Use ANSI escape codes to set terminal title
    std::cout << "\033]0;" << title << "\007";
#endif
}

std::vector<std::string> getSystemPathDirectories()
{
    std::vector<std::string> directories;

    const char *pathEnv = std::getenv("PATH");
    if (!pathEnv)
    {
        std::cerr << "PATH environment variable not found!" << std::endl;
        return directories;
    }

    std::stringstream pathStream(pathEnv);
    std::string dir;
    while (std::getline(pathStream, dir, PATH_DELIMITER))
    {
        if (!dir.empty())
        {
            directories.push_back(dir);
        }
    }

    return directories;
}

std::vector<std::string> getPathsOfFilesOfDirectory(std::string path)
{
    std::vector<std::string> directories;

    std::stringstream pathStream(path);
    std::string dir;
    while (std::getline(pathStream, dir, PATH_DELIMITER))
    {
        if (!dir.empty())
        {
            directories.push_back(dir);
        }
    }

    return directories;
}

std::string findCommandPath(const std::string &command_name)
{
    std::vector<std::string> pathDirs = getSystemPathDirectories();

    for (const auto &dir : pathDirs)
    {
        std::filesystem::path dir_path(dir);
        if (!std::filesystem::exists(dir_path) || !std::filesystem::is_directory(dir_path))
        {
            continue;
        }

        for (const auto &entry : std::filesystem::directory_iterator(dir_path))
        {
            if (entry.is_regular_file() && entry.path().filename() == command_name)
            {
                return entry.path().string();
            }
        }
    }

    std::filesystem::path current_dir = std::filesystem::current_path();
    for (const auto &entry : std::filesystem::directory_iterator(current_dir))
    {
        if (entry.is_regular_file() && entry.path().filename() == command_name)
        {
            return entry.path().string();
        }
    }

    return "";
}

bool end_shell = false;

int main()
{
    std::filesystem::current_path(getDefaultPath());
    setConsoleTitle("Jewl Shell");
    clearTerminal();
    printStartupMessage();

    while (!end_shell)
    {
        std::string input;

        std::string coutPath = std::filesystem::current_path().string();
        boost::replace_all(coutPath, "\\", "/");

        std::cout << coutPath << " " << CLI_PREFIX_DELIMITER << " ";
        std::getline(std::cin, input);

        boost::trim(input);

        if (input.empty())
        {
            continue;
        }

        std::vector<std::string> split_input;
        boost::split(split_input, input, boost::is_any_of(" "));

        std::string command_name = split_input[0];
        std::vector<std::string> args(split_input.begin() + 1, split_input.end());

        if (command_name == "exit" || command_name == "quit")
        {
            end_shell = true;
            continue;
        }

        else if (command_name == "cd")
        {
            if (args.size() == 1)
            {
                std::filesystem::current_path(split_input[1]);
            }

            continue;
        }

        std::string command_path = findCommandPath(command_name);

        if (!command_path.empty())
        {
            std::string args_string;
            for (const std::string &arg : args)
            {
                args_string += " " + arg;
            }

            int result = std::system((command_path + args_string).c_str());
            if (result != 0)
            {
                standardShellOutput("Command execution failed with code: " + std::to_string(result));
            }
        }
        else
        {
            standardShellOutput("Command not found in PATH: " + command_name);
        }
    }

    return 0;
}

void printStartupMessage()
{
    std::cout << "--------------------------------------------------------------------------------------------------------\n\
Welcome to JewlShell ("
              << char(std::toupper(getOsPlatformName()[0])) << getOsPlatformName().erase(0, 1) << ")\n\
\n\
\n\
Use help/h/info to get detailed information about commands and terminal structure.\n\
\n\
Created By Nerd Bear to make a better shell for modern systems!\n\
As I am a beginner programmer in C++ I created this little project to test new things I learned!\n\
\n\
Github: https://github.com/nerd-bear Discord: nerd.bear\n\
--------------------------------------------------------------------------------------------------------\n";
}

void clearTerminal()
{
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

void standardShellOutput(const std::string &content, const std::string &end, const std::string &prefix)
{
    std::cout << prefix << " >> " << content << end;
}