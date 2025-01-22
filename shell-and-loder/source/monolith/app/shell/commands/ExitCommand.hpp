#pragma once
#include "Command.hpp"


class ExitCommand : public Command {
    public:
        explicit ExitCommand(Shell* shell) : Command(shell) {}
        int Execute(const std::vector<std::string>& args) override;
};