// CVL includes
#include <cvl/core/Region.h>
#include <cvl/core/macros.h>

// GTest includes
IGNORE_WARNINGS_GTEST_PUSH
#include <gtest/gtest.h>
// #include <gmock/gmock.h> // GTest Update required because of removed function
//  in C++20
IGNORE_WARNINGS_POP

// STD includes
#include <random>
#include <list>

//
// Typed tests
// https://google.github.io/googletest/advanced.html#typed-tests

using namespace cvl::core;
using testing::Eq;

template < typename T >
class TestCvlCoreRegion : public testing::Test
{
public:
    using List = std::list< T >;
    static T shared_;
    T value_ { };

    CVL_DEFAULT_ONLY( TestCvlCoreRegion );
};

using Types = testing::Types< uint8_t, uint16_t, int32_t >;

TYPED_TEST_SUITE(
    TestCvlCoreRegion,
    Types ); // NOLINT(clang-diagnostic-gnu-zero-variadic-macro-arguments)

TYPED_TEST( TestCvlCoreRegion, DefaultCostruct )
{
    auto region = Region< TypeParam >( );

    EXPECT_EQ( region.getLabelNumber( ), 0 );

    EXPECT_EQ( region.getLabelImage( ).getSize( ), SizeI( ) );
}

TYPED_TEST( TestCvlCoreRegion, ValueConstructLabelImageAndNumber )
{
    auto image = Image< TypeParam, 1 >( 512, 256, TypeParam { 5 } );

    auto region = Region( image, TypeParam { 5 } );

    EXPECT_EQ( region.getLabelNumber( ), 5 );

    EXPECT_EQ( region.getLabelImage( ).getSize( ), SizeI( 512, 256 ) );
}

TYPED_TEST( TestCvlCoreRegion, CopyCostruct )
{
    constexpr int32_t width = 256;
    constexpr int32_t stride = width * sizeof( TypeParam );
    constexpr int32_t height = 512;
    constexpr TypeParam value = TypeParam { 5 };
    constexpr size_t bufferSize =
        static_cast< size_t >( stride ) * static_cast< size_t >( height );

    auto image = Image< TypeParam, 1 >( width, height, value );

    Region< TypeParam > regionA( image, value );
    Region< TypeParam > regionB( regionA );

    ASSERT_EQ( image.getWidth( ), regionA.getLabelImage( ).getWidth( ) );
    ASSERT_EQ( image.getWidth( ), regionB.getLabelImage( ).getWidth( ) );

    ASSERT_EQ( image.getHeight( ), regionA.getLabelImage( ).getHeight( ) );
    ASSERT_EQ( image.getHeight( ), regionB.getLabelImage( ).getHeight( ) );

    ASSERT_EQ( image.getStrideInBytes( ),
               regionA.getLabelImage( ).getStrideInBytes( ) );
    ASSERT_EQ( image.getStrideInBytes( ),
               regionB.getLabelImage( ).getStrideInBytes( ) );

    ASSERT_EQ( regionA.getLabelNumber( ), value );
    ASSERT_EQ( regionB.getLabelNumber( ), value );

    ASSERT_EQ( std::memcmp( image.getData( ),
                            regionA.getLabelImage( ).getData( ),
                            bufferSize ),
               0 );

    ASSERT_EQ( std::memcmp( image.getData( ),
                            regionB.getLabelImage( ).getData( ),
                            bufferSize ),
               0 );
}

TYPED_TEST( TestCvlCoreRegion, MoveCostruct )
{
    constexpr int32_t width = 256;
    constexpr int32_t stride = width * sizeof( TypeParam );
    constexpr int32_t height = 512;
    constexpr TypeParam value = TypeParam { 5 };
    constexpr size_t bufferSize =
        static_cast< size_t >( stride ) * static_cast< size_t >( height );

    auto image = Image< TypeParam, 1 >( width, height, value );

    Region< TypeParam > regionA( image, value );
    Region< TypeParam > regionToMove( regionA );
    Region< TypeParam > regionC( std::move( regionToMove ) );

    ASSERT_EQ( image.getWidth( ), regionA.getLabelImage( ).getWidth( ) );
    ASSERT_EQ( image.getWidth( ), regionC.getLabelImage( ).getWidth( ) );

    ASSERT_EQ( image.getHeight( ), regionA.getLabelImage( ).getHeight( ) );
    ASSERT_EQ( image.getHeight( ), regionC.getLabelImage( ).getHeight( ) );

    ASSERT_EQ( image.getStrideInBytes( ),
               regionA.getLabelImage( ).getStrideInBytes( ) );
    ASSERT_EQ( image.getStrideInBytes( ),
               regionC.getLabelImage( ).getStrideInBytes( ) );

    ASSERT_EQ( regionA.getLabelNumber( ), value );
    ASSERT_EQ( regionC.getLabelNumber( ), value );

    ASSERT_EQ( std::memcmp( image.getData( ),
                            regionA.getLabelImage( ).getData( ),
                            bufferSize ),
               0 );

    ASSERT_EQ( std::memcmp( image.getData( ),
                            regionC.getLabelImage( ).getData( ),
                            bufferSize ),
               0 );
}

TYPED_TEST( TestCvlCoreRegion, AssignmentOperator )
{
    constexpr int32_t width = 256;
    constexpr int32_t stride = width * sizeof( TypeParam );
    constexpr int32_t height = 512;
    constexpr TypeParam value = TypeParam { 5 };
    constexpr size_t bufferSize =
        static_cast< size_t >( stride ) * static_cast< size_t >( height );

    auto image = Image< TypeParam, 1 >( width, height, value );

    Region< TypeParam > regionA( image, value );
    Region< TypeParam > regionB;
    regionB = regionA;

    ASSERT_EQ( image.getWidth( ), regionA.getLabelImage( ).getWidth( ) );
    ASSERT_EQ( image.getWidth( ), regionB.getLabelImage( ).getWidth( ) );

    ASSERT_EQ( image.getHeight( ), regionA.getLabelImage( ).getHeight( ) );
    ASSERT_EQ( image.getHeight( ), regionB.getLabelImage( ).getHeight( ) );

    ASSERT_EQ( image.getStrideInBytes( ),
               regionA.getLabelImage( ).getStrideInBytes( ) );
    ASSERT_EQ( image.getStrideInBytes( ),
               regionB.getLabelImage( ).getStrideInBytes( ) );

    ASSERT_EQ( regionA.getLabelNumber( ), value );
    ASSERT_EQ( regionB.getLabelNumber( ), value );

    ASSERT_EQ( std::memcmp( image.getData( ),
                            regionA.getLabelImage( ).getData( ),
                            bufferSize ),
               0 );

    ASSERT_EQ( std::memcmp( image.getData( ),
                            regionB.getLabelImage( ).getData( ),
                            bufferSize ),
               0 );
}

TYPED_TEST( TestCvlCoreRegion, MoveOperator )
{
    constexpr int32_t width = 256;
    constexpr int32_t stride = width * sizeof( TypeParam );
    constexpr int32_t height = 512;
    constexpr TypeParam value = TypeParam { 5 };
    constexpr size_t bufferSize =
        static_cast< size_t >( stride ) * static_cast< size_t >( height );

    auto image = Image< TypeParam, 1 >( width, height, value );

    Region< TypeParam > regionA( image, value );
    Region< TypeParam > regionToMove( regionA );
    Region< TypeParam > regionC;
    regionC = std::move( regionToMove );

    ASSERT_EQ( image.getWidth( ), regionA.getLabelImage( ).getWidth( ) );
    ASSERT_EQ( image.getWidth( ), regionC.getLabelImage( ).getWidth( ) );

    ASSERT_EQ( image.getHeight( ), regionA.getLabelImage( ).getHeight( ) );
    ASSERT_EQ( image.getHeight( ), regionC.getLabelImage( ).getHeight( ) );

    ASSERT_EQ( image.getStrideInBytes( ),
               regionA.getLabelImage( ).getStrideInBytes( ) );
    ASSERT_EQ( image.getStrideInBytes( ),
               regionC.getLabelImage( ).getStrideInBytes( ) );

    ASSERT_EQ( regionA.getLabelNumber( ), value );
    ASSERT_EQ( regionC.getLabelNumber( ), value );

    ASSERT_EQ( std::memcmp( image.getData( ),
                            regionA.getLabelImage( ).getData( ),
                            bufferSize ),
               0 );

    ASSERT_EQ( std::memcmp( image.getData( ),
                            regionC.getLabelImage( ).getData( ),
                            bufferSize ),
               0 );
}