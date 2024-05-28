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

// CVL includes
#include <cvl/processing/Threshold.h>

using namespace cvl::core;
using namespace cvl::processing;
using testing::Eq;

template < typename T >
class TestCvlProcessingThreshold : public testing::Test
{
public:
    using List = std::list< T >;
    static T shared_;
    T value_ { };

    CVL_DEFAULT_ONLY( TestCvlProcessingThreshold );

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

using Types = testing::Types< uint8_t, int16_t, uint16_t, float, double >;

TYPED_TEST_SUITE(
    TestCvlProcessingThreshold,
    Types ); // NOLINT(clang-diagnostic-gnu-zero-variadic-macro-arguments)

TYPED_TEST( TestCvlProcessingThreshold, FixThresholdEmptyRegion )
{
    const auto threshValue =
        static_cast< TypeParam >( this->getRandomThresholdValue( ) );
    const auto testImage = this->getGrayWedgeImage( );

    {
        Region< uint8_t > region;
        threshold( testImage, region, threshValue, uint8_t { 255 } );

        EXPECT_EQ( region.getLabelImage( ).getSize( ), testImage.getSize( ) );
        EXPECT_EQ( region.getLabelNumber( ), uint8_t { 255 } );

        for ( int32_t y = 0; y < region.getLabelImage( ).getHeight( ); y++ )
        {
            const auto srcPtr = region.getLabelImage( ).getRowPointer( y );

            for ( int32_t x = 0; x < region.getLabelImage( ).getWidth( ); x++ )
            {
                if ( x > static_cast< int32_t >( threshValue ) )
                {
                    EXPECT_EQ( srcPtr[ x ], uint8_t { 255 } );
                }
                else
                {
                    EXPECT_EQ( srcPtr[ x ], uint8_t { 0 } );
                }
            }
        }
    }
}
