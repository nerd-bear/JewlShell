CommandResult clearScreenCommand(const std::vector<std::string> &args, const std::vector<std::string> &flags)
{
    clearTerminal();

    return CR_SUCCESS;
}