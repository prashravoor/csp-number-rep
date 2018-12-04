#ifndef LOGGER_H
#define LOGGER_H

#include <sstream>
#include <string>

#define ILOG (Logger(LOG_INFO))
#define ELOG (Logger(LOG_ERROR))
#define DLOG (Logger(LOG_DEBUG))

enum LogLevel
{
  LOG_ERROR = 0,
  LOG_INFO,
  LOG_DEBUG
};

class Logger
{
public:
  Logger(LogLevel logLevel = LOG_INFO);

  template <typename T>
  Logger &operator<<(const T &value)
  {
    buffer << value;
    return *this;
  }

  ~Logger();

  static void init();

private:
  std::ostringstream buffer;
  LogLevel level;
  static LogLevel globalLogLevel;
  static const std::string LOG_LEVEL_ENV;
};

#endif