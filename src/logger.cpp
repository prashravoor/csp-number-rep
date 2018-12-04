#include "logger.h"
#include <iostream>
#include <cstdlib>

LogLevel Logger::globalLogLevel = LOG_ERROR;
const std::string Logger::LOG_LEVEL_ENV("LOG_LEVEL");

Logger::Logger(LogLevel logLevel) : level(logLevel)
{
    switch (level)
    {
    case LOG_INFO:
        buffer << "INFO ";
        break;
    case LOG_DEBUG:
        buffer << "DEBUG ";
        break;
    case LOG_ERROR:
        buffer << "ERROR ";
        break;
    }
}

Logger::~Logger()
{
    if (level > globalLogLevel)
    {
        return;
    }

    switch (level)
    {
    case LOG_INFO:
    case LOG_DEBUG:
        std::cout << buffer.str() << std::endl;
        break;
    case LOG_ERROR:
        std::cerr << buffer.str() << std::endl;
    }
}

void Logger::init()
{
    globalLogLevel = LOG_INFO;
    char *logLevel = getenv(LOG_LEVEL_ENV.c_str());
    if (logLevel && std::string("ERROR").find(logLevel) == 0)
    {
        globalLogLevel = LOG_ERROR;
    }
    else if (logLevel && std::string("DEBUG").find(logLevel) == 0)
    {
        globalLogLevel = LOG_DEBUG;
    }
}
