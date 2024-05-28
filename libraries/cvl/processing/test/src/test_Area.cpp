#include <cvl/core/macros.h>

// GTest includes
IGNORE_WARNINGS_GTEST_PUSH
#include <gtest/gtest.h>
// #include <gmock/gmock.h> // GTest Update required because of removed function
//  in C++20
IGNORE_WARNINGS_POP

// STD includes
#include <random>

// CVL includes
#include <cvl/core/Image.h>
#include <cvl/core/Region.h>
#include <cvl/processing/Area.h>

using namespace cvl::core;
using namespace cvl::processing;
using testing::Eq;

template < typename T >
class TestCvlProcessingArea : public testing::Test
{
public:
    using List = std::list< T >;
    static T shared_;
    T value_ { };

    CVL_DEFAULT_ONLY( TestCvlProcessingArea );

    static int32_t getRandomValue( )
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
    TestCvlProcessingArea,
    Types ); // NOLINT(clang-diagnostic-gnu-zero-variadic-macro-arguments)

TYPED_TEST( TestCvlProcessingArea, ValueConstruct )
{
    const auto randomArea = static_cast< double >( this->getRandomValue( ) );

    const auto randomLabel = static_cast< int32_t >( this->getRandomValue( ) );

    const Image< uint8_t, 1 > image( 16, 16 );

    Region< uint8_t, AlignedAllocator< uint8_t >, Area > region(
        image, randomLabel, randomArea );

    EXPECT_EQ( region.getLabelNumber( ), randomLabel );
    EXPECT_EQ( region.getArea( ), randomArea );
}
