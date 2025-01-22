#include "monolith/app/shell/commands/Command.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>


class LsCommand : public Command {
    public:
        explicit LsCommand(Shell* shell) : Command(shell){
        }
        int Execute(const std::vector<std::string>& args) override;
};