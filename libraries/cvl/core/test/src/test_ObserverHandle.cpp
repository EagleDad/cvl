#include <cvl/core/macros.h>

IGNORE_WARNINGS_GTEST_PUSH
#include <gtest/gtest.h>
// #include <gmock/gmock.h> // GTest Update required because of removed function
IGNORE_WARNINGS_POP

#include <cvl/core/ObserverHandle.h>

using namespace cvl::core;
using testing::Eq;

class Observer
{
public:
    Observer( ) = default;

    virtual ~Observer( ) = default;

    Observer( const Observer& other ) = delete;

    Observer( Observer&& other ) = delete;

    Observer& operator=( const Observer& other ) = delete;

    Observer& operator=( Observer&& other ) = delete;

    bool waitCount( size_t count )
    {
        std::unique_lock lock( mMutex );

        using namespace std::chrono_literals;
        return mCondVar.wait_for(
            lock, 10s, [ & ]( ) { return count == mCount; } );
    }

    [[nodiscard]] size_t getCount( ) const { return mCount; }

    void reset( )
    {
        std::unique_lock lock( mMutex );
        mCount = 0;
    }

    void notify( )
    {
        std::unique_lock lock( mMutex );
        ++mCount;
        mCondVar.notify_all( );
    }

private:
    size_t mCount { 0 };
    std::mutex mMutex;
    std::condition_variable mCondVar;
};

TEST( TestCvlCoreObserverHandle, RegisterDeregister )
{
    Observer observer;
    ObserverHandle< Observer > handler;

    EXPECT_EQ( handler.isRegistered( &observer ), false );

    const auto handle = handler.registerObserver( &observer );
    EXPECT_EQ( handler.isRegistered( &observer ), true );

    handle->clear( );
    EXPECT_EQ( handler.isRegistered( &observer ), false );
}

TEST( TestCvlCoreObserverHandle, RegisterDeregisterNotify )
{
    Observer observer;
    ObserverHandle< Observer > handler;

    EXPECT_EQ( handler.isRegistered( &observer ), false );

    const auto handle = handler.registerObserver( &observer );
    EXPECT_EQ( handler.isRegistered( &observer ), true );

    handler.for_each( []( Observer* backend ) { backend->notify( ); } );

    EXPECT_EQ( observer.getCount( ), 1 );

    handle->clear( );
    EXPECT_EQ( handler.isRegistered( &observer ), false );
}

TEST( TestCvlCoreObserverHandle, RegisterDeregisterNotifyEmpty )
{
    const Observer observer;
    ObserverHandle< Observer > handler;

    EXPECT_EQ( handler.isRegistered( &observer ), false );

    EXPECT_NO_THROW(
        handler.for_each( []( Observer* backend ) { backend->notify( ); } ); );
}

TEST( TestCvlCoreObserverHandle, RegisterDeregisterNotifyThrowObserverNull )
{
    Observer* observer = nullptr;
    ObserverHandle< Observer > handler;

    const auto handle = handler.registerObserver( observer );

    EXPECT_THROW(
        handler.for_each( []( Observer* backend ) { backend->notify( ); } );
        , Error );
}

TEST( TestCvlCoreObserverHandle, RegisterDeregisterNotifyThrowNtifyNull )
{
    Observer observer;
    ObserverHandle< Observer > handler;

    const auto handle = handler.registerObserver( &observer );

    EXPECT_THROW( handler.for_each( nullptr );, Error );
}
