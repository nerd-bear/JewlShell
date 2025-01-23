CommandResult littleCommand(const std::vector<std::string> &args, const std::vector<std::string> &flags) // EASTER EGG COMMAND
{
    standardShellOutput("Britain?");
    standardShellOutput("Little Britain?");
    standardShellOutput("Thank you Matt Lucas and David Walliams!");

    return CR_SUCCESS;
}