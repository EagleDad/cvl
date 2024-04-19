// OWN includes
#include <Logger.h>

// STD includes
#include <chrono>
#include <ctime>
#include <iomanip>

namespace cvl::core
{
Logger::Logger( )
{
    mThreadActive = true;
    mLoggerThread = std::thread( [ this ] { threadFunc( ); } );
}

Logger::~Logger( )
{
    mThreadActive = false;

    if ( mLoggerThread.joinable( ) )
    {
        mLoggerThread.join( );
    }
}

HandleSharedPtr Logger::registerBackend( ILoggerBackEnd* backend )
{
    return mBackendHandler.registerObserver( backend );
}

void Logger::setSeverity( const Severity severity )
{
    if ( mSeverity != severity )
    {
        mSeverity = severity;
    }
}

void logIt( const ILogger::LogMessage& logMessage )
{
    /* auto a1 = std::async( std::launch::async,
                           []( ) { std::cout << "TEST" << std::endl; } );
     std::ignore = a1;
     a1.wait( );*/

    std::ignore = logMessage;
    /*handler.for_each(
        [ &logMessage ]( const ILoggerBackEnd* backEnd )
        { backEnd->log( logMessage ); } );*/
};

void Logger::log( const std::string& logMsg, const std::string& file,
                  const std::string& function, uint32_t line, Severity severity,
                  TimeStamp timeStamp /*= Clock::now( )*/,
                  std::thread::id threadId /*= std::this_thread::get_id( )*/ )
{
    LogMessage logMessage;

    logMessage.timestamp = timeStamp;
    logMessage.severity = severity;
    logMessage.message = logMsg;
    logMessage.threadId = threadId;
    logMessage.file = file;
    logMessage.function = function;
    logMessage.line = line;

    // BackendHandler& handler = mBackendHandler;

    mBackendHandler.for_each( [ &logMessage ]( const ILoggerBackEnd* backEnd )
                              { backEnd->log( logMessage ); } );

    //mLogQueue.push( logMessage );
}

std::stringstream Logger::getTimeStamp( const TimeStamp& timestamp ) const
{
    std::stringstream strTimeStamp;
    const auto in_time_t = Clock::to_time_t( timestamp );
    const auto ms = std::chrono::duration_cast< std::chrono::milliseconds >(
                        timestamp.time_since_epoch( ) )
                        .count( ) %
                    1000;

    tm currentLocalTime { };
    const auto timeBuffer = localtime_s( &currentLocalTime, &in_time_t );
    std::ignore = timeBuffer;

    strTimeStamp << std::put_time( &currentLocalTime, "%Y-%m-%d %H:%M:%S." )
                 << std::setw( 3 ) << std::setfill( '0' ) << ms;

    return strTimeStamp;
}

bool Logger::isLogQueueEmpty( )
{
    return mLogQueue.isEmpty( );
}

void Logger::log( const LogMessage& logMessage )
{
    mBackendHandler.for_each( [ &logMessage ]( const ILoggerBackEnd* backEnd )
                              { backEnd->log( logMessage ); } );
}

void Logger::threadFunc( )
{
    // Maybe run until queue is empty on close
    while ( mThreadActive )
    {
        LogMessage logMessage;
        if ( mLogQueue.tryPop( logMessage, std::chrono::milliseconds( 100 ) ) )
        {
            log( logMessage );
        }
    }

    while ( ! isLogQueueEmpty( ) )
    {
        LogMessage logMessage;
        if ( mLogQueue.tryPop( logMessage, std::chrono::milliseconds( 100 ) ) )
        {
            log( logMessage );
        }
    }
}
} // namespace cvl::core