#include "shell/Shell.hpp"


namespace monolith::app {

namespace {

void Main() {
  Shell shell = Shell();
  shell.Start();
  
  }

}  // namespace

}  // namespace monolith::app

int main() {
  monolith::app::Main();
}
