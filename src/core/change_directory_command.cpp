CommandResult changeDirectoryCommand(const std::vector<std::string>& args) {
    if (args.size() < 1) {
        return CR_MISSING_ARGUMENTS;
    }

    std::string new_path = args[0];
    std::filesystem::path target_path;

    if (new_path[0] == '.' && new_path[1] == '/') {
        new_path.erase(0, 2);
        target_path = std::filesystem::path(getPathAsString()) / new_path;
    } 
    else if (new_path[0] == '.' && new_path[1] == '.') {
        target_path = std::filesystem::path(getPathAsString()).parent_path();
    } 
    else {
        target_path = std::filesystem::path(new_path);
    }

    std::string target_path_str = target_path.string();
    std::replace(target_path_str.begin(), target_path_str.end(), '\\', '/');

    if (!std::filesystem::exists(target_path)) {
        standardShellOutput("CR_ERROR : Directory could not be found! \"" + target_path_str + "\"");
        return CR_WRONG_ARGUMENTS;
    }

    setPathFromString(target_path_str);
    return CR_SUCCESS;
}