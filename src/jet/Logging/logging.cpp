#include <jet.h>
#include "logging.h"
#include <macros.h>

#include <chrono>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>

namespace jet
{
    static std::mutex critical;

    static std::ostream* InfoOutStream = &std::cout;
    static std::ostream* WarnOutStream = &std::cout;
    static std::ostream* ErrorOutStream = &std::cerr;
    static std::ostream* DebugOutStream = &std::cout;

    inline std::ostream* LevelToStream(LoggingLevel level)
    {
        switch(level)
        {
            case LoggingLevel::Info:
                return InfoOutStream;
            case LoggingLevel::Warn:
                return WarnOutStream;
            case LoggingLevel::Error:
                return ErrorOutStream;
            case LoggingLevel::Debug:
                return DebugOutStream;
        }

        return nullptr;
    }

    inline std::string LevelToString(LoggingLevel level)
    {
        switch(level)
        {
            case LoggingLevel::Info:
                return "INFO";
            case LoggingLevel::Warn:
                return "WARN";
            case LoggingLevel::Error:
                return "ERROR";
            case LoggingLevel::Debug:
                return "DEBUG";
        }

        return nullptr;
    }

    Logger::Logger(LoggingLevel level)
        : _Level(level)
    {}


    Logger::~Logger()
    {
        std::lock_guard<std::mutex> Lock(critical);
        #ifdef JET_DEBUG_MODE
            if (_Level != LoggingLevel::Debug) {
        #endif
            auto strm = LevelToStream(_Level);
            (*strm) << _buffer.str() << std::endl;
            strm->flush();
        #ifdef JET_DEBUG_MODE
            }
        #endif
    }

    void Logging::SetInfoStream(std::ostream* strm)
    {
        std::lock_guard<std::mutex> Lock(critical);
        InfoOutStream = strm;
    }

    void Logging::SetWarnStream(std::ostream* strm)
    {
        std::lock_guard<std::mutex> Lock(critical);
        WarnOutStream = strm;
    }

    void Logging::SetErrorStream(std::ostream* strm)
    {
        std::lock_guard<std::mutex> Lock(critical);
        ErrorOutStream = strm;
    }

    void Logging::SetDebugStream(std::ostream* strm)
    {
        std::lock_guard<std::mutex> Lock(critical);
        DebugOutStream = strm;
    }

    void Logging::SetAllStream(std::ostream* strm)
    {
        SetInfoStream(strm);
        SetWarnStream(strm);
        SetErrorStream(strm);
        SetDebugStream(strm);
    }

    std::string Logging::GetHeader(LoggingLevel level) {
        auto now = std::chrono::system_clock::to_time_t(
            std::chrono::system_clock::now());
        char timeStr[20];
    #ifdef JET_WINDOWS
        tm time;
        localtime_s(&time, &now);
        strftime(timeStr, sizeof(timeStr), "%F %T", &time);
    #else
        strftime(timeStr, sizeof(timeStr), "%F %T", std::localtime(&now));
    #endif
        char header[256];
        snprintf(
            header, sizeof(header), "[%s] %s ",
            LevelToString(level).c_str(),
            timeStr);
        return header;
    }
}