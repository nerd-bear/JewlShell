CommandResult exitCommand(const std::vector<std::string>& args) {
    end_shell = true;
    standardShellOutput("Exiting shell...");

    return CR_SUCCESS;
}