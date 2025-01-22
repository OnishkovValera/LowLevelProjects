#include "RunCommand.hpp"
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>

int RunCommand::Execute(const std::vector<std::string>& args){
    if(args.empty()){
        std::cout << "Execute command: no arguments found" << "\n";
        return -1; 
    }

    std::vector<char*> c_args;
    for (auto& arg : args) {
        c_args.push_back(const_cast<char*>(arg.c_str()));
    }
    
    c_args.push_back(nullptr);

    char* const* argv = c_args.data();
    pid_t pid = vfork(); 
    if (pid < 0) {
        perror("vfork failed");
        return -1 ;
    } 

    if (pid == 0) {
        std::cout << argv[0];
        if (execvp(argv[0], argv) < 0) {
            perror("execvp failed");
            _exit(EXIT_FAILURE);
        }
    } else {
        int status = 0;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            std::cout << "Process exited with status: " << WEXITSTATUS(status) << '\n';
        } else {
            std::cerr << "Process terminated abnormally" << '\n';
        }
    }
    return 0;
}