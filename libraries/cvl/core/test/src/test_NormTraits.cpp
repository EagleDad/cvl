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
#include <cvl/core/NormTraits.h>

//
// Typed tests
// https://google.github.io/googletest/advanced.html#typed-tests

using namespace cvl::core;
using testing::Eq;

template < typename T >
class TestCvlCoreNormTraits : public testing::Test
{
public:
    using List = std::list< T >;
    static T shared_;
    T value_ { };

    TestCvlCoreNormTraits( ) = default;

    ~TestCvlCoreNormTraits( ) override = default;

    TestCvlCoreNormTraits( const TestCvlCoreNormTraits& other ) = delete;

    TestCvlCoreNormTraits( const TestCvlCoreNormTraits&& other ) = delete;

    TestCvlCoreNormTraits&
    operator=( const TestCvlCoreNormTraits& other ) = delete;

    TestCvlCoreNormTraits& operator=( TestCvlCoreNormTraits&& other ) = delete;

    constexpr std::vector< T > getRandomValues( const size_t dimension )
    {
        // Will be used to obtain a seed for the random number engine
        std::random_device rd;
        // Standard mersenne_twister_engine seeded with rd()
        std::mt19937 gen( rd( ) );

        std::vector< T > data;

        if constexpr ( std::is_floating_point< T >( ) )
        {
            std::uniform_real_distribution<> distribution( 1., 1000. );

            data.reserve( dimension );
            for ( size_t i = 0; i < dimension; i++ )
            {
                data.push_back( static_cast< T >( distribution( gen ) ) );
            }
        }
        else
        {
            std::uniform_int_distribution<> distribution( 1, 1000 );

            data.reserve( dimension );
            for ( size_t i = 0; i < dimension; i++ )
            {
                data.push_back( static_cast< T >( distribution( gen ) ) );
            }
        }

        return data;
    }

    template < int32_t DIMENSION >
    constexpr auto getVector( const std::vector< T >& data )
    {
        std::array< double, DIMENSION > arrayData { };

        for ( size_t i = 0; i < data.size( ); i++ )
        {
            arrayData[ i ] = static_cast< double >( data[ i ] );
        }
        return Vector< DIMENSION >( arrayData );
    }
};

using Types =
    testing::Types< int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t,
                    int64_t, uint64_t, float, double, size_t >;

TYPED_TEST_SUITE(
    TestCvlCoreNormTraits,
    Types ); // NOLINT(clang-diagnostic-gnu-zero-variadic-macro-arguments)

TYPED_TEST( TestCvlCoreNormTraits, NormDefaultPoint1DTyped )
{
    Point< TypeParam, 1 > point1;
    Point< TypeParam, 1 > point2;

    ASSERT_EQ( distance< cvl::core::Norm::Manhattan >( point1, point2 ), 0 );
    ASSERT_EQ( distance< cvl::core::Norm::Euclidean >( point1, point2 ), 0 );
    ASSERT_EQ( distance< cvl::core::Norm::Maximum >( point1, point2 ), 0 );
}

TYPED_TEST( TestCvlCoreNormTraits, NormDefaultPoint10DTyped )
{
    Point< TypeParam, 10 > point1;
    Point< TypeParam, 10 > point2;

    ASSERT_EQ( distance< cvl::core::Norm::Manhattan >( point1, point2 ), 0 );
    ASSERT_EQ( distance< cvl::core::Norm::Euclidean >( point1, point2 ), 0 );
    ASSERT_EQ( distance< cvl::core::Norm::Maximum >( point1, point2 ), 0 );
}

TYPED_TEST( TestCvlCoreNormTraits, NormDefaultPoint2DTyped )
{
    Point< TypeParam, 2 > point1( TypeParam { 1 }, TypeParam { 1 } );
    Point< TypeParam, 2 > point2( TypeParam { 2 }, TypeParam { 2 } );

    ASSERT_EQ( distance< cvl::core::Norm::Manhattan >( point1, point2 ), 2 );
    ASSERT_EQ( distance< cvl::core::Norm::Euclidean >( point1, point2 ),
               std::sqrt( 2 ) );
    ASSERT_EQ( distance< cvl::core::Norm::Maximum >( point1, point2 ), 1 );
}

TYPED_TEST( TestCvlCoreNormTraits, NormDefaultPoint2DOriginTyped )
{
    Point< TypeParam, 2 > point( TypeParam { 1 }, TypeParam { 1 } );

    ASSERT_EQ( norm< cvl::core::Norm::Manhattan >( point ), 2 );
    ASSERT_EQ( norm< cvl::core::Norm::Euclidean >( point ), std::sqrt( 2 ) );
    ASSERT_EQ( norm< cvl::core::Norm::Maximum >( point ), 1 );
}

TEST( TestCvlCoreNormTraits, NormDefaultVector2DOrigin )
{
    const Vector< 2 > vector( 1.0, 1.0 );

    ASSERT_EQ( norm< cvl::core::Norm::Manhattan >( vector ), 2 );
    ASSERT_EQ( norm< cvl::core::Norm::Euclidean >( vector ), std::sqrt( 2 ) );
    ASSERT_EQ( norm< cvl::core::Norm::Maximum >( vector ), 1 );
}

TYPED_TEST( TestCvlCoreNormTraits, NormVectorNDOrigin )
{
    constexpr int32_t dimension = 10;

    auto data = this->getRandomValues( dimension );

    Vector< dimension > vector( this->template getVector< dimension >( data ) );

    auto normCmpEuclidean = 0.0;
    auto normCmpManhattan = 0.0;
    auto normCmpMaximum = 0.0;

    for ( const auto elem : vector.elements( ) )
    {
        normCmpEuclidean += std::pow( elem, 2 );
        normCmpManhattan += std::abs( elem );
        normCmpMaximum = std::max( normCmpMaximum, std::abs( elem ) );
    }

    normCmpEuclidean = std::sqrt( normCmpEuclidean );

    ASSERT_EQ( norm< cvl::core::Norm::Manhattan >( vector ), normCmpManhattan );
    ASSERT_EQ( norm< cvl::core::Norm::Euclidean >( vector ), normCmpEuclidean );
    ASSERT_EQ( norm< cvl::core::Norm::Maximum >( vector ), normCmpMaximum );
}

TEST( TestCvlCoreNormTraits, NormalizeVector3D )
{
    constexpr int32_t dimension = 3;

    const Vector< dimension > vector( 1.0, 2.0, 3.0 );

    {
        const auto normalizedVector =
            normalize< Norm::Manhattan >( vector );

        constexpr auto vecNorm = 1.0 + 2.0 + 3.0;

        ASSERT_EQ( normalizedVector[ 0 ], vector[ 0 ] / vecNorm );
        ASSERT_EQ( normalizedVector[ 1 ], vector[ 1 ] / vecNorm );
        ASSERT_EQ( normalizedVector[ 2 ], vector[ 2 ] / vecNorm );
    }

    {
        const auto normalizedVector =
            normalize< Norm::Euclidean >( vector );

        const auto vecNorm = std::sqrt( 1 + 4 + 9 );

        ASSERT_EQ( normalizedVector[ 0 ], vector[ 0 ] / vecNorm );
        ASSERT_EQ( normalizedVector[ 1 ], vector[ 1 ] / vecNorm );
        ASSERT_EQ( normalizedVector[ 2 ], vector[ 2 ] / vecNorm );
    }

    {
        const auto normalizedVector =
            normalize< Norm::Maximum >( vector );

        constexpr auto vecNorm = 3.0;

        ASSERT_EQ( normalizedVector[ 0 ], vector[ 0 ] / vecNorm );
        ASSERT_EQ( normalizedVector[ 1 ], vector[ 1 ] / vecNorm );
        ASSERT_EQ( normalizedVector[ 2 ], vector[ 2 ] / vecNorm );
    }
}
