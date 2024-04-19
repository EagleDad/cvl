// CVL includes
#include <cvl/core/ConsoleLoggingBackend.h>
#include <cvl/core/ILogger.h>
#include <cvl/core/macros.h>

// Std include
IGNORE_WARNINGS_STD_PUSH
#include <future>
#include <regex>
IGNORE_WARNINGS_POP

IGNORE_WARNINGS_GTEST_PUSH
#include <gtest/gtest.h>
// #include <gmock/gmock.h> // GTest Update required because of removed function
IGNORE_WARNINGS_POP

using namespace cvl::core;
using testing::Eq;
using namespace std::chrono_literals;

static int32_t Counter = 0;
static ILogger::LogMessage Message;

class TestLoggingBackend : public ILogger::ILoggerBackEnd
{
public:
    TestLoggingBackend( )
    {
        mLogger = getLoggerInterface( );
        mLoggingHandle = mLogger->registerBackend( this );
    }
    ~TestLoggingBackend( ) override = default;

    TestLoggingBackend( const TestLoggingBackend& rhs ) = delete;
    TestLoggingBackend( const TestLoggingBackend&& rhs ) = delete;

    TestLoggingBackend& operator=( const TestLoggingBackend& rhs ) = delete;
    TestLoggingBackend& operator=( const TestLoggingBackend&& rhs ) = delete;

    // ILogging::ILoggingBackEnd overrides
    void log( const ILogger::LogMessage& logMessage ) const override
    {
        Message = logMessage;
        Counter++;
        /*const auto timestamp = mLogger->getTimeStamp( logMessage.timestamp );

        std::cout << timestamp.str( ) << " " << logMessage.message <<
        std::endl;*/
    }

private:
    ILogger* mLogger;
    HandleSharedPtr mLoggingHandle;
};

TEST( TestCvlCoreLogger, RegisterDeregister )
{
    EXPECT_NO_THROW( getLoggerInterface( ); );
    EXPECT_NO_THROW( setLoggerInterface( getLoggerInterface( ) ); );
}

TEST( TestCvlCoreLogger, GetSetSeverty )
{
    const auto logger = getLoggerInterface( );

    logger->setSeverity( ILogger::Severity::Alert );

    EXPECT_EQ( logger->getSeverity( ), ILogger::Severity::Alert );
}

TEST( TestCvlCoreLogger, TestBackend )
{
    const auto logger = getLoggerInterface( );
    TestLoggingBackend testBackend;

    logger->setSeverity( ILogger::Severity::Debug );

    LOG_INFO( "Test" );

    while ( ! logger->isLogQueueEmpty( ) )
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    }

    std::this_thread::sleep_for( std::chrono::milliseconds( 250 ) );

    EXPECT_EQ( Counter, 1 );
    EXPECT_EQ( Message.message, "Test" );

    EXPECT_NE( logger->getTimeStamp( TimeStamp { } ).str( ).length( ), 0 );
}

std::string getConsoleLoggerMessage( const ILogger::Severity severity,
                                     const std::string& logMessage )
{
    const auto logger = getLoggerInterface( );
    ConsoleLoggingBackend testBackend;

    logger->setSeverity( ILogger::Severity::Debug );

    std::stringbuf sbuf( std::ios::out ); // create a stringbuf
    auto oldbuf = std::cout.rdbuf(
        std::addressof( sbuf ) ); // associate the stringbuf with std::cout

    switch ( severity )
    {
    case ILogger::Severity::Debug:
    {
        LOG_DEBUG( logMessage );
        break;
    }

    case ILogger::Severity::Informational:
    {
        LOG_INFO( logMessage );
        break;
    }

    case ILogger::Severity::Notice:
    {
        LOG_NOTICE( logMessage );
        break;
    }

    case ILogger::Severity::Warning:
    {
        LOG_WARNING( logMessage );
        break;
    }

    case ILogger::Severity::Error:
    {
        LOG_ERROR( logMessage );
        break;
    }

    case ILogger::Severity::Critical:
    {
        LOG_CRITICAL( logMessage );
        break;
    }

    case ILogger::Severity::Alert:
    {
        LOG_ALERT( logMessage );
        break;
    }

    case ILogger::Severity::Emergency:
    {
        LOG_EMERGENCY( logMessage );
        break;
    }
    }

    auto future = std::async(
        [ & ]
        {
            std::this_thread::sleep_for( 250ms );

            std::cout.rdbuf( oldbuf ); // restore cout's original buffer

            return std::string { sbuf.str( ) };
        } );

    while ( ! logger->isLogQueueEmpty( ) )
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    }

    future.wait( );

    const auto retString = future.get( );

    std::regex regex { R"([\s ]+)" }; // split on space and comma
    std::sregex_token_iterator it {
        retString.begin( ), retString.end( ), regex, -1 };
    std::vector< std::string > words { it, {} };

    return words[ 4 ];
}

TEST( TestCvlCoreLogger, TestConsoleBackend )
{
    EXPECT_EQ( getConsoleLoggerMessage( ILogger::Severity::Debug, "Debug" ),
               "Debug" );

    EXPECT_EQ( getConsoleLoggerMessage( ILogger::Severity::Informational,
                                        "Informational" ),
               "Informational" );

    EXPECT_EQ( getConsoleLoggerMessage( ILogger::Severity::Notice, "Notice" ),
               "Notice" );

    EXPECT_EQ( getConsoleLoggerMessage( ILogger::Severity::Warning, "Warning" ),
               "Warning" );

    EXPECT_EQ( getConsoleLoggerMessage( ILogger::Severity::Error, "Error" ),
               "Error" );

    EXPECT_EQ(
        getConsoleLoggerMessage( ILogger::Severity::Critical, "Critical" ),
        "Critical" );

    EXPECT_EQ( getConsoleLoggerMessage( ILogger::Severity::Alert, "Alert" ),
               "Alert" );

    EXPECT_EQ(
        getConsoleLoggerMessage( ILogger::Severity::Emergency, "Emergency" ),
        "Emergency" );
}
