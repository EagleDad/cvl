// Std includes
#include <sstream>

// Own includes
#include <cvl/core/Error.h>

namespace cvl::core
{
Error::Error( const char* file, const uint32_t line )
    : mFile( file )
    , mLine( line )
{
}

Error::Error( const char* file, const uint32_t line, std::string message )
    : mFile( file )
    , mLine( line )
    , mMessage( std::move( message ) )
{
}

auto Error::getLine( ) const -> uint32_t
{
    return mLine;
}

auto Error::getFileName( ) const -> std::string
{
    return mFile;
}

auto Error::getMessage( ) const -> std::string
{
    return mMessage;
}

auto operator<<( std::ostream& os, const Error& error ) -> std::ostream&
{
    // Print the current error
    os << error.getFileName( ) << ":" << error.getLine( ) << ":";

    os << error.getMessage( );

    os << "\n";

    return os;
}

auto Error::what( ) const -> char const*
{
    static thread_local std::string message;

    std::stringstream ss;
    ss << *this;
    message = ss.str( );

    return message.c_str( );
}

} // namespace cvl::core
