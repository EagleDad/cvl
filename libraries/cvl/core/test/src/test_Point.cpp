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
#include <cvl/core/Point.h>

//
// Typed tests
// https://google.github.io/googletest/advanced.html#typed-tests

using namespace cvl::core;
using testing::Eq;

template < typename T >
class TestCvlCorePoint : public testing::Test
{
public:
    using List = std::list< T >;
    static T shared_;
    T value_ { };

    TestCvlCorePoint( ) = default;

    ~TestCvlCorePoint( ) override = default;

    TestCvlCorePoint( const TestCvlCorePoint& other ) = delete;

    TestCvlCorePoint( const TestCvlCorePoint&& other ) = delete;

    TestCvlCorePoint& operator=( const TestCvlCorePoint& other ) = delete;

    TestCvlCorePoint& operator=( TestCvlCorePoint&& other ) = delete;

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
    constexpr auto getPoint( const std::vector< T >& data )
    {
        std::array< T, DIMENSION > arrayData { };
        std::copy_n( data.begin( ), DIMENSION, arrayData.begin( ) );
        return Point< T, DIMENSION >( arrayData );
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

using Types =
    testing::Types< int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t,
                    int64_t, uint64_t, float, double, size_t >;

TYPED_TEST_SUITE(
    TestCvlCorePoint,
    Types ); // NOLINT(clang-diagnostic-gnu-zero-variadic-macro-arguments)

TYPED_TEST( TestCvlCorePoint, DefaultConstruct1DTyped )
{
    Point< TypeParam, 1 > point;

    ASSERT_EQ( point.at( 0 ), 0 );

    ASSERT_EQ( point[ 0 ], 0 );
}

TYPED_TEST( TestCvlCorePoint, DefaultConstruct2DTyped )
{
    Point< TypeParam, 2 > point;

    ASSERT_EQ( point.at( 0 ), 0 );
    ASSERT_EQ( point.at( 1 ), 0 );

    ASSERT_EQ( point[ 0 ], 0 );
    ASSERT_EQ( point[ 1 ], 0 );
}

TYPED_TEST( TestCvlCorePoint, DefaultConstruct3DTyped )
{
    Point< TypeParam, 3 > point;

    ASSERT_EQ( point.at( 0 ), 0 );
    ASSERT_EQ( point.at( 1 ), 0 );
    ASSERT_EQ( point.at( 2 ), 0 );

    ASSERT_EQ( point[ 0 ], 0 );
    ASSERT_EQ( point[ 1 ], 0 );
    ASSERT_EQ( point[ 2 ], 0 );
}

TYPED_TEST( TestCvlCorePoint, DefaultConstruct4DTyped )
{
    Point< TypeParam, 4 > point;

    ASSERT_EQ( point.at( 0 ), 0 );
    ASSERT_EQ( point.at( 1 ), 0 );
    ASSERT_EQ( point.at( 2 ), 0 );
    ASSERT_EQ( point.at( 3 ), 0 );

    ASSERT_EQ( point[ 0 ], 0 );
    ASSERT_EQ( point[ 1 ], 0 );
    ASSERT_EQ( point[ 2 ], 0 );
    ASSERT_EQ( point[ 3 ], 0 );
}

TYPED_TEST( TestCvlCorePoint, ValueConstructNDTyped )
{
    constexpr int32_t dimension = 10;

    auto data = this->getRandomValues( dimension );

    Point< TypeParam, dimension > point( data[ 0 ],
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
        ASSERT_EQ( point.at( i ), data[ i ] );
    }
}

TYPED_TEST( TestCvlCorePoint, ValueConstructArrayTyped )
{
    constexpr int32_t dimension = 10;

    auto data = this->getRandomValues( dimension );

    const Point< TypeParam, dimension > point(
        this->template getPoint< dimension >( data ) );

    for ( size_t i = 0; i < static_cast< size_t >( dimension ); i++ )
    {
        ASSERT_EQ( point.at( i ), data[ i ] );
    }
}

TYPED_TEST( TestCvlCorePoint, CopyConstructTyped )
{
    constexpr int32_t dimension = 10;

    auto data = this->getRandomValues( dimension );

    const Point< TypeParam, dimension > point1(
        this->template getPoint< dimension >( data ) );

    Point< TypeParam, dimension > point2( point1 );

    for ( size_t i = 0; i < static_cast< size_t >( dimension ); i++ )
    {
        ASSERT_EQ( point1.at( i ), data[ i ] );
        ASSERT_EQ( point1.at( i ), point2.at( i ) );
    }
}

TYPED_TEST( TestCvlCorePoint, MoveConstructTyped )
{
    constexpr int32_t dimension = 10;

    auto data = this->getRandomValues( dimension );

    const Point< TypeParam, dimension > point1(
        this->template getPoint< dimension >( data ) );

    Point< TypeParam, dimension > pointToMove(
        this->template getPoint< dimension >( data ) );

    Point< TypeParam, dimension > point2( std::move( pointToMove ) );

    for ( size_t i = 0; i < static_cast< size_t >( dimension ); i++ )
    {
        ASSERT_EQ( point1[ i ], point2[ i ] );
    }
}

TYPED_TEST( TestCvlCorePoint, AssignmentOperatorTyped )
{
    constexpr int32_t dimension = 10;

    auto data = this->getRandomValues( dimension );

    const Point< TypeParam, dimension > point1(
        this->template getPoint< dimension >( data ) );

    Point< TypeParam, dimension > point2;

    point2 = point1;

    for ( size_t i = 0; i < static_cast< size_t >( dimension ); i++ )
    {
        ASSERT_EQ( point1.at( i ), point2.at( i ) );
    }
}

TYPED_TEST( TestCvlCorePoint, MoveOperatorTyped )
{
    constexpr int32_t dimension = 10;

    auto data = this->getRandomValues( dimension );

    const Point< TypeParam, dimension > point1(
        this->template getPoint< dimension >( data ) );

    Point< TypeParam, dimension > point2;

    Point< TypeParam, dimension > pointToMove( point1 );

    point2 = std::move( pointToMove );

    for ( size_t i = 0; i < static_cast< size_t >( dimension ); i++ )
    {
        ASSERT_EQ( point1[ i ], point2[ i ] );
    }
}

TYPED_TEST( TestCvlCorePoint, EqualOperatorTyped )
{
    constexpr int32_t dimension = 10;

    auto data = this->getRandomValues( dimension );

    const Point< TypeParam, dimension > point1(
        this->template getPoint< dimension >( data ) );

    const Point< TypeParam, dimension > point2(
        this->template getPoint< dimension >( data ) );

    ASSERT_EQ( point1, point2 );
}

TYPED_TEST( TestCvlCorePoint, NotEqualOperatorTyped )
{
    constexpr int32_t dimension = 10;

    auto data1 = this->getRandomValues( dimension );
    auto data2 = this->getRandomValues( dimension );

    const Point< TypeParam, dimension > point1(
        this->template getPoint< dimension >( data1 ) );

    const Point< TypeParam, dimension > point2(
        this->template getPoint< dimension >( data2 ) );

    ASSERT_NE( point1, point2 );
}

TYPED_TEST( TestCvlCorePoint, OperatorAddNewPointTyped )
{
    constexpr int32_t dimension = 10;

    auto data1 = this->getRandomValues( dimension );
    auto data2 = this->getRandomValues( dimension );

    const Point< TypeParam, dimension > point1(
        this->template getPoint< dimension >( data1 ) );

    const Point< TypeParam, dimension > point2(
        this->template getPoint< dimension >( data2 ) );

    const auto pointRes = point1 + point2;

    for ( size_t i = 0; i < static_cast< size_t >( dimension ); i++ )
    {
        ASSERT_EQ( pointRes.at( i ),
                   static_cast< TypeParam >( data1.at( i ) + data2.at( i ) ) );
    }
}

TEST( TestCvlCorePoint, OperatorAddNewPoint )
{
    constexpr int32_t x1 = 50;
    constexpr int32_t y1 = 100;

    constexpr double x2 = 50.5;
    constexpr double y2 = 100.25;

    const Point2i point1( x1, y1 );
    const Point2d point2( x2, y2 );

    const auto pointRes = point1 + point2;

    ASSERT_NEAR( pointRes.at( 0 ), static_cast< double >( x1 ) + x2, 0.01 );
    ASSERT_NEAR( pointRes.at( 1 ), static_cast< double >( y1 ) + y2, 0.01 );
}

TYPED_TEST( TestCvlCorePoint, OperatorAddSamePointTyped )
{
    constexpr int32_t dimension = 10;

    auto data1 = this->getRandomValues( dimension );
    auto data2 = this->getRandomValues( dimension );

    Point< TypeParam, dimension > point1(
        this->template getPoint< dimension >( data1 ) );

    const Point< TypeParam, dimension > point2(
        this->template getPoint< dimension >( data2 ) );

    point1 += point2;

    for ( size_t i = 0; i < static_cast< size_t >( dimension ); i++ )
    {
        ASSERT_EQ( point1.at( i ),
                   static_cast< TypeParam >( data1.at( i ) + data2.at( i ) ) );
    }
}

TYPED_TEST( TestCvlCorePoint, OperatorSubtractNewPointTyped )
{
    constexpr int32_t dimension = 10;

    auto data1 = this->getRandomValues( dimension );
    auto data2 = this->getRandomValues( dimension );

    const Point< TypeParam, dimension > point1(
        this->template getPoint< dimension >( data1 ) );

    const Point< TypeParam, dimension > point2(
        this->template getPoint< dimension >( data2 ) );

    const auto pointRes = point1 - point2;

    for ( size_t i = 0; i < static_cast< size_t >( dimension ); i++ )
    {
        ASSERT_EQ( pointRes.at( i ),
                   static_cast< TypeParam >( data1.at( i ) - data2.at( i ) ) );
    }
}

TEST( TestCvlCorePoint, OperatorSubtractNewPoint )
{
    constexpr int32_t x1 = 50;
    constexpr int32_t y1 = 100;

    constexpr double x2 = 50.5;
    constexpr double y2 = 100.25;

    const Point2i point1( x1, y1 );
    const Point2d point2( x2, y2 );

    const auto pointRes = point1 - point2;

    ASSERT_NEAR( pointRes.at( 0 ), static_cast< double >( x1 ) - x2, 0.01 );
    ASSERT_NEAR( pointRes.at( 1 ), static_cast< double >( y1 ) - y2, 0.01 );
}

TYPED_TEST( TestCvlCorePoint, OperatorSubtractSamePointTyped )
{
    constexpr int32_t dimension = 10;

    auto data1 = this->getRandomValues( dimension );
    auto data2 = this->getRandomValues( dimension );

    Point< TypeParam, dimension > point1(
        this->template getPoint< dimension >( data1 ) );

    const Point< TypeParam, dimension > point2(
        this->template getPoint< dimension >( data2 ) );

    point1 -= point2;

    for ( size_t i = 0; i < static_cast< size_t >( dimension ); i++ )
    {
        ASSERT_EQ( point1.at( i ),
                   static_cast< TypeParam >( data1.at( i ) - data2.at( i ) ) );
    }
}

TYPED_TEST( TestCvlCorePoint, OperatorScaleNewPointTyped )
{
    constexpr int32_t dimension = 10;

    auto data = this->getRandomValues( dimension );
    auto scale = this->getRandomValue( );

    const Point< TypeParam, dimension > point(
        this->template getPoint< dimension >( data ) );

    const auto scaledPoint = point * scale;

    for ( size_t i = 0; i < static_cast< size_t >( dimension ); i++ )
    {
        ASSERT_EQ( scaledPoint.at( i ),
                   static_cast< TypeParam >( data.at( i ) * scale ) );
    }
}

TYPED_TEST( TestCvlCorePoint, OperatorScaleSameTyped )
{
    constexpr int32_t dimension = 10;

    auto data = this->getRandomValues( dimension );
    auto scale = this->getRandomValue( );

    Point< TypeParam, dimension > point(
        this->template getPoint< dimension >( data ) );

    point *= scale;

    for ( size_t i = 0; i < static_cast< size_t >( dimension ); i++ )
    {
        ASSERT_EQ( point.at( i ),
                   static_cast< TypeParam >( data.at( i ) * scale ) );
    }
}

TEST( TestCvlCorePoint, OperatorScaleNewPoint )
{
    constexpr int32_t x1 = 50;
    constexpr int32_t y1 = 100;

    constexpr double s = 0.25;

    const Point2i point1( x1, y1 );

    const auto pointRes = point1 * s;

    ASSERT_NEAR( pointRes.at( 0 ), static_cast< double >( x1 ) * s, 0.01 );
    ASSERT_NEAR( pointRes.at( 1 ), static_cast< double >( y1 ) * s, 0.01 );
}

TYPED_TEST( TestCvlCorePoint, AccessorXTyped )
{
    constexpr int32_t dimension = 1;

    auto data = this->getRandomValues( dimension );

    Point< TypeParam, dimension > point(
        this->template getPoint< dimension >( data ) );

    ASSERT_EQ( point.getX( ), data[ 0 ] );
}

TYPED_TEST( TestCvlCorePoint, AccessorYTyped )
{
    constexpr int32_t dimension = 2;

    auto data = this->getRandomValues( dimension );

    Point< TypeParam, dimension > point(
        this->template getPoint< dimension >( data ) );

    ASSERT_EQ( point.getX( ), data[ 0 ] );
    ASSERT_EQ( point.getY( ), data[ 1 ] );
}

TYPED_TEST( TestCvlCorePoint, AccessorZTyped )
{
    constexpr int32_t dimension = 3;

    auto data = this->getRandomValues( dimension );

    Point< TypeParam, dimension > point(
        this->template getPoint< dimension >( data ) );

    ASSERT_EQ( point.getX( ), data[ 0 ] );
    ASSERT_EQ( point.getY( ), data[ 1 ] );
    ASSERT_EQ( point.getZ( ), data[ 2 ] );
}

TYPED_TEST( TestCvlCorePoint, AccessorArrayTyped )
{
    constexpr int32_t dimension = 10;

    auto data = this->getRandomValues( dimension );

    Point< TypeParam, dimension > point(
        this->template getPoint< dimension >( data ) );

    std::array< TypeParam, dimension > arrayData { };
    std::copy_n( data.begin( ), dimension, arrayData.begin( ) );

    ASSERT_EQ( point.coordinates( ), arrayData );
}

TYPED_TEST( TestCvlCorePoint, AccessorArrayConstTyped )
{
    constexpr int32_t dimension = 10;

    auto data = this->getRandomValues( dimension );

    const Point< TypeParam, dimension > point(
        this->template getPoint< dimension >( data ) );

    std::array< TypeParam, dimension > arrayData { };
    std::copy_n( data.begin( ), dimension, arrayData.begin( ) );

    ASSERT_EQ( point.coordinates( ), arrayData );
}

TYPED_TEST( TestCvlCorePoint, IteratorBeginTyped )
{
    constexpr int32_t dimension = 10;

    auto data = this->getRandomValues( dimension );

    Point< TypeParam, dimension > point1(
        this->template getPoint< dimension >( data ) );

    ASSERT_EQ( *data.begin( ), *point1.begin( ) );
}

TYPED_TEST( TestCvlCorePoint, IteratorConstBeginTyped )
{
    constexpr int32_t dimension = 10;

    auto data = this->getRandomValues( dimension );

    const Point< TypeParam, dimension > point1(
        this->template getPoint< dimension >( data ) );

    ASSERT_EQ( *data.begin( ), *point1.begin( ) );
}

TYPED_TEST( TestCvlCorePoint, IteratorEndTyped )
{
    constexpr int32_t dimension = 10;

    auto data = this->getRandomValues( dimension );

    Point< TypeParam, dimension > point1(
        this->template getPoint< dimension >( data ) );

    ASSERT_EQ( *( data.end( ) - 1 ), *( point1.end( ) - 1 ) );
}

TYPED_TEST( TestCvlCorePoint, IteratorConstEndTyped )
{
    constexpr int32_t dimension = 10;

    auto data = this->getRandomValues( dimension );

    const Point< TypeParam, dimension > point1(
        this->template getPoint< dimension >( data ) );

    ASSERT_EQ( *( data.end( ) - 1 ), *( point1.end( ) - 1 ) );
}

TYPED_TEST( TestCvlCorePoint, OutputOperator1DTyped )
{
    if constexpr ( ! std::is_same< TypeParam, uint8_t >( ) &&
                   ! std::is_same< TypeParam, int8_t >( ) )
    {
        Point< TypeParam, 1 > point( TypeParam { 1 } );
        std::stringstream ss;
        ss << point;
        ASSERT_EQ( ss.str( ), "Point (1)" );
    }
    else
    {
        // uint8_t and int8_t will be interpreted as characters.
        ASSERT_EQ( true, true );
    }
}

TYPED_TEST( TestCvlCorePoint, OutputOperator2DTyped )
{
    if constexpr ( ! std::is_same< TypeParam, uint8_t >( ) &&
                   ! std::is_same< TypeParam, int8_t >( ) )
    {
        Point< TypeParam, 2 > point( TypeParam { 1 }, TypeParam { 2 } );
        std::stringstream ss;
        ss << point;
        ASSERT_EQ( ss.str( ), "Point (x: 1, y: 2)" );
    }
    else
    {
        // uint8_t and int8_t will be interpreted as characters.
        ASSERT_EQ( true, true );
    }
}

TYPED_TEST( TestCvlCorePoint, OutputOperator3DTyped )
{
    if constexpr ( ! std::is_same< TypeParam, uint8_t >( ) &&
                   ! std::is_same< TypeParam, int8_t >( ) )
    {
        Point< TypeParam, 3 > point(
            TypeParam { 1 }, TypeParam { 2 }, TypeParam { 3 } );
        std::stringstream ss;
        ss << point;
        ASSERT_EQ( ss.str( ), "Point (x: 1, y: 2, z: 3)" );
    }
    else
    {
        // uint8_t and int8_t will be interpreted as characters.
        ASSERT_EQ( true, true );
    }
}

TYPED_TEST( TestCvlCorePoint, OutputOperator4DTyped )
{
    if constexpr ( ! std::is_same< TypeParam, uint8_t >( ) &&
                   ! std::is_same< TypeParam, int8_t >( ) )
    {
        Point< TypeParam, 4 > point( TypeParam { 1 },
                                     TypeParam { 2 },
                                     TypeParam { 3 },
                                     TypeParam { 4 } );
        std::stringstream ss;
        ss << point;
        ASSERT_EQ( ss.str( ), "Point (1, 2, 3, 4)" );
    }
    else
    {
        // uint8_t and int8_t will be interpreted as characters.
        ASSERT_EQ( true, true );
    }
}