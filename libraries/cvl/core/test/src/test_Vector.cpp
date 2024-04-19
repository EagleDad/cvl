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
#include <cvl/core/Vector.h>

//
// Typed tests
// https://google.github.io/googletest/advanced.html#typed-tests

using namespace cvl::core;
using testing::Eq;

template < typename T >
class TestCvlCoreVector : public testing::Test
{
public:
    using List = std::list< T >;
    static T shared_;
    T value_ { };

    TestCvlCoreVector( ) = default;

    ~TestCvlCoreVector( ) override = default;

    TestCvlCoreVector( const TestCvlCoreVector& other ) = delete;

    TestCvlCoreVector( const TestCvlCoreVector&& other ) = delete;

    TestCvlCoreVector& operator=( const TestCvlCoreVector& other ) = delete;

    TestCvlCoreVector& operator=( TestCvlCoreVector&& other ) = delete;

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
    constexpr auto getArray( const std::vector< T >& data )
    {
        std::array< T, DIMENSION > arrayData { };
        std::copy_n( data.begin( ), DIMENSION, arrayData.begin( ) );
        return arrayData;
    }

    template < int32_t DIMENSION >
    constexpr auto getVector( const std::vector< T >& data )
    {
        std::array< T, DIMENSION > arrayData { };
        std::copy_n( data.begin( ), DIMENSION, arrayData.begin( ) );
        return Vector< DIMENSION >( arrayData );
    }

    constexpr T getRandomValue( ) const
    {
        // Will be used to obtain a seed for the random number engine
        std::random_device rd;
        // Standard mersenne_twister_engine seeded with rd()
        std::mt19937 gen( rd( ) );

        if constexpr ( std::is_floating_point< T >( ) )
        {
            std::uniform_real_distribution<> distribution( 1., 1000. );
            return static_cast< T >( distribution( gen ) );
        }
        else
        {
            std::uniform_int_distribution<> distribution( 1, 1000 );
            return static_cast< T >( distribution( gen ) );
        }
    }
};

using Types = testing::Types< double >;

TYPED_TEST_SUITE(
    TestCvlCoreVector,
    Types ); // NOLINT(clang-diagnostic-gnu-zero-variadic-macro-arguments)

TEST( TestCvlCoreVector, DefaultConstruct1DTyped )
{
    Vector< 1 > vector;

    ASSERT_EQ( vector.at( 0 ), 0 );

    ASSERT_EQ( vector[ 0 ], 0 );
}

TEST( TestCvlCoreVector, DefaultConstruct2DTyped )
{
    Vector< 2 > vector;

    ASSERT_EQ( vector.at( 0 ), 0 );
    ASSERT_EQ( vector.at( 1 ), 0 );

    ASSERT_EQ( vector[ 0 ], 0 );
    ASSERT_EQ( vector[ 1 ], 0 );
}

TEST( TestCvlCoreVector, DefaultConstruct3DTyped )
{
    Vector< 3 > vector;

    ASSERT_EQ( vector.at( 0 ), 0 );
    ASSERT_EQ( vector.at( 1 ), 0 );
    ASSERT_EQ( vector.at( 2 ), 0 );

    ASSERT_EQ( vector[ 0 ], 0 );
    ASSERT_EQ( vector[ 1 ], 0 );
    ASSERT_EQ( vector[ 2 ], 0 );
}

TEST( TestCvlCoreVector, DefaultConstruct4DTyped )
{
    constexpr Vector< 4 > vector;

    ASSERT_EQ( vector.at( 0 ), 0 );
    ASSERT_EQ( vector.at( 1 ), 0 );
    ASSERT_EQ( vector.at( 2 ), 0 );
    ASSERT_EQ( vector.at( 3 ), 0 );

    ASSERT_EQ( vector[ 0 ], 0 );
    ASSERT_EQ( vector[ 1 ], 0 );
    ASSERT_EQ( vector[ 2 ], 0 );
    ASSERT_EQ( vector[ 3 ], 0 );
}

TYPED_TEST( TestCvlCoreVector, ValueConstructNDTyped )
{
    constexpr int32_t dimension = 10;

    auto data = this->getRandomValues( dimension );

    Vector< dimension > vector( data[ 0 ],
                                data[ 1 ],
                                data[ 2 ],
                                data[ 3 ],
                                data[ 4 ],
                                data[ 5 ],
                                data[ 6 ],
                                data[ 7 ],
                                data[ 8 ],
                                data[ 9 ] );

    for ( size_t i = 0; i < static_cast< size_t >( dimension ); i++ )
    {
        ASSERT_EQ( vector.at( i ), data[ i ] );
    }
}

TYPED_TEST( TestCvlCoreVector, ValueConstructArrayTyped )
{
    constexpr int32_t dimension = 10;

    auto data = this->getRandomValues( dimension );

    const Vector< dimension > vector(
        this->template getVector< dimension >( data ) );

    for ( size_t i = 0; i < static_cast< size_t >( dimension ); i++ )
    {
        ASSERT_EQ( vector.at( i ), data[ i ] );
    }
}

TYPED_TEST( TestCvlCoreVector, CopyConstructTyped )
{
    constexpr int32_t dimension = 10;

    auto data = this->getRandomValues( dimension );

    const Vector< dimension > vector1(
        this->template getVector< dimension >( data ) );

    Vector< dimension > vector2( vector1 );

    for ( size_t i = 0; i < static_cast< size_t >( dimension ); i++ )
    {
        ASSERT_EQ( vector1.at( i ), data[ i ] );
        ASSERT_EQ( vector1.at( i ), vector2.at( i ) );
    }
}

TYPED_TEST( TestCvlCoreVector, MoveConstructTyped )
{
    constexpr int32_t dimension = 10;

    auto data = this->getRandomValues( dimension );

    const Vector< dimension > vector1(
        this->template getVector< dimension >( data ) );

    Vector< dimension > vectorToMove(
        this->template getVector< dimension >( data ) );

    Vector< dimension > vector2( std::move( vectorToMove ) );

    for ( size_t i = 0; i < static_cast< size_t >( dimension ); i++ )
    {
        ASSERT_EQ( vector1[ i ], vector2[ i ] );
    }
}

TYPED_TEST( TestCvlCoreVector, AssignmentOperatorTyped )
{
    constexpr int32_t dimension = 10;

    auto data = this->getRandomValues( dimension );

    const Vector< dimension > vector1(
        this->template getVector< dimension >( data ) );

    Vector< dimension > vector2;

    vector2 = vector1;

    for ( size_t i = 0; i < static_cast< size_t >( dimension ); i++ )
    {
        ASSERT_EQ( vector1.at( i ), vector2.at( i ) );
    }
}

TYPED_TEST( TestCvlCoreVector, MoveOperatorTyped )
{
    constexpr int32_t dimension = 10;

    auto data = this->getRandomValues( dimension );

    const Vector< dimension > vector1(
        this->template getVector< dimension >( data ) );

    Vector< dimension > vector2;

    Vector< dimension > vectorToMove( vector1 );

    vector2 = std::move( vectorToMove );

    for ( size_t i = 0; i < static_cast< size_t >( dimension ); i++ )
    {
        ASSERT_EQ( vector1[ i ], vector2[ i ] );
    }
}

TYPED_TEST( TestCvlCoreVector, VectorFromPointTyped )
{
    constexpr int32_t dimension = 10;

    auto data = this->getRandomValues( dimension );

    const Point< double, dimension > point(
        this->getArray< dimension >( data ) );

    const Vector< dimension > vector( point );

    for ( size_t i = 0; i < static_cast< size_t >( dimension ); i++ )
    {
        ASSERT_EQ( vector[ i ], point[ i ] );
    }
}

TEST( TestCvlCoreVector, VectorFromPoint2i )
{
    constexpr int32_t dimension = 2;

    const Point< int32_t, dimension > point( 1, 4 );

    const Vector< dimension > vector( point );

    for ( size_t i = 0; i < static_cast< size_t >( dimension ); i++ )
    {
        ASSERT_EQ( vector[ i ], point[ i ] );
    }
}

TYPED_TEST( TestCvlCoreVector, VectorFromTwoPointsTyped )
{
    constexpr int32_t dimension = 10;

    auto data1 = this->getRandomValues( dimension );
    auto data2 = this->getRandomValues( dimension );

    const Point< double, dimension > point1(
        this->getArray< dimension >( data1 ) );

    const Point< double, dimension > point2(
        this->getArray< dimension >( data2 ) );

    const Vector< dimension > vector( point1, point2 );

    for ( size_t i = 0; i < static_cast< size_t >( dimension ); i++ )
    {
        ASSERT_EQ( vector[ i ], point2[ i ] - point1[ i ] );
    }
}

TYPED_TEST( TestCvlCoreVector, EqualOperatorTyped )
{
    constexpr int32_t dimension = 10;

    auto data = this->getRandomValues( dimension );

    const Vector< dimension > vector1(
        this->template getVector< dimension >( data ) );

    const Vector< dimension > vector2(
        this->template getVector< dimension >( data ) );

    ASSERT_EQ( vector1, vector2 );
}

TYPED_TEST( TestCvlCoreVector, NotEqualOperatorTyped )
{
    constexpr int32_t dimension = 10;

    auto data1 = this->getRandomValues( dimension );
    auto data2 = this->getRandomValues( dimension );

    const Vector< dimension > vector1(
        this->template getVector< dimension >( data1 ) );

    const Vector< dimension > vector2(
        this->template getVector< dimension >( data2 ) );

    ASSERT_NE( vector1, vector2 );
}

TYPED_TEST( TestCvlCoreVector, OperatorAddNewVectorTyped )
{
    constexpr int32_t dimension = 10;

    auto data1 = this->getRandomValues( dimension );
    auto data2 = this->getRandomValues( dimension );

    const Vector< dimension > vector1(
        this->template getVector< dimension >( data1 ) );

    const Vector< dimension > vector2(
        this->template getVector< dimension >( data2 ) );

    const auto pointRes = vector1 + vector2;

    for ( size_t i = 0; i < static_cast< size_t >( dimension ); i++ )
    {
        ASSERT_EQ( pointRes.at( i ), data1.at( i ) + data2.at( i ) );
    }
}

TYPED_TEST( TestCvlCoreVector, OperatorAddSameVectorTyped )
{
    constexpr int32_t dimension = 10;

    auto data = this->getRandomValues( dimension );

    Vector< dimension > vector1(
        this->template getVector< dimension >( data ) );

    const Vector< dimension > vector2(
        this->template getVector< dimension >( data ) );

    vector1 += vector2;

    for ( size_t i = 0; i < static_cast< size_t >( dimension ); i++ )
    {
        ASSERT_EQ( vector1.at( i ), data.at( i ) + data.at( i ) );
    }
}

TYPED_TEST( TestCvlCoreVector, OperatorSubtractNewVectorTyped )
{
    constexpr int32_t dimension = 10;

    auto data1 = this->getRandomValues( dimension );
    auto data2 = this->getRandomValues( dimension );

    const Vector< dimension > vector1(
        this->template getVector< dimension >( data1 ) );

    const Vector< dimension > vector2(
        this->template getVector< dimension >( data2 ) );

    const auto pointRes = vector1 - vector2;

    for ( size_t i = 0; i < static_cast< size_t >( dimension ); i++ )
    {
        ASSERT_EQ( pointRes.at( i ), data1.at( i ) - data2.at( i ) );
    }
}

TYPED_TEST( TestCvlCoreVector, OperatorSubtractSameVectorTyped )
{
    constexpr int32_t dimension = 10;

    auto data = this->getRandomValues( dimension );

    Vector< dimension > vector1(
        this->template getVector< dimension >( data ) );

    const Vector< dimension > vector2(
        this->template getVector< dimension >( data ) );

    vector1 -= vector2;

    for ( size_t i = 0; i < static_cast< size_t >( dimension ); i++ )
    {
        ASSERT_EQ( vector1.at( i ), data.at( i ) - data.at( i ) );
    }
}

TYPED_TEST( TestCvlCoreVector, OperatorScaleNewVectorTyped )
{
    constexpr int32_t dimension = 10;

    auto data = this->getRandomValues( dimension );
    auto scale = this->getRandomValue( );

    const Vector< dimension > vector(
        this->template getVector< dimension >( data ) );

    const auto scaledVector = vector * scale;

    for ( size_t i = 0; i < static_cast< size_t >( dimension ); i++ )
    {
        ASSERT_EQ( scaledVector.at( i ), data.at( i ) * scale );
    }
}

TYPED_TEST( TestCvlCoreVector, OperatorScaleSameVectorTyped )
{
    constexpr int32_t dimension = 10;

    auto data = this->getRandomValues( dimension );
    auto scale = this->getRandomValue( );

    Vector< dimension > vector( this->template getVector< dimension >( data ) );

    vector *= scale;

    for ( size_t i = 0; i < static_cast< size_t >( dimension ); i++ )
    {
        ASSERT_EQ( vector.at( i ), data.at( i ) * scale );
    }
}

TYPED_TEST( TestCvlCoreVector, OperatorDotProductTyped )
{
    constexpr int32_t dimension = 10;

    auto data1 = this->getRandomValues( dimension );
    auto data2 = this->getRandomValues( dimension );

    const Vector< dimension > vector1(
        this->template getVector< dimension >( data1 ) );

    const Vector< dimension > vector2(
        this->template getVector< dimension >( data2 ) );

    auto dot = vector1 * vector2;

    auto dotCmp = 0.0;

    for ( size_t i = 0; i < static_cast< size_t >( dimension ); i++ )
    {
        dotCmp += data1[ i ] * data2[ i ];
    }

    ASSERT_EQ( dot, dotCmp );
}

TYPED_TEST( TestCvlCoreVector, AccessorArrayTyped )
{
    constexpr int32_t dimension = 10;

    auto data = this->getRandomValues( dimension );

    Vector< dimension > vector( this->template getVector< dimension >( data ) );

    std::array< TypeParam, dimension > arrayData { };
    std::copy_n( data.begin( ), dimension, arrayData.begin( ) );

    ASSERT_EQ( vector.elements( ), arrayData );
}

TYPED_TEST( TestCvlCoreVector, AccessorArrayConstTyped )
{
    constexpr int32_t dimension = 10;

    auto data = this->getRandomValues( dimension );

    const Vector< dimension > vector(
        this->template getVector< dimension >( data ) );

    std::array< TypeParam, dimension > arrayData { };
    std::copy_n( data.begin( ), dimension, arrayData.begin( ) );

    ASSERT_EQ( vector.elements( ), arrayData );
}

TYPED_TEST( TestCvlCoreVector, IteratorBeginTyped )
{
    constexpr int32_t dimension = 10;

    auto data = this->getRandomValues( dimension );

    Vector< dimension > vector( this->template getVector< dimension >( data ) );

    ASSERT_EQ( *data.begin( ), *vector.begin( ) );
}

TYPED_TEST( TestCvlCoreVector, IteratorConstBeginTyped )
{
    constexpr int32_t dimension = 10;

    auto data = this->getRandomValues( dimension );

    const Vector< dimension > vector(
        this->template getVector< dimension >( data ) );

    ASSERT_EQ( *data.begin( ), *vector.begin( ) );
}

TYPED_TEST( TestCvlCoreVector, IteratorEndTyped )
{
    constexpr int32_t dimension = 10;

    auto data = this->getRandomValues( dimension );

    Vector< dimension > vector( this->template getVector< dimension >( data ) );

    ASSERT_EQ( *( data.end( ) - 1 ), *( vector.end( ) - 1 ) );
}

TYPED_TEST( TestCvlCoreVector, IteratorConstEndTyped )
{
    constexpr int32_t dimension = 10;

    auto data = this->getRandomValues( dimension );

    const Vector< dimension > vector(
        this->template getVector< dimension >( data ) );

    ASSERT_EQ( *( data.end( ) - 1 ), *( vector.end( ) - 1 ) );
}
