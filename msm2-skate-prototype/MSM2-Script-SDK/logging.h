#pragma once

#ifdef MSM2SCRIPTSDK_EXPORTS
#define MSM2SCRIPTSDK_API __declspec(dllexport)
#else
#define MSM2SCRIPTSDK_API __declspec(dllimport)
#endif

#if LOGLEVEL >= 4
    #define DEBUG(fmt, ...) logging::log(logging::LogLevel::DEBUG, fmt, __VA_ARGS__)
#else
    #define DEBUG(fmt, ...)
#endif

#if LOGLEVEL >= 3
    #define INFO(fmt, ...) logging::log(logging::LogLevel::INFO, fmt, __VA_ARGS__)
#else
    #define INFO(fmt, ...)
#endif

#if LOGLEVEL >= 2
    #define WARN(fmt, ...) logging::log(logging::LogLevel::WARN, fmt, __VA_ARGS__)
#else
    #define WARN(fmt, ...)
#endif

#if LOGLEVEL >= 1
    #define FATAL(fmt, ...) logging::log(logging::LogLevel::FATAL, fmt, __VA_ARGS__)
#else
    #define FATAL(fmt, ...)
#endif

namespace logging {
    enum LogLevel {
        DEBUG,
        INFO,
        WARN,
        FATAL
    };

    MSM2SCRIPTSDK_API void log(const LogLevel level, const char* fmt, ...);
}
