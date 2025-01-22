#include "CommandInvoker.hpp"
#include "commands/LsCommand.hpp"
#include "commands/CdCommand.hpp"
#include "monolith/app/shell/commands/ExitCommand.hpp"
#include "monolith/app/shell/commands/RunCommand.hpp"


CommandInvoker::CommandInvoker(Shell* shell) : shell_(shell){
    RegisterCommands();
}


void CommandInvoker::RegisterCommands(){
    commands_.insert({"ls", std::make_unique<LsCommand>(shell_)});
    commands_.insert({"cd", std::make_unique<CdCommand>(shell_)});
    commands_.insert({"exit", std::make_unique<ExitCommand>(shell_)});
    commands_.insert({"run", std::make_unique<RunCommand>(shell_)});

}

int CommandInvoker::Execute(const std::string& command, const std::vector<std::string>& args){
    if(commands_.find(command) == commands_.end()){
        std::cerr << "Command not found: " << command << '\n';
        return 1;
    }
    return commands_[command]->Execute(args);
}