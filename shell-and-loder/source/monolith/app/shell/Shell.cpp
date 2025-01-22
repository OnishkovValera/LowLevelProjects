#include "Shell.hpp"
#include <iostream>
#include <filesystem>
#include "CommandInvoker.hpp"

namespace fs = std::filesystem;


Shell::Shell() : is_running_(true){}

void Shell::Start(){
    this->is_running_ = true;
    this->Run();
}

void Shell::Run(){
    this->SetDirectory();
    CommandInvoker command_invoker = CommandInvoker(this);
    while(is_running_){
        std::cout << this->current_directory_.c_str() << "# ";
        std::string command;
        std::getline(std::cin, command);
        std::vector<std::string> args = ParseCommand(command);
        if(args.empty() or args[0].empty()){
            continue;
        }
        command_invoker.Execute(args[0], std::vector(args.begin() + 1, args.end()));
    }

}

void Shell::Stop(){
    is_running_ = false;
}

std::vector<std::string> Shell::ParseCommand(const std::string& command){
    std::vector<std::string> args;
    std::string arg;
    for(char character : command){
        if(character == ' '){
            if(!arg.empty()){
                args.push_back(arg);
                arg = "";
            }
        } else {
            arg += character;
        }
    }
    if(!arg.empty()){
        args.push_back(arg);
    }
    return args;
}

void Shell::SetDirectory(){
    this->current_directory_ = fs::current_path();
}


fs::path Shell::GetDirectory(){
    return this->current_directory_;
}