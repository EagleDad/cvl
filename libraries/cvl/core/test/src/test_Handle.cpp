#include <cvl/core/macros.h>

IGNORE_WARNINGS_GTEST_PUSH
#include <gtest/gtest.h>
// #include <gmock/gmock.h> // GTest Update required because of removed function
IGNORE_WARNINGS_POP

#include <cvl/core/Handle.h>

using namespace cvl::core;

TEST( TestCvlCoreHandle, CleanupInvocation )
{
    bool cleanupWasCalled = false;
    {
        Handle handle( [ &cleanupWasCalled ]( ) { cleanupWasCalled = true; } );
    }
    ASSERT_EQ( true, cleanupWasCalled );
}

TEST( TestCvlCoreHandle, MoveConstruct )
{
    bool cleanupWasCalled = false;
    {
        Handle handle( [ &cleanupWasCalled ]( ) { cleanupWasCalled = true; } );

        Handle moved( std::move( handle ) );
    }
    ASSERT_EQ( true, cleanupWasCalled );
}

TEST( TestCvlCoreHandle, MoveAssign )
{
    bool cleanupWasCalled = false;
    {
        Handle moved;
        moved = Handle( [ &cleanupWasCalled ]( ) { cleanupWasCalled = true; } );
    }
    ASSERT_EQ( true, cleanupWasCalled );
}