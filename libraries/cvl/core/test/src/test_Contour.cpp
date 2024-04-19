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
#include <cvl/core/Contour.h>

//
// Typed tests
// https://google.github.io/googletest/advanced.html#typed-tests

using namespace cvl::core;
using testing::Eq;

template < typename T >
class TestCvlCoreContour : public testing::Test
{
public:
    using List = std::list< T >;
    static T shared_;
    T value_ { };

    TestCvlCoreContour( ) = default;

    ~TestCvlCoreContour( ) override = default;

    TestCvlCoreContour( const TestCvlCoreContour& other ) = delete;

    TestCvlCoreContour( const TestCvlCoreContour&& other ) = delete;

    TestCvlCoreContour& operator=( const TestCvlCoreContour& other ) = delete;

    TestCvlCoreContour& operator=( TestCvlCoreContour&& other ) = delete;

    // TODO: Move to testHelper.h
    constexpr std::vector< Point< T, 2 > >
    getRandomPoints( const size_t dimension )
    {
        // Will be used to obtain a seed for the random number engine
        std::random_device rd;
        // Standard mersenne_twister_engine seeded with rd()
        std::mt19937 gen( rd( ) );

        std::vector< Point< T, 2 > > data;

        if constexpr ( std::is_floating_point< T >( ) )
        {
            std::uniform_real_distribution<> distribution( 1., 1000. );

            data.reserve( dimension );
            for ( size_t i = 0; i < dimension; i++ )
            {
                const auto x = static_cast< T >( distribution( gen ) );
                const auto y = static_cast< T >( distribution( gen ) );
                data.push_back( { x, y } );
            }
        }
        else
        {
            std::uniform_int_distribution<> distribution( 1, 1000 );

            data.reserve( dimension );
            for ( size_t i = 0; i < dimension; i++ )
            {
                const auto x = static_cast< T >( distribution( gen ) );
                const auto y = static_cast< T >( distribution( gen ) );
                data.push_back( { x, y } );
            }
        }

        return data;
    }

    // TODO: Move to testHelper.h
    static constexpr std::vector< double >
    getRandomValues( const size_t dimension )
    {
        // Will be used to obtain a seed for the random number engine
        std::random_device rd;
        // Standard mersenne_twister_engine seeded with rd()
        std::mt19937 gen( rd( ) );

        std::vector< double > data;

        std::uniform_real_distribution<> distribution( 1., 1000. );

        data.reserve( dimension );
        for ( size_t i = 0; i < dimension; i++ )
        {
            data.push_back( static_cast< double >( distribution( gen ) ) );
        }

        return data;
    }
};

using Types = testing::Types< double, int32_t >;

TYPED_TEST_SUITE(
    TestCvlCoreContour,
    Types ); // NOLINT(clang-diagnostic-gnu-zero-variadic-macro-arguments)

TYPED_TEST( TestCvlCoreContour, ValueConstructNoThrowTyped )
{
    constexpr int32_t numberElements = 50;

    const auto contourPoints = this->getRandomPoints( numberElements );
    const auto response = this->getRandomValues( numberElements );
    const auto directions = this->getRandomValues( numberElements );

    EXPECT_NO_THROW(
        Contour< TypeParam >( contourPoints, response, directions ) );
}

TYPED_TEST( TestCvlCoreContour, ValueConstructThrowContourEmptyTyped )
{
    std::vector< Point< TypeParam, 2 > > contourPoints;

    EXPECT_THROW( Contour< TypeParam >( { contourPoints } ), Error );
}

TYPED_TEST( TestCvlCoreContour, ValueConstructThrowUnequalDirectionsTyped )
{
    constexpr int32_t numberElements = 50;

    const auto contourPoints = this->getRandomPoints( numberElements );
    const auto response = this->getRandomValues( numberElements );
    const auto directions = this->getRandomValues( numberElements - 1 );

    EXPECT_THROW( Contour< TypeParam >( contourPoints, directions, response ),
                  Error );
}

TYPED_TEST( TestCvlCoreContour, ValueConstructThrowUnequalResponsesTyped )
{
    constexpr int32_t numberElements = 50;

    const auto contourPoints = this->getRandomPoints( numberElements );
    const auto response = this->getRandomValues( numberElements - 1 );
    const auto directions = this->getRandomValues( numberElements );

    EXPECT_THROW( Contour< TypeParam >( contourPoints, directions, response ),
                  Error );
}

TYPED_TEST( TestCvlCoreContour, ValueConstructTyped )
{
    constexpr int32_t numberElements = 50;

    const auto contourPoints = this->getRandomPoints( numberElements );
    const auto response = this->getRandomValues( numberElements );
    const auto directions = this->getRandomValues( numberElements );

    auto contour = std::make_unique< Contour< TypeParam > >(
        contourPoints, response, directions );

    ASSERT_EQ( contourPoints, contour->getContourPoints( ) );
    ASSERT_EQ( response, contour->getResponses( ) );
    ASSERT_EQ( directions, contour->getDirections( ) );
}