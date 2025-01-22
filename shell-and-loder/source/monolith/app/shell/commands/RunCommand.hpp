#pragma once
#include "Command.hpp"

class RunCommand : public Command {
public:
  explicit RunCommand(Shell* shell) : Command(shell) {
  }
  int Execute(const std::vector<std::string>& args) override;

private:
  static int ChildProcess(void* args);
};