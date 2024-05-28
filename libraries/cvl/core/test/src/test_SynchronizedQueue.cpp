// CVL includes
#include <cvl/core/macros.h>

// STD includes
IGNORE_WARNINGS_STD_PUSH
#include <future>
#include <thread>
#include <list>
IGNORE_WARNINGS_POP

IGNORE_WARNINGS_GTEST_PUSH
#include <gtest/gtest.h>
// #include <gmock/gmock.h> // GTest Update required because of removed function
IGNORE_WARNINGS_POP

#include <cvl/core/SynchronizedQueue.h>

using namespace cvl::core;
using namespace std::chrono_literals;
using testing::Eq;

template < typename T >
class TestCvlCoreSynchronizedQueue : public testing::Test
{
public:
    using List = std::list< T >;
    static T shared_;
    T value_ { };

    TestCvlCoreSynchronizedQueue( ) = default;

    ~TestCvlCoreSynchronizedQueue( ) override = default;

    TestCvlCoreSynchronizedQueue( const TestCvlCoreSynchronizedQueue& other ) =
        delete;

    TestCvlCoreSynchronizedQueue( const TestCvlCoreSynchronizedQueue&& other ) =
        delete;

    TestCvlCoreSynchronizedQueue&
    operator=( const TestCvlCoreSynchronizedQueue& other ) = delete;

    TestCvlCoreSynchronizedQueue&
    operator=( TestCvlCoreSynchronizedQueue&& other ) = delete;
};

using Types =
    testing::Types< int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t,
                    int64_t, uint64_t, float, double, size_t >;

TYPED_TEST_SUITE(
    TestCvlCoreSynchronizedQueue,
    Types ); // NOLINT(clang-diagnostic-gnu-zero-variadic-macro-arguments)

TYPED_TEST( TestCvlCoreSynchronizedQueue, SetGetValueSync )
{
    constexpr auto value = TypeParam { 10 };

    SynchronizedQueue< TypeParam > queue;

    EXPECT_EQ( queue.isEmpty( ), true );

    queue.push( value );

    EXPECT_EQ( queue.isEmpty( ), false );

    TypeParam readValue;
    EXPECT_EQ( queue.tryPop( readValue, 1000ms ), true );

    EXPECT_EQ( readValue, value );
}

TYPED_TEST( TestCvlCoreSynchronizedQueue, PushPopAsync )
{
    using namespace std::chrono_literals;
    SynchronizedQueue< TypeParam > queue;

    auto future = std::async(
        [ & ]
        {
            std::this_thread::sleep_for( 250ms );
            queue.push( TypeParam { 1 } );
            queue.push( TypeParam { 2 } );
        } );

    TypeParam readValue;
    EXPECT_EQ( queue.tryPop( readValue, 1000ms ), true );

    EXPECT_EQ( readValue, TypeParam { 1 } );

    EXPECT_EQ( queue.tryPop( readValue, 1000ms ), true );

    EXPECT_EQ( readValue, TypeParam { 2 } );

    EXPECT_EQ( queue.isEmpty( ), true );
}

TYPED_TEST( TestCvlCoreSynchronizedQueue, PushPopNoItem )
{
    using namespace std::chrono_literals;
    SynchronizedQueue< TypeParam > queue;

    TypeParam readValue;
    EXPECT_EQ( queue.tryPop( readValue, 100ms ), false );
}
