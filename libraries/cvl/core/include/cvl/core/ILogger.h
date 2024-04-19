#pragma once

// CVL includes
#include <cvl/core/Handle.h>
#include <cvl/core/macros.h>

// STD includes
#include <chrono>
#include <sstream>
#include <thread>

using Clock = std::chrono::system_clock;
using TimeStamp = Clock::time_point;

namespace cvl::core
{

/**
 * @brief Interface definition for logging
 *
 *
 */
class ILogger
{
public:
    CVL_INTERFACE_DECL( ILogger );

    enum class Severity
    {
        Debug = 0,
        Informational = 1,
        Notice = 2,
        Warning = 3,
        Error = 4,
        Critical = 5,
        Alert = 6,
        Emergency = 7
    };

    struct LogMessage
    {
        TimeStamp timestamp;
        Severity severity;
        std::string message;
        std::thread::id threadId;
        std::string file;
        std::string function;
        uint32_t line;
    };

    class ILoggerBackEnd
    {
    public:
        CVL_INTERFACE( ILoggerBackEnd );

        virtual void log( const LogMessage& logMessage ) const = 0;
    };

    virtual HandleSharedPtr registerBackend( ILoggerBackEnd* backend ) = 0;

    virtual void
    log( const std::string& logMsg, const std::string& file,
         const std::string& function, uint32_t line, Severity severity,
         TimeStamp timeStamp = Clock::now( ),
         std::thread::id threadId = std::this_thread::get_id( ) ) = 0;

    [[nodiscard]] virtual Severity getSeverity( ) const = 0;

    virtual void setSeverity( Severity severity ) = 0;

    [[nodiscard]] virtual std::stringstream
    getTimeStamp( const TimeStamp& timestamp ) const = 0;

    virtual bool isLogQueueEmpty( ) = 0;
};

DECLARE_SMARTPTR( ILogger );

CVL_CORE_EXPORT ILogger* getLoggerInterface( );
CVL_CORE_EXPORT void setLoggerInterface( ILogger* logger );

CVL_CORE_EXPORT std::string
getSeverityName( const ILogger::Severity& severity );

// Declaration of the one 'instance' variable
extern ILogger* loggerInstance;

} // namespace cvl::core

#define LOG_MESSAGE( message, severity )                                       \
    do                                                                         \
    {                                                                          \
        auto ANONYMOUS_VARIABLE( logger ) = cvl::core::getLoggerInterface( );  \
        if ( ANONYMOUS_VARIABLE( logger )->getSeverity( ) <= severity )        \
        {                                                                      \
            std::stringstream ANONYMOUS_VARIABLE( stream );                    \
            ANONYMOUS_VARIABLE( stream ) << message;                           \
            ANONYMOUS_VARIABLE( logger )->log(                                 \
                ANONYMOUS_VARIABLE( stream ).str( ),                           \
                __FILE__,                                                      \
                __FUNCTION__,                                                  \
                __LINE__,                                                      \
                severity );                                                    \
        }                                                                      \
    } while ( 0 )

#define LOG_DEBUG( message )                                                   \
    LOG_MESSAGE( message, cvl::core::ILogger::Severity::Debug )

#define LOG_INFO( message )                                                    \
    LOG_MESSAGE( message, cvl::core::ILogger::Severity::Informational )

#define LOG_NOTICE( message )                                                  \
    LOG_MESSAGE( message, cvl::core::ILogger::Severity::Notice )

#define LOG_WARNING( message )                                                 \
    LOG_MESSAGE( message, cvl::core::ILogger::Severity::Warning )

#define LOG_ERROR( message )                                                   \
    LOG_MESSAGE( message, cvl::core::ILogger::Severity::Error )

#define LOG_CRITICAL( message )                                                \
    LOG_MESSAGE( message, cvl::core::ILogger::Severity::Critical )

#define LOG_ALERT( message )                                                   \
    LOG_MESSAGE( message, cvl::core::ILogger::Severity::Alert )

#define LOG_EMERGENCY( message )                                               \
    LOG_MESSAGE( message, cvl::core::ILogger::Severity::Emergency )