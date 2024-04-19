#pragma once

// CVL includes
#include <cvl/core/ILogger.h>

// Std includes
#include <memory>

namespace cvl::core
{
class CVL_CORE_EXPORT ConsoleLoggingBackend : public ILogger::ILoggerBackEnd
{
public:
    ConsoleLoggingBackend( );
    ~ConsoleLoggingBackend( ) override;

    ConsoleLoggingBackend( const ConsoleLoggingBackend& rhs ) = delete;
    ConsoleLoggingBackend( const ConsoleLoggingBackend&& rhs ) = delete;

    ConsoleLoggingBackend&
    operator=( const ConsoleLoggingBackend& rhs ) = delete;
    ConsoleLoggingBackend&
    operator=( const ConsoleLoggingBackend&& rhs ) = delete;

    void log( const ILogger::LogMessage& logMessage ) const override;

private:
    class Impl;
    std::unique_ptr< Impl > mImpl;
};
} // namespace cvl::core
