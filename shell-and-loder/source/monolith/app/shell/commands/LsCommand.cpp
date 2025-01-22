#include "LsCommand.hpp"
#include <algorithm>
#include <iostream>

namespace fs = std::filesystem;

int LsCommand::Execute(const std::vector<std::string>& args){

    if(args.size() > 1){
        std::cout << "ls: too many arguments" << '\n';
        return 1;
    }

    std::vector<fs::path> entries;

    try {
        if (fs::exists(shell_->GetDirectory()) && fs::is_directory(shell_->GetDirectory())) {
            for (const auto& entry : fs::directory_iterator(shell_->GetDirectory())) {
                entries.push_back(entry.path());
            }
        } else {
            std::cerr << "Error: Directory '" << shell_->GetDirectory() << "' does not exist or is not a directory." << std::endl;
        }
    } catch (const fs::filesystem_error& ex) {
        std::cerr << "Error reading directory: " << ex.what();
    }
    std::sort(entries.begin(), entries.end());


    for (const auto& entry : entries) {
        std::cout << entry.filename().string() <<  (fs::is_directory(entry) ? "(directory)   " : "   ");
    }
    std::cout << "\n";
    return 0;
}