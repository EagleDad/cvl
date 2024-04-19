#pragma once

// CVL includes
#include <cvl/core/macros.h>

// STD includes
#include <condition_variable>
#include <mutex>
#include <queue>

namespace cvl::core
{
/**
 * @brief Template implementation for a synchronized queue
 *
 * https://www.geeksforgeeks.org/implement-thread-safe-queue-in-c/
 *
 */
template < typename Type >
class SynchronizedQueue
{
public:
    CVL_DEFAULT_ONLY( SynchronizedQueue );

    /**
     * Function that pushes an element to the queue
     *
     * @param [in]  element     The element to be pushed
     *
     */
    void push( const Type& element );

    /**
     * Function that tries to get an element from the queue
     *
     * @param [in]  element     The element to be received
     * @param [in]  timeout     The time to wait for success
     *
     * @return true if success, else false
     */
    bool tryPop( Type& element, std::chrono::milliseconds timeout );

    /**
     * Function that checks if a queue is empty
     *
     * @return true if empty, else false
     */
    bool isEmpty( ) const;

private:
    std::queue< Type > mQueue;
    mutable std::mutex mLockMutex;
    std::condition_variable mCondVar;
};

template < typename Type >
void SynchronizedQueue< Type >::push( const Type& element )
{
    {
        std::lock_guard lock( mLockMutex );

        mQueue.push( std::move( element ) );
    }

    mCondVar.notify_one( );
}

template < typename Type >
bool SynchronizedQueue< Type >::tryPop(
    Type& element, const std::chrono::milliseconds timeout )
{
    std::unique_lock lock( mLockMutex );

    if ( ! mCondVar.wait_for(
             lock, timeout, [ this ] { return ! mQueue.empty( ); } ) )
    {
        return false;
    }

    element = std::move( mQueue.front( ) );
    mQueue.pop( );

    return true;
}

template < typename Type >
bool SynchronizedQueue< Type >::isEmpty( ) const
{
    std::unique_lock lock( mLockMutex );
    return mQueue.empty( );
}

} // namespace cvl::core