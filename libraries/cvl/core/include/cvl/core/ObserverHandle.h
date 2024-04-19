#pragma once

// CVL includes
#include <cvl/core/Handle.h>
#include <cvl/core/macros.h>

// STD includes
#include <iostream>
#include <list>
#include <mutex>

namespace cvl::core
{
/**
 * @brief Template implementation for a handle class
 *
 * https://stackoverflow.com/questions/39516416/using-weak-ptr-to-implement-the-observer-pattern
 */
template < typename Type >
class ObserverHandle
{
public:
    CVL_DEFAULT_ONLY( ObserverHandle );

    HandleSharedPtr registerObserver( Type* observer )
    {
        // EXPECT_MSG( observer, "Invalid observer" );
        // EXPECT_MSG( !isRegistered( observer), "Observer already egisterd");

        std::lock_guard lock( mMutex );

        auto itr = mObservers.insert( mObservers.end( ), observer );

        return std::make_shared< Handle >(
            [ this, itr ]
            {
                std::lock_guard lock( mMutex );
                mObservers.erase( itr );
            } );
    }

    bool isRegistered( const Type* observer ) const noexcept
    {
        std::lock_guard< std::recursive_mutex > lock( mMutex );

        return std::find( mObservers.begin( ), mObservers.end( ), observer ) !=
               mObservers.end( );
    }

    void for_each( std::function< void( Type* ) > execute )
    {
        if ( mObservers.empty( ) )
        {
            return;
        }

        std::list< Type* > observers = mObservers;

        for ( auto observer : observers )
        {
            EXPECT_MSG( observer != nullptr, "Observer is nullptr!" );

            EXPECT_MSG( execute != nullptr, "Notify function is nullptr!" );

            execute( observer );
        }
    }

private:
    std::list< Type* > mObservers { };
    mutable std::recursive_mutex mMutex { };
};
} // namespace cvl::core