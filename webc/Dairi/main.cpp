#include <cstdio>
#include <stdexcept>
#include "Dairi.h"

int main(void) {
  ConsoleLogger clog;
  FileLogger flog;
  Agent agent{ &clog };

  agent.make_transfer(10201, 10202, 710.69);
  agent.make_transfer(10202, 10204, 420.808);
  agent.set_logger(&flog);
  agent.make_transfer(10202, 10204, 420.808);

  return 0;
}

