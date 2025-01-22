
#pragma once
#include "../Shell.hpp"

class Command {
    public: 
        virtual ~Command() = default;
        explicit Command(Shell* shell);
        virtual int Execute(const std::vector<std::string>& args) = 0;
    protected:
        Shell* shell_;
};