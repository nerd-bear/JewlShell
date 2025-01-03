CommandResult directoryListCommand(const std::vector<std::string>& args) {
    int total_files = 0;
    int total_dirs = 0;

    for (const auto & entry : std::filesystem::directory_iterator(getPathAsString())) {
        standardShellOutput(entry.path().string());

        total_files++;    
    }

    standardShellOutput("Scanned and displayed " + std::to_string(total_files) + " file(s) in " + getPathAsString());

    return CR_SUCCESS;
}