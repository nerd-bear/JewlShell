CommandResult getLocationCommand(const std::vector<std::string> &args)
{
    std::string location = getPathAsString();
    std::string formatedLocation = location;
    int underlineLength = location.length();

    if (underlineLength < 4)
    {
        underlineLength = 4;
    }

    standardShellOutput("\n\nPath \n" + std::string(underlineLength, '-') + "\n" + formatedLocation + "\n");
    return CR_SUCCESS;
}