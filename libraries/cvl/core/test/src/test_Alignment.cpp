// CVL includes
#include <cvl/core/Alignment.h>
#include <cvl/core/macros.h>

// STD includes
#include <random>
#include <list>

// GTest includes
IGNORE_WARNINGS_GTEST_PUSH
#include <gtest/gtest.h>
// #include <gmock/gmock.h> // GTest Update required because of removed function
//  in C++20
IGNORE_WARNINGS_POP

//
// Typed tests
// https://google.github.io/googletest/advanced.html#typed-tests

using namespace cvl::core;
using testing::Eq;

template < typename T >
class TestCvlCoreAlignment : public testing::Test
{
public:
    using List = std::list< T >;
    static T shared_;
    T value_ { };

    CVL_DEFAULT_ONLY( TestCvlCoreAlignment );

    constexpr T getRandomValue( T min, T max ) const
    {
        // Will be used to obtain a seed for the random number engine
        std::random_device rd;
        // Standard mersenne_twister_engine seeded with rd()
        std::mt19937 gen( rd( ) );

        std::uniform_int_distribution distribution(
            static_cast< int32_t >( min ), static_cast< int32_t >( max ) );

        return static_cast< T >( distribution( gen ) );
    }
};

using Types = testing::Types< int8_t, uint8_t, int16_t, uint16_t, int32_t,
                              uint32_t, int64_t, uint64_t, size_t >;

TYPED_TEST_SUITE(
    TestCvlCoreAlignment,
    Types ); // NOLINT(clang-diagnostic-gnu-zero-variadic-macro-arguments)

TYPED_TEST( TestCvlCoreAlignment, Align8 )
{
    const TypeParam value =
        this->getRandomValue( TypeParam { 73 }, TypeParam { 79 } );

    const auto alignedValue = alignTo( value, TypeParam { 8 } );

    ASSERT_EQ( alignedValue, static_cast< TypeParam >( 80 ) );
}

TYPED_TEST( TestCvlCoreAlignment, Align16 )
{
    const TypeParam value =
        this->getRandomValue( TypeParam { 40 }, TypeParam { 47 } );

    const auto alignedValue = alignTo( value, TypeParam { 16 } );

    ASSERT_EQ( alignedValue, static_cast< TypeParam >( 48 ) );
}

TYPED_TEST( TestCvlCoreAlignment, Align32 )
{
    const TypeParam value =
        this->getRandomValue( TypeParam { 33 }, TypeParam { 50 } );

    const auto alignedValue = alignTo( value, TypeParam { 32 } );

    ASSERT_EQ( alignedValue, static_cast< TypeParam >( 64 ) );
}

TYPED_TEST( TestCvlCoreAlignment, Align40 )
{
    const TypeParam value =
        this->getRandomValue( TypeParam { 0 }, TypeParam { 63 } );

    const auto alignedValue = alignTo( value, TypeParam { 64 } );

    ASSERT_EQ( alignedValue, static_cast< TypeParam >( 64 ) );
}
