// CVL includes
#include <cvl/core/Compare.h>
#include <cvl/core/macros.h>

IGNORE_WARNINGS_GTEST_PUSH
//#include <gmock/gmock.h>
#include <gtest/gtest.h>
IGNORE_WARNINGS_POP

using namespace cvl::core;

TEST( TestBegCoreComapre, CompareFloatEqual )
{
    const auto value =
        static_cast< float >( std::rand( ) ); // NOLINT(concurrency-mt-unsafe)

    ASSERT_EQ( isEqual( value, value ), true );
}

TEST( TestBegCoreComapre, CompareIntEqual )
{
    const auto value = std::rand( ); // NOLINT(concurrency-mt-unsafe)

    ASSERT_EQ( isEqual( value, value ), true );
}

TEST( TestBegCoreComapre, CompareFloatEqualUlb )
{
    const auto value =
        static_cast< float >( std::rand( ) ); // NOLINT(concurrency-mt-unsafe)

    ASSERT_EQ( isEqual( value, value, 1 ), true );
}

TEST( TestBegCoreComapre, CheckZeroInt )
{
    ASSERT_EQ( isZero( 0 ), true );
}

TEST( TestBegCoreComapre, CheckZeroDouble )
{
    ASSERT_EQ( isZero( 0.0 ), true );
}

TEST( TestBegCoreComapre, CheckZeroDoubleNumLimit )
{
    ASSERT_EQ( isZero( std::numeric_limits< double >::epsilon( ) ), true );
}

TEST( TestBegCoreComapre, CompareFloatGE )
{
    constexpr auto e = std::numeric_limits< double >::epsilon( ) * 100.0;
    constexpr auto a = 125.00000000000001;
    constexpr auto b = 125.00000000000000;

    ASSERT_EQ( isGreaterEqual( a, b, e ), true );
    ASSERT_EQ( isGreaterEqual( b, a, e ), true );
}

TEST( TestBegCoreComapre, CompareFloatLE )
{
    constexpr auto e = std::numeric_limits< double >::epsilon( ) * 100.0;
    constexpr auto a = 125.00000000000001;
    constexpr auto b = 125.00000000000000;

    ASSERT_EQ( isLowerEqual( a, b, e ), true );
    ASSERT_EQ( isLowerEqual( b, a, e ), true );
}

TEST( TestBegCoreComapre, CompareIntGE )
{
    constexpr auto a = 125;
    constexpr auto b = 126;
    constexpr auto c = 125;

    ASSERT_EQ( isGreaterEqual( a, b ), false );
    ASSERT_EQ( isGreaterEqual( b, c ), true );
    ASSERT_EQ( isGreaterEqual( a, c ), true );
}

TEST( TestBegCoreComapre, CompareIntLE )
{
    constexpr auto a = 125;
    constexpr auto b = 126;
    constexpr auto c = 125;

    ASSERT_EQ( isLowerEqual( a, b ), true );
    ASSERT_EQ( isLowerEqual( b, c ), false );
    ASSERT_EQ( isLowerEqual( a, c ), true );
}