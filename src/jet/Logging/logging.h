#pragma once

#include<iostream>
#include<sstream>
#include<string>

namespace jet
{
    //! Logging level.
    enum class LoggingLevel
    {
        Info,
        Warn,
        Error,
        Debug
    };


    //! \brief Simple Logger implementation.
    
    class Logger final
    {
    public:
        //! Constructs a logger with logging level.
        explicit Logger(LoggingLevel level);

        //! Destructor
        ~Logger();

        //! Writes a value to the buffer stream.
        template<typename T>
        const Logger& operator<<(const T& x) const
        {
            _buffer << x;
            return *this;
        }

    private:
        LoggingLevel _Level;
        mutable std::stringstream _buffer;
    };

    //! Helper class for logging.
    class Logging
    {
    public:
        //! Sets the output stream for the info level logs.
        static void SetInfoStream(std::ostream* strm);

        //! Sets the output stream for the warning level logs.
        static void SetWarnStream(std::ostream* strm);

        //! Sets the output stream for the error level logs.
        static void SetErrorStream(std::ostream* strm);

        //! Sets the output stream for the debug level logs.
        static void SetDebugStream(std::ostream* strm);

        //! Sets the output stream for all the log levels.
        static void SetAllStream(std::ostream* strm);

        //! Returns the header string.
        static std::string GetHeader(LoggingLevel level);
    };

    //! Info-level logger.
    extern Logger InfoLogger;

    //! Warn-level logger.
    extern Logger WarnLogger;

    //! Error-level logger.
    extern Logger ErrorLogger;

    //! Debug-level logger.
    extern Logger DebugLogger;

    #define JET_INFO \
        (Logger(LoggingLevel::Info) << Logging::GetHeader(LoggingLevel::Info) \
        << "[" << __FILE__ << ":" << __LINE__ << " (" << __func__ << ")] ")
    #define JET_WARN \
        (Logger(LoggingLevel::Warn) << Logging::GetHeader(LoggingLevel::Warn) \
        << "[" << __FILE__ << ":" << __LINE__ << " (" << __func__ << ")] ")
    #define JET_ERROR \
        (Logger(LoggingLevel::Error) << Logging::GetHeader(LoggingLevel::Error) \
        << "[" << __FILE__ << ":" << __LINE__ << " (" << __func__ << ")] ")
    #define JET_DEBUG \
        (Logger(LoggingLevel::Debug) << Logging::GetHeader(LoggingLevel::Debug) \
        << "[" << __FILE__ << ":" << __LINE__ << " (" << __func__ << ")] ")
}