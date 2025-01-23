CommandResult exitCommand(const std::vector<std::string> &args, const std::vector<std::string> &flags)
{
    end_shell = true;
    standardShellOutput("Exiting shell...");

    return CR_SUCCESS;
}