// CVL includes
#include <cvl/core/macros.h>
#include <cvl/processing/ConnectedComponents.h>

// GTest includes
IGNORE_WARNINGS_GTEST_PUSH
#include <gtest/gtest.h>
// #include <gmock/gmock.h> // GTest Update required because of removed function
//  in C++20
IGNORE_WARNINGS_POP

// STD includes
#include <list>
#include <random>

using namespace cvl::core;
using namespace cvl::processing;
using testing::Eq;

template < typename T >
class TestCvlProcessingConnectedComponents : public testing::Test
{
public:
    using List = std::list< T >;
    static T shared_;
    T value_ { };

    CVL_DEFAULT_ONLY( TestCvlProcessingConnectedComponents );

    Image< T, 1 > getGrayWedgeImage( )
    {
        Image< T, 1 > imageGrayWedge( 256, 256 );

        for ( int32_t y = 0; y < imageGrayWedge.getHeight( ); y++ )
        {
            const auto srcPtr = imageGrayWedge.getRowPointer( y );

            for ( int32_t x = 0; x < imageGrayWedge.getWidth( ); x++ )
            {
                srcPtr[ x ] = static_cast< T >( x );
            }
        }

        return imageGrayWedge;
    }

    static int32_t getRandomThresholdValue( )
    {
        std::random_device rd;     // a seed source for the random number engine
        std::mt19937 gen( rd( ) ); // mersenne_twister_engine seeded with rd()
        std::uniform_int_distribution< std::mt19937::result_type > dist( 0,
                                                                         255 );

        return dist( gen );
    }
};

using Types = testing::Types< uint8_t, int16_t, uint16_t /*, float, double */ >;

TYPED_TEST_SUITE(
    TestCvlProcessingConnectedComponents,
    Types ); // NOLINT(clang-diagnostic-gnu-zero-variadic-macro-arguments)

TYPED_TEST( TestCvlProcessingConnectedComponents, ConnectedComponentsImage )
{
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    // 0 0 1 1 0 0 1 1 0 0 1 1 0 0 1 1 0
    // 0 1 1 1 1 1 1 1 1 0 0 1 1 1 1 0 0
    // 0 0 0 1 1 1 1 0 0 0 1 1 1 1 0 0 0
    // 0 0 1 1 1 1 0 0 0 1 1 1 0 0 1 1 0
    // 0 1 1 1 0 0 1 1 0 0 0 1 1 1 0 0 0
    // 0 0 1 1 0 0 0 0 0 1 1 0 0 0 1 1 0
    // 0 0 0 0 0 0 1 1 1 1 0 0 1 1 1 1 0
    // 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0

    constexpr auto width = 17;
    constexpr auto height = 9;
    constexpr auto size = width * height;
    std::vector< uint8_t > imageData( size );

    imageData = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF,
        0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF,
        0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
        0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF,
        0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF,
        0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00,
        0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00,
        0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF,
        0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

    Image< uint8_t, 1 > image( width, height, true );

    size_t vectorIdx = 0;
    for ( int32_t y = 0; y < image.getHeight( ); y++ )
    {
        for ( int32_t x = 0; x < image.getWidth( ); x++ )
        {
            image.at( y, x ) = imageData[ vectorIdx++ ];
        }
    }

    Image< TypeParam, 1 > labelImage( width, height, true );

    /*  auto region = std::make_unique<
          Region< uint8_t, AlignedAllocator< uint8_t >, Area, Center > >(
          image, int32_t { }, double { }, Point2f { } );*/

    // std::vector< std::unique_ptr<
    //     Region< uint8_t, AlignedAllocator< uint8_t >, Area, Center > > >
    //     regionsOut;

    // regionsOut.push_back( std::move( region ) );

    const auto regions =
        cvl::processing::connectedComponents< TypeParam,
                                              AlignedAllocator< uint8_t >, Area >(
            image, labelImage );

    EXPECT_EQ( regions.size( ), 2 );
}

TYPED_TEST( TestCvlProcessingConnectedComponents, ConnectionFilledRect )
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
    std::vector< uint8_t > imageData( size );

    imageData = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF,
                  0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                  0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00,
                  0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                  0x00, 0x00, 0x00, 0x00 };

    Image< uint8_t, 1 > image( width, height, true );

    size_t vectorIdx = 0;
    for ( int32_t y = 0; y < image.getHeight( ); y++ )
    {
        for ( int32_t x = 0; x < image.getWidth( ); x++ )
        {
            image.at( y, x ) = imageData[ vectorIdx++ ];
        }
    }

    Image< TypeParam, 1 > labelImage( width, height, true );

    auto regions =
        cvl::processing::connectedComponents< TypeParam >( image, labelImage );

    EXPECT_EQ( regions.size( ), 1 );

    /* EXPECT_EQ(
         regions[ 0 ]->getBoundingRect( ),
         Rectangle< int32_t >( Point< int32_t, 2 >( 1, 2 ), Size( 6, 4 ) ) );

     const auto cmpCenter = Point< double, 2 >( 3.5, 3.5 );
     EXPECT_EQ( regions[ 0 ]->getCenter( ), cmpCenter );

     EXPECT_EQ( regions[ 0 ]->getArea( ), 24 );*/
}