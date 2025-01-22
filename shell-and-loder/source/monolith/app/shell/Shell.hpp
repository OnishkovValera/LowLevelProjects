#pragma once
#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;


class Shell{
    public: 
        Shell();
        void Start();
        void Run();
        void Stop();
        fs::path GetDirectory();
        void SetDirectory();
        std::vector<std::string> ParseCommand(const std::string& command);
    protected:
        fs::path current_directory_; 
        bool is_running_;

};
