CommandResult operatingSystemCommand(const std::vector<std::string> &args)
{
    std::string osName = char(std::toupper(getOsPlatformName()[0])) + getOsPlatformName().erase(0, 1);
    standardShellOutput("Operating System: " + osName);

    return CR_SUCCESS;
}