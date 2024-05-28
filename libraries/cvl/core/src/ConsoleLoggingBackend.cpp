// OWN includes
#include <cvl/core/ConsoleLoggingBackend.h>

// STD includes
#include <iostream>

// Windows includes
#if defined( _MSC_VER )
    #pragma warning( push )
    #pragma warning( disable : 5039 )
    #include <Windows.h>
    #pragma warning( pop )

    constexpr int16_t CONSOLE_COLOR_BLACK = 0;
    constexpr int16_t CONSOLE_COLOR_DARK_BLUE = 1;
    constexpr int16_t CONSOLE_COLOR_DARK_GREEN = 2;
    constexpr int16_t CONSOLE_COLOR_LIGHT_BLUE = 3;
    constexpr int16_t CONSOLE_COLOR_DARK_RED = 4;
    constexpr int16_t CONSOLE_COLOR_MAGENTA = 5;
    constexpr int16_t CONSOLE_COLOR_ORANGE = 6;
    constexpr int16_t CONSOLE_COLOR_LIGHT_GRAY = 7;
    constexpr int16_t CONSOLE_COLOR_GRAY = 8;
    constexpr int16_t CONSOLE_COLOR_BLUE = 9;
    constexpr int16_t CONSOLE_COLOR_GREEN = 10;
    constexpr int16_t CONSOLE_COLOR_CYAN = 11;
    constexpr int16_t CONSOLE_COLOR_RED = 12;
    constexpr int16_t CONSOLE_COLOR_PINK = 13;
    constexpr int16_t CONSOLE_COLOR_YELLOW = 14;
    constexpr int16_t CONSOLE_COLOR_WHITE = 15;
#endif

namespace cvl::core
{
#if defined( _MSC_VER )
void setConsoleOutputColor( const HANDLE& h, const ILogger::Severity& severity )
{
    switch ( severity )
    {
    case ILogger::Severity::Debug:
    {
        SetConsoleTextAttribute( h, CONSOLE_COLOR_LIGHT_GRAY );
        break;
    }

    case ILogger::Severity::Informational:
    {
        SetConsoleTextAttribute(
            h, CONSOLE_COLOR_LIGHT_BLUE + CONSOLE_COLOR_LIGHT_GRAY * 16 );
        break;
    }

    case ILogger::Severity::Notice:
    {
        SetConsoleTextAttribute(
            h, CONSOLE_COLOR_WHITE + CONSOLE_COLOR_LIGHT_BLUE * 16 );
        break;
    }

    case ILogger::Severity::Warning:
    {
        SetConsoleTextAttribute(
            h, CONSOLE_COLOR_DARK_BLUE + CONSOLE_COLOR_YELLOW * 16 );
        break;
    }

    case ILogger::Severity::Error:
    {
        SetConsoleTextAttribute(
            h, CONSOLE_COLOR_YELLOW + CONSOLE_COLOR_RED * 16 );
        break;
    }

    case ILogger::Severity::Critical:
    {
        SetConsoleTextAttribute(
            h, CONSOLE_COLOR_DARK_BLUE + CONSOLE_COLOR_LIGHT_GRAY * 16 );
        break;
    }

    case ILogger::Severity::Alert:
    {
        SetConsoleTextAttribute(
            h, CONSOLE_COLOR_DARK_BLUE + CONSOLE_COLOR_DARK_GREEN * 16 );
        break;
    }

    case ILogger::Severity::Emergency:
    {
        SetConsoleTextAttribute(
            h, CONSOLE_COLOR_DARK_BLUE + CONSOLE_COLOR_DARK_RED * 16 );
        break;
    }
    }
}
#endif
class ConsoleLoggingBackend::Impl
{
public:
    Impl( )
    {
        mLogger = getLoggerInterface( );
#if defined( _MSC_VER )
        mOutputConsoleHandle = GetStdHandle( STD_OUTPUT_HANDLE );
        GetConsoleScreenBufferInfo( mOutputConsoleHandle,
                                    &mDefaultConsoleAttributes );

        // Change RGB colors to saturated red green and blue
        CONSOLE_SCREEN_BUFFER_INFOEX sbInfoEx;
        sbInfoEx.cbSize = sizeof( CONSOLE_SCREEN_BUFFER_INFOEX );
        GetConsoleScreenBufferInfoEx( mOutputConsoleHandle, &sbInfoEx );

        sbInfoEx.ColorTable[ CONSOLE_COLOR_DARK_RED ] = RGB( 0xFF, 0x00, 0x00 );
        sbInfoEx.ColorTable[ CONSOLE_COLOR_DARK_GREEN ] =
            RGB( 0x00, 0xFF, 0x00 );
        sbInfoEx.ColorTable[ CONSOLE_COLOR_DARK_BLUE ] =
            RGB( 0x00, 0x00, 0xFF );
        sbInfoEx.ColorTable[ CONSOLE_COLOR_YELLOW ] = RGB( 0xFF, 0xFF, 0x00 );

        SetConsoleScreenBufferInfoEx( mOutputConsoleHandle, &sbInfoEx );
#endif        
    }

    ~Impl( ) = default;

    CVT_DISABLE_COPY( Impl );
    CVT_DISABLE_MOVE( Impl );

    void log( const ILogger::LogMessage& logMessage ) const
    {
#if defined( _MSC_VER )        
        setConsoleOutputColor( mOutputConsoleHandle, logMessage.severity );
#endif  

        const auto timestamp = mLogger->getTimeStamp( logMessage.timestamp );      

        std::cout << "[" << timestamp.str( ) << "] [" << logMessage.threadId
                  << "] [" << getSeverityName( logMessage.severity ) << "]\t"
                  << logMessage.message << std::endl;

#if defined( _MSC_VER )
        FlushConsoleInputBuffer( mOutputConsoleHandle );

        // set console back to default
        SetConsoleTextAttribute( mOutputConsoleHandle,
                                 mDefaultConsoleAttributes.wAttributes );
#endif
    }

public:
    HandleSharedPtr mLoggingHandle { };
    ILogger* mLogger { };

private:
#if defined( _MSC_VER )
    HANDLE mOutputConsoleHandle { };
    CONSOLE_SCREEN_BUFFER_INFO mDefaultConsoleAttributes { };
#endif
};

ConsoleLoggingBackend::ConsoleLoggingBackend( )
    : mImpl( std::make_unique< Impl >( ) )
{
    mImpl->mLoggingHandle = mImpl->mLogger->registerBackend( this );
}

ConsoleLoggingBackend::~ConsoleLoggingBackend( ) = default;

void ConsoleLoggingBackend::log( const ILogger::LogMessage& logMessage ) const
{
    mImpl->log( logMessage );
}

} // namespace cvl::core