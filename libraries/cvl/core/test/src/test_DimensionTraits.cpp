#include <cvl/core/macros.h>

// GTest includes
IGNORE_WARNINGS_GTEST_PUSH
#include <gtest/gtest.h>
// #include <gmock/gmock.h> // GTest Update required because of removed function
//  in C++20
IGNORE_WARNINGS_POP

// STD includes
#include <list>

// CVL includes
#include <cvl/core/DimensionTraits.h>

//
// Typed tests
// https://google.github.io/googletest/advanced.html#typed-tests

using namespace cvl::core;
using testing::Eq;

template < typename T >
class TestCvlCoreDimensionTraits : public testing::Test
{
public:
    using List = std::list< T >;
    static T shared_;
    T value_ { };

    TestCvlCoreDimensionTraits( ) = default;

    ~TestCvlCoreDimensionTraits( ) override = default;

    TestCvlCoreDimensionTraits( const TestCvlCoreDimensionTraits& other ) =
        delete;

    TestCvlCoreDimensionTraits( const TestCvlCoreDimensionTraits&& other ) =
        delete;

    TestCvlCoreDimensionTraits&
    operator=( const TestCvlCoreDimensionTraits& other ) = delete;

    TestCvlCoreDimensionTraits&
    operator=( TestCvlCoreDimensionTraits&& other ) = delete;
};

using Types =
    testing::Types< int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t,
                    int64_t, uint64_t, float, double, size_t >;

TYPED_TEST_SUITE(
    TestCvlCoreDimensionTraits,
    Types ); // NOLINT(clang-diagnostic-gnu-zero-variadic-macro-arguments)

TYPED_TEST( TestCvlCoreDimensionTraits, DefaultConstructPoint1DTyped )
{
    dimension< Point< TypeParam, 1 > > dim;

    ASSERT_EQ( dim.value, 1 );
}

TYPED_TEST( TestCvlCoreDimensionTraits, RandomPointNDTyped )
{
    dimension< Point< TypeParam, 10 > > dim;

    ASSERT_EQ( dim.value, 10 );
}

TEST( TestCvlCoreDimensionTraits, RandomVectorNDTyped )
{
    dimension< Vector< 10 > > dim;

    ASSERT_EQ( dim.value, 10 );
}
