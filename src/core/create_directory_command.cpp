CommandResult createDirectoryCommand(const std::vector<std::string> &args, const std::vector<std::string> &flags)
{
    if (args.size() < 1)
    {
        return CR_MISSING_ARGUMENTS;
    }

    try
    {
        if (std::filesystem::create_directory(getPathAsString() + "/" + args[0]))
        {
            return CR_SUCCESS;
        }
        else
        {
            return CR_ERROR;
        }
    }
    catch (const std::exception &e)
    {
        return CR_ERROR;
    }
}