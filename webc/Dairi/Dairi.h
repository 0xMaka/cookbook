#ifndef DAIRI_H
#define DAIRI_H

#include <cstdio>
#include <stdexcept>

struct Logger {
  virtual ~Logger() = default;
  virtual void log_transfer(long from, long to, double amount) = 0;
};

struct FileLogger : Logger {
  void log_transfer(long from, long to, double amount) override {
    // snip
    printf("[file][+] %ld, %ld, %0.8f\n");
  }
};

struct ConsoleLogger : Logger {
  void log_transfer(long from, long to, double amount) override {
    printf("[cons][>] %ld -> %ld: %0.8f\n", from, to, amount);
  }
};

enum class LoggerType {
  Console,
  File
};

struct Agent {
  Agent(Logger *_logger) : logger { _logger } {}

  void set_logger(Logger *_logger) {
    logger = _logger;
  }
  void make_transfer(long from, long to, double amount) {
    if (logger) logger->log_transfer(from, to, amount);
  }
  private:
    Logger *logger{};
};
#endif /* DAIRI_H */
