CommandResult clearScreenCommand(const std::vector<std::string>& args) {
    clearTerminal();

    return CR_SUCCESS;
}