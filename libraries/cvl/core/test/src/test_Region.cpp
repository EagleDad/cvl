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

TYPED_TEST( TestCvlCoreRegion, ValueConstructNoThrowTyped )
{
    auto imgPtr = std::make_shared< Image< TypeParam, 1 > >( 512, 256, false );

    EXPECT_NO_THROW( Region( imgPtr, TypeParam { 5 } ) );
}

TYPED_TEST( TestCvlCoreRegion, ValueConstructThrowTyped )
{
    std::shared_ptr< Image< TypeParam, 1 > > imgPtr = nullptr;

    EXPECT_THROW( Region( imgPtr, TypeParam { 5 } ), Error );
}

TYPED_TEST( TestCvlCoreRegion, ValueConstructTyped )
{
    auto imgPtr = std::make_shared< Image< TypeParam, 1 > >( 512, 256, false );

    auto region = Region( imgPtr, TypeParam { 5 } );

    ASSERT_EQ( region.getLabelNumber( ), TypeParam { 5 } );

    ASSERT_EQ( region.getLabelImage( ).get( ), imgPtr.get( ) );
}

TYPED_TEST( TestCvlCoreRegion, ConnectionFilledRect )
{
    // 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0
    // 0 1 1 1 1 1 1 0
    // 0 1 1 1 1 1 1 0
    // 0 1 1 1 1 1 1 0
    // 0 1 1 1 1 1 1 0
    // 0 0 0 0 0 0 0 0
    // 0 0 0 0 0 0 0 0

    constexpr auto width = 8;
    constexpr auto height = 8;
    constexpr auto size = width * height;
    std::vector< uint8_t > imageDataRaw( size );

    imageDataRaw = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF,
                     0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                     0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00,
                     0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00,
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                     0x00, 0x00, 0x00, 0x00 };

    Image< TypeParam, 1 > image( width, height, true );

    size_t vectorIdx = 0;
    for ( int32_t y = 0; y < image.getHeight( ); y++ )
    {
        for ( int32_t x = 0; x < image.getWidth( ); x++ )
        {
            image.at( y, x ) =
                static_cast< TypeParam >( imageDataRaw[ vectorIdx++ ] );
        }
    }

    auto imgPtr = std::make_shared< Image< TypeParam, 1 > >( image.clone( ) );

    std::vector< TypeParam > imageData( size );

    for ( size_t i = 0; i < imageDataRaw.size( ); i++ )
    {
        imageData[ i ] = static_cast< TypeParam >( imageDataRaw[ i ] );
    }

    auto region = Region( imgPtr, TypeParam { 0xFF } );

    EXPECT_EQ(
        region.getBoundingRect( ),
        Rectangle< int32_t >( Point< int32_t, 2 >( 1, 2 ), Size( 6, 4 ) ) );

    const auto cmpCenter = Point< double, 2 >( 3.5, 3.5 );
    EXPECT_EQ( region.getCenter( ), cmpCenter );

    EXPECT_EQ( region.getArea( ), 24 );
}