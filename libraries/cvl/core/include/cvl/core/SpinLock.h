#pragma once

// CVL includes
#include <cvl/core/macros.h>

// STD includes
#include <atomic>

// https://stackoverflow.com/questions/26583433/c11-implementation-of-spinlock-using-header-atomic
// https://en.cppreference.com/w/cpp/atomic/atomic_flag

namespace cvl::core
{
class SpinLock
{
public:
    CVT_DISABLE_COPY( SpinLock );
    CVT_DISABLE_MOVE( SpinLock );

    SpinLock( ) = default;

    ~SpinLock( ) = default;

    void lock( )
    {
        while ( mLocked.test_and_set( std::memory_order_acquire ) )
        {
#if defined( __cpp_lib_atomic_flag_test )
            while ( mLocked.test( std::memory_order_relaxed ) )
            { // test lock
            }
#endif
            // Spin
        }
    }
    void unlock( ) { mLocked.clear( std::memory_order_release ); }

private:
    std::atomic_flag mLocked = ATOMIC_FLAG_INIT;
};

class Lock
{
public:
    Lock( ) = delete;

    CVT_DISABLE_COPY( Lock );
    CVT_DISABLE_MOVE( Lock );

    Lock( SpinLock& spinLock )
        : mSpinLock( spinLock )
    {
        mSpinLock.lock( );
    }

    ~Lock( ) { mSpinLock.unlock( ); }

private:
    SpinLock& mSpinLock;
};

template < class Callable, class... Args >
void call_once( bool& flag, Callable&& f, Args&&... args )
{
    if ( ! flag )
    {
        f( std::forward< decltype( args ) >( args )... );
        flag = true;
    }
}

} // namespace cvl::core