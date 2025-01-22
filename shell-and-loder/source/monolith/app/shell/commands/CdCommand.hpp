


#include "monolith/app/shell/commands/Command.hpp"

class CdCommand : public Command{
    public:
        explicit CdCommand(Shell* shell) : Command(shell){}
        int Execute(const std::vector<std::string>& args) override;
};