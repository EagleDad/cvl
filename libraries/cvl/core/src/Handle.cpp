// Own includes
#include <cvl/core/Handle.h>

namespace cvl::core
{

Handle::Handle( std::function< void( ) > onDestroy )
    : mOnDestroy( std::move( onDestroy ) )
{
}

Handle::Handle( Handle&& other ) noexcept
    : mOnDestroy( std::move( other.mOnDestroy ) )
{
    other.mOnDestroy = std::function< void( ) >( );
}

Handle::~Handle( )
{
    clear( );
}

Handle& Handle::operator=( Handle&& rhs ) noexcept
{
    mOnDestroy = std::move( rhs.mOnDestroy );
    rhs.mOnDestroy = std::function< void( ) >( );
    return *this;
}

void Handle::clear( )
{
    if ( mOnDestroy )
    {
        mOnDestroy( );
        mOnDestroy = std::function< void( ) >( );
    }
}

} // namespace cvl::core