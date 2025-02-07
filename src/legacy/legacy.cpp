#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include <vector>
#include <filesystem>
#include <cstdlib>

#include <boost/algorithm/string.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "snbp.hpp"

const std::string CLI_PREFIX_DELIMITER = ":";
const std::string SHELL_PREFIX = "SSO";

enum CommandResult
{
    CR_SUCCESS,
    CR_WRONG_ARGUMENTS,
    CR_MISSING_ARGUMENTS,
    CR_UNKNOWN,
    CR_ERROR
};

std::unordered_map<CommandResult, std::string> CommandResultNames = {
    {CR_SUCCESS, "CR_SUCCESS"},
    {CR_WRONG_ARGUMENTS, "CR_WRONG_ARGUMENTS"},
    {CR_MISSING_ARGUMENTS, "CR_MISSING_ARGUMENTS"},
    {CR_UNKNOWN, "CR_UNKNOWN"},
    {CR_ERROR, "CR_ERROR"}};

void printStartupMessage();
void clearTerminal();
void standardShellOutput(const auto &content, const std::string &end = "\n", const std::string &prefix = SHELL_PREFIX);
std::string getDefaultPath();
CommandResult echoCommand(const std::vector<std::string> &args, const std::vector<std::string> &flags);
CommandResult exitCommand(const std::vector<std::string> &args, const std::vector<std::string> &flags);
CommandResult changeDirectoryCommand(const std::vector<std::string> &args, const std::vector<std::string> &flags);
CommandResult createDirectoryCommand(const std::vector<std::string> &args, const std::vector<std::string> &flags);
CommandResult directoryListCommand(const std::vector<std::string> &args, const std::vector<std::string> &flags);
CommandResult clearScreenCommand(const std::vector<std::string> &args, const std::vector<std::string> &flags);
CommandResult runFileCommand(const std::vector<std::string> &args, const std::vector<std::string> &flags);
CommandResult irhCommand(const std::vector<std::string> &args, const std::vector<std::string> &flags);
CommandResult operatingSystemCommand(const std::vector<std::string> &args, const std::vector<std::string> &flags);
CommandResult getLocationCommand(const std::vector<std::string> &, const std::vector<std::string> &flags);
CommandResult littleCommand(const std::vector<std::string> &, const std::vector<std::string> &flags); // EASTER EGG COMMAND
CommandResult ipconfigCommand(const std::vector<std::string> &args, const std::vector<std::string> &flags);

std::vector<std::string> current_path_fragments;

std::string getPathAsString()
{
    std::string _path;

    for (const auto &path_fragment : current_path_fragments)
    {
        _path = _path + "/" + path_fragment;
    }

    _path.erase(0, 1);

    return _path;
}

void setPathFromString(const std::string &path)
{
    std::vector<std::string> split_path = SNBP::splitString(path, "/");

    current_path_fragments = split_path;
}

std::string getDefaultPath()
{
#ifdef WIN32
    return "C:/";
#elif defined(__unix__) || defined(__gnu_linux__) || defined(__linux__)
    return "/usr/";
#endif

    std::cout << "Unsupported OS" << std::endl;
    exit(-1);
    return "";
}

std::string getOsPlatformName()
{
#ifdef WIN32
    return "windows";
#elif defined(__unix__) || defined(__gnu_linux__) || defined(__linux__)
    return "linux";
#endif

    return "Unsupported OS";
}

void setConsoleTitle(const std::string title)
{
#warning This function will need to be updated once a proper GUI implementation is available.

    SetConsoleTitleA(title.c_str());
}

bool end_shell = false;
std::unordered_map<std::string, std::function<CommandResult(const std::vector<std::string> &, const std::vector<std::string> &)>> command_map = {
    {"echo", echoCommand},
    {"exit", exitCommand},
    {"cd", changeDirectoryCommand},
    {"mkdir", createDirectoryCommand},
    {"ls", directoryListCommand},
    {"clear", clearScreenCommand},
    {"rf", runFileCommand},
    {"irh", irhCommand},
    {"os", operatingSystemCommand},
    {"pwd", getLocationCommand},
    {"ipconfig", ipconfigCommand},
    {"little", littleCommand}};

int main()
{
    setPathFromString(getDefaultPath());
    setConsoleTitle("Jewel Shell");
    clearTerminal();
    printStartupMessage();

    while (!end_shell)
    {
        std::string input;
        std::cout << getPathAsString() << " " << CLI_PREFIX_DELIMITER << " ";
        std::getline(std::cin, input);

        input = SNBP::trim(input);

        std::vector<std::string> split_input = SNBP::splitString(input, " ");
        std::vector<std::string> command_flags;

        for (int i = 0; i < split_input.size(); i++)
        {
            if (split_input[i][0] == '-')
            {
                command_flags.push_back(split_input[i]);
                split_input.erase(split_input.begin() + i);
                i--;
            }
        }

        if (split_input.empty())
        {
            continue;
        }

        std::string command_name = split_input[0];
        std::vector<std::string> args(split_input.begin() + 1, split_input.end());

        if (input == "")
        {
            continue;
        }

        if (command_map.find(command_name) == command_map.end())
        {
            standardShellOutput("ERROR \"" + command_name + "\" is not a executable file, custom script or command! Please check your spelling or if the module that the command may originate from is enabled");
            continue;
        }

        CommandResult result = command_map[command_name](args, command_flags);

        if (result != CR_SUCCESS)
        {
            standardShellOutput("Command \"" + command_name + "\" failed with error code " + std::to_string(result) + "(" + CommandResultNames[result] + ")");
        }

        std::cin.clear();
    }

    return 0;
}

void printStartupMessage()
{
    std::cout << "--------------------------------------------------------------------------------------------------------\n\
Welcome to JewelShell ("
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
#warning This function will need to be updated once a proper GUI implementation is available.

#if defined _WIN32
    std::system("cls");
#elif defined(__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    std::system("clear");
#endif
}

void standardShellOutput(const auto &content, const std::string &end, const std::string &prefix)
{
    std::cout << prefix << " >> " << content << end;
}

#include "core/echo_command.cpp"
#include "core/change_directory_command.cpp"
#include "core/clear_command.cpp"
#include "core/list_directory_command.cpp"
#include "core/create_directory_command.cpp"
#include "core/run_file_command.cpp"
#include "core/exit_command.cpp"
#include "core/integrated_request_handler.cpp"
#include "core/operating_system_command.cpp"
#include "core/get_location_command.cpp"
#include "core/little_command.cpp"

CommandResult ipconfigCommand(const std::vector<std::string> &args, const std::vector<std::string> &flags)
{
    try
    {
        boost::asio::io_context io_context;

        char hostname[256];
        gethostname(hostname, sizeof(hostname));

        boost::asio::ip::tcp::tcp::resolver resolver(io_context);

        auto endpoints = resolver.resolve(hostname, "");

        for (const auto &endpoint : endpoints)
        {
            std::cout << "IP Address: " << endpoint.endpoint().address().to_string() << std::endl;
        }
    }

    catch (std::exception &e)
    {
        return CR_ERROR;
    }

    return CR_SUCCESS;
}