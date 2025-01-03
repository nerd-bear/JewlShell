#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include <vector>
#include <filesystem>

#include "snbp.hpp"

const std::string CLI_PREFIX_DELIMITER = ":";
const std::string SHELL_PREFIX = "SSO";

enum CommandResult {
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
    {CR_ERROR, "CR_ERROR"}
};


void printStartupMessage();
void clearTerminal();
void standardShellOutput(const std::string& content, const std::string& end = "\n", const std::string& prefix = SHELL_PREFIX);
CommandResult echoCommand(const std::vector<std::string>& args);
CommandResult exitCommand(const std::vector<std::string>& args);
CommandResult changeDirectoryCommand(const std::vector<std::string>& args);
CommandResult createDirectoryCommand(const std::vector<std::string>& args);
CommandResult directoryListCommand(const std::vector<std::string>& args);
CommandResult clearScreenCommand(const std::vector<std::string>& args);
CommandResult runFileCommand(const std::vector<std::string>& args);

std::vector<std::string> current_path_fragments = {"C:", "WINDOWS", "system32"};

std::string getPathAsString() {
    std::string _path;

    for (const auto& path_fragment : current_path_fragments) {
        _path = _path + "/" + path_fragment;
    }

    _path.erase(0, 1);

    return _path;
}

void setPathFromString(const std::string& path) {
    std::vector<std::string> split_path = SNBP::splitString(path, "/");

    current_path_fragments = split_path;
}

bool end_shell = false;
std::unordered_map<std::string, std::function<CommandResult(const std::vector<std::string>&)>> command_map = {
    {"echo", echoCommand},
    {"exit", exitCommand},
    {"cd", changeDirectoryCommand},
    {"mkdir", createDirectoryCommand},
    {"ls", directoryListCommand},
    {"clear", clearScreenCommand},
    {"rf", runFileCommand}
};

int main() {
    SNBP::enableAnsiInConsole();

    clearTerminal();
    printStartupMessage();

    while (!end_shell) {
        std::string input;
        std::cout << getPathAsString() << " " << CLI_PREFIX_DELIMITER << " ";
        std::getline(std::cin, input);

        input = SNBP::trim(input);

        std::vector<std::string> split_input = SNBP::splitString(input, " ");

        if (split_input.empty()) {
            continue;
        }

        std::string command_name = split_input[0];
        std::vector<std::string> args(split_input.begin() + 1, split_input.end());

        if (input == "") {
            continue;
        }

        if (command_map.find(command_name) == command_map.end()) {
            standardShellOutput("ERROR \"" + command_name + "\" is not a executable file, custom script or command! Please check your spelling or if the module that the command may originate from is enabled");
            continue;
        }

        CommandResult result = command_map[command_name](args);
        if (result != CR_SUCCESS) {
            standardShellOutput("Command \"" + command_name + "\" failed with error code " + std::to_string(result) + "(" + CommandResultNames[result] + ")");
        }
    }   

    return 0;
}

void printStartupMessage() {
    std::cout << "--------------------------------------------------------------------------------------------------------\n\
 __             _     _             __ _          _ _ \n\
/ _\\ __ _ _   _(_)___| |__  _   _  / _\\ |__   ___| | |\n\
\\ \\ / _` | | | | / __| '_ \\| | | | \\ \\| '_ \\ / _ \\ | |\n\
_\\ \\ (_| | |_| | \\__ \\ | | | |_| | _\\ \\ | | |  __/ | |\n\
\\__/\\__, |\\__,_|_|___/_| |_|\\__, | \\__/_| |_|\\___|_|_|\n\
       |_|                  |___/                     \n\
\n\
Use help/h/info to get detailed information about commands and terminal structure.\n\
\n\
Created By Nerd Bear to make a better shell for modern systems!\n\
As I am a beginner programmer in C++ I created this little project to test new things I learned!\n\
\n\
Github: https://github.com/nerd-bear Discord: nerd.bear\n\
--------------------------------------------------------------------------------------------------------\n";
}

void clearTerminal() {
#if defined _WIN32
    std::system("cls");
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    std::system("clear");
#endif
}

void standardShellOutput(const std::string& content, const std::string& end, const std::string& prefix) {
    std::cout << prefix << " >> " << content << end;
}

CommandResult exitCommand(const std::vector<std::string>& args) {
    end_shell = true;
    standardShellOutput("Exiting shell...");

    return CR_SUCCESS;
}

#include "commands/echo_command.cpp"
#include "commands/change_directory_command.cpp"
#include "commands/clear_command.cpp"
#include "commands/list_directory_command.cpp"
#include "commands/create_directory_command.cpp"
#include "commands/run_file_command.cpp"