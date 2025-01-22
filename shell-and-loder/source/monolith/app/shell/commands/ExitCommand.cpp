#include "ExitCommand.hpp"

int ExitCommand::Execute(const std::vector<std::string>& args) {
    this->shell_->Stop();
    return 0;
}