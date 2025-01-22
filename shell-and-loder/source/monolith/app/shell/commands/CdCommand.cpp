#include "CdCommand.hpp"
#include <unistd.h>
#include <iostream>

int CdCommand::Execute(const std::vector<std::string>& args){
    if(args.empty()){
        std::cerr << "cd: " << "No argument passed" << "\n";
        return -1; 
    }
    if(chdir(args[0].c_str()) != 0){
        std::cerr << "cd: " << args[0] << ": No directory" << '\n';
        return -1;
    }
    shell_->SetDirectory();
    return 0;
}