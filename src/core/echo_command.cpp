CommandResult echoCommand(const std::vector<std::string>& args) {
    std::string final_string;

    for (const auto& arg : args) {
        final_string += " " + arg;
    }

    final_string.erase(0, 1);

    standardShellOutput(final_string);

    return CR_SUCCESS;
}