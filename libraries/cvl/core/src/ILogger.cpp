#include <Logger.h>
#include <cvl/core/ILogger.h>

namespace cvl::core
{
// Definition of the one 'instance' variable
ILogger* loggerInstance = nullptr;

ILogger* getLoggerInterface( )
{
    // Local object, initialized by an 'Immediately Invoked Lambda Expression
    // (IILE)'
    static bool init = []( )
    {
        if ( ! loggerInstance )
        {
            static Logger logger;
            loggerInstance = &logger;
        }
        return true; // or false, as the actual value does not matter.
    }( ); // Note the '()' after the lambda expression. This invokes the lambda.

    return loggerInstance;
}

void setLoggerInterface( ILogger* logger )
{
    loggerInstance = logger;
}

std::string getSeverityName( const ILogger::Severity& severity )
{
    switch ( severity )
    {
    case ILogger::Severity::Debug:
    {
        return "Debug";
    }

    case ILogger::Severity::Informational:
    {
        return "Informational";
    }

    case ILogger::Severity::Notice:
    {
        return "Notice";
    }

    case ILogger::Severity::Warning:
    {
        return "Warning";
    }

    case ILogger::Severity::Error:
    {
        return "Error";
    }

    case ILogger::Severity::Critical:
    {
        return "Critical";
    }

    case ILogger::Severity::Alert:
    {
        return "Alert";
    }

    case ILogger::Severity::Emergency:
    {
        return "Emergency";
    }
    }
    return "Invalid";
}

} // namespace cvl::core