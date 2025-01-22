#include <map>
#include "Shell.hpp"
#include "monolith/app/shell/commands/Command.hpp"



class CommandInvoker{
    public:
        explicit CommandInvoker(Shell* shell);

        int Execute(const std::string& command, const std::vector<std::string>& args);
    private:
        Shell* shell_;
        std::map<std::string, std::unique_ptr<Command>> commands_;
        void RegisterCommands();
};