#pragma once

// CVL includes
#include <cvl/core/ILogger.h>
#include <cvl/core/ObserverHandle.h>
#include <cvl/core/SynchronizedQueue.h>

// STD includes

namespace cvl::core
{
/**
 * @brief Implementation of the logger
 *
 *
 */
class Logger final : public ILogger
{
public:
    Logger( );
    ~Logger( ) override;
    Logger( const Logger& other ) = delete;
    Logger( Logger&& other ) = delete;
    Logger& operator=( const Logger& other ) = delete;
    Logger& operator=( Logger&& other ) = delete;

    HandleSharedPtr registerBackend( ILoggerBackEnd* backend ) override;

     void log( const std::string& logMsg, const std::string& file,
              const std::string& function, uint32_t line, Severity severity,
              TimeStamp timeStamp = Clock::now( ),
              std::thread::id threadId = std::this_thread::get_id( ) ) override;

    Severity getSeverity( ) const override { return mSeverity; }

    void setSeverity( Severity severity ) override;

    std::stringstream getTimeStamp( const TimeStamp& timestamp ) const override;

    bool isLogQueueEmpty( ) override;

private:
    void log( const LogMessage& logMessage );

    void threadFunc( );

private:
    using BackendHandler = ObserverHandle< ILoggerBackEnd >;
    BackendHandler mBackendHandler;

    std::atomic< Severity > mSeverity { Severity::Notice };
    SynchronizedQueue< LogMessage > mLogQueue;

    std::thread mLoggerThread;
    std::atomic_bool mThreadActive { false };
};

} // namespace cvl::core