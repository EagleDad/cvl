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
#include <cvl/core/Rectangle.h>

//
// Typed tests
// https://google.github.io/googletest/advanced.html#typed-tests

using namespace cvl::core;
using testing::Eq;

template < typename T >
class TestCvlCoreRectangle : public testing::Test
{
public:
    using List = std::list< T >;
    static T shared_;
    T value_ { };

    TestCvlCoreRectangle( ) = default;

    ~TestCvlCoreRectangle( ) override = default;

    TestCvlCoreRectangle( const TestCvlCoreRectangle& other ) = delete;

    TestCvlCoreRectangle( const TestCvlCoreRectangle&& other ) = delete;

    TestCvlCoreRectangle&
    operator=( const TestCvlCoreRectangle& other ) = delete;

    TestCvlCoreRectangle& operator=( TestCvlCoreRectangle&& other ) = delete;

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
    TestCvlCoreRectangle,
    Types ); // NOLINT(clang-diagnostic-gnu-zero-variadic-macro-arguments)

TYPED_TEST( TestCvlCoreRectangle, DefaultConstructTyped )
{
    Rectangle< TypeParam > rect;

    Point< TypeParam, 2 > point;
    Size< TypeParam > size;

    ASSERT_EQ( rect.getSize( ), size );
    ASSERT_EQ( rect.getTopLeft( ), point );
    ASSERT_EQ( rect.getTopRight( ), point );
    ASSERT_EQ( rect.getBottomRight( ), point );
    ASSERT_EQ( rect.getBottomLeft( ), point );

    ASSERT_EQ( rect.getLeft( ), TypeParam { } );
    ASSERT_EQ( rect.getRight( ), TypeParam { } );
    ASSERT_EQ( rect.getTop( ), TypeParam { } );
    ASSERT_EQ( rect.getBottom( ), TypeParam { } );
    ASSERT_EQ( rect.getWidth( ), TypeParam { } );
    ASSERT_EQ( rect.getHeight( ), TypeParam { } );
}

TYPED_TEST( TestCvlCoreRectangle, ValueConstructTyped )
{
    if constexpr ( ! std::is_same< TypeParam, uint8_t >( ) &&
                   ! std::is_same< TypeParam, int8_t >( ) )
    {
        const auto x = this->getRandomValue( );
        const auto y = this->getRandomValue( );
        const auto w = this->getRandomValue( );
        const auto h = this->getRandomValue( );

        Point< TypeParam, 2 > tl( x, y );
        Size< TypeParam > size( w, h );

        Point< TypeParam, 2 > tr( static_cast< TypeParam >( x + w - 1 ), y );
        Point< TypeParam, 2 > br( static_cast< TypeParam >( x + w - 1 ),
                                  static_cast< TypeParam >( y + h - 1 ) );
        Point< TypeParam, 2 > bl( x, static_cast< TypeParam >( y + h - 1 ) );

        Rectangle< TypeParam > rect( tl, size );

        ASSERT_EQ( rect.getSize( ), size );
        ASSERT_EQ( rect.getTopLeft( ), tl );
        ASSERT_EQ( rect.getTopRight( ), tr );
        ASSERT_EQ( rect.getBottomRight( ), br );
        ASSERT_EQ( rect.getBottomLeft( ), bl );
        ASSERT_EQ( rect.getLeft( ), tl.getX( ) );
        ASSERT_EQ( rect.getRight( ), tr.getX( ) );
        ASSERT_EQ( rect.getTop( ), tl.getY( ) );
        ASSERT_EQ( rect.getBottom( ), bl.getY( ) );
        ASSERT_EQ( rect.getWidth( ), size.getWidth( ) );
        ASSERT_EQ( rect.getHeight( ), size.getHeight( ) );
    }
    else
    {
        // uint8_t and int8_t will be interpreted as characters.
        ASSERT_EQ( true, true );
    }
}

TYPED_TEST( TestCvlCoreRectangle, CopyConstructTyped )
{
    const auto x = this->getRandomValue( );
    const auto y = this->getRandomValue( );
    const auto w = this->getRandomValue( );
    const auto h = this->getRandomValue( );

    Point< TypeParam, 2 > tl( x, y );
    Size< TypeParam > size( w, h );

    Point< TypeParam, 2 > tr( static_cast< TypeParam >( x + w - 1 ), y );
    Point< TypeParam, 2 > br( static_cast< TypeParam >( x + w - 1 ),
                              static_cast< TypeParam >( y + h - 1 ) );
    Point< TypeParam, 2 > bl( x, static_cast< TypeParam >( y + h - 1 ) );

    Rectangle< TypeParam > rect1( tl, size );
    const Rectangle< TypeParam > rect2( rect1 );

    ASSERT_EQ( rect1.getSize( ), rect2.getSize( ) );
    ASSERT_EQ( rect1.getTopLeft( ), rect2.getTopLeft( ) );
    ASSERT_EQ( rect1.getTopRight( ), rect2.getTopRight( ) );
    ASSERT_EQ( rect1.getBottomRight( ), rect2.getBottomRight( ) );
    ASSERT_EQ( rect1.getBottomLeft( ), rect2.getBottomLeft( ) );
    ASSERT_EQ( rect1.getLeft( ), rect2.getLeft( ) );
    ASSERT_EQ( rect1.getRight( ), rect2.getRight( ) );
    ASSERT_EQ( rect1.getTop( ), rect2.getTop( ) );
    ASSERT_EQ( rect1.getBottom( ), rect2.getBottom( ) );
    ASSERT_EQ( rect1.getWidth( ), rect2.getWidth( ) );
    ASSERT_EQ( rect1.getHeight( ), rect2.getHeight( ) );
}

TYPED_TEST( TestCvlCoreRectangle, MoveConstructTyped )
{
    const auto x = this->getRandomValue( );
    const auto y = this->getRandomValue( );
    const auto w = this->getRandomValue( );
    const auto h = this->getRandomValue( );

    Point< TypeParam, 2 > tl( x, y );
    Size< TypeParam > size( w, h );

    Point< TypeParam, 2 > tr( static_cast< TypeParam >( x + w - 1 ), y );
    Point< TypeParam, 2 > br( static_cast< TypeParam >( x + w - 1 ),
                              static_cast< TypeParam >( y + h - 1 ) );
    Point< TypeParam, 2 > bl( x, static_cast< TypeParam >( y + h - 1 ) );

    Rectangle< TypeParam > rect1( tl, size );
    Rectangle< TypeParam > rect2( rect1 );

    Rectangle< TypeParam > rectMoved( std::move( rect2 ) );

    ASSERT_EQ( rect1.getSize( ), rectMoved.getSize( ) );
    ASSERT_EQ( rect1.getTopLeft( ), rectMoved.getTopLeft( ) );
    ASSERT_EQ( rect1.getTopRight( ), rectMoved.getTopRight( ) );
    ASSERT_EQ( rect1.getBottomRight( ), rectMoved.getBottomRight( ) );
    ASSERT_EQ( rect1.getBottomLeft( ), rectMoved.getBottomLeft( ) );
    ASSERT_EQ( rect1.getLeft( ), rectMoved.getLeft( ) );
    ASSERT_EQ( rect1.getRight( ), rectMoved.getRight( ) );
    ASSERT_EQ( rect1.getTop( ), rectMoved.getTop( ) );
    ASSERT_EQ( rect1.getBottom( ), rectMoved.getBottom( ) );
    ASSERT_EQ( rect1.getWidth( ), rectMoved.getWidth( ) );
    ASSERT_EQ( rect1.getHeight( ), rectMoved.getHeight( ) );
}

TYPED_TEST( TestCvlCoreRectangle, AssignmentOperatorTyped )
{
    const auto x = this->getRandomValue( );
    const auto y = this->getRandomValue( );
    const auto w = this->getRandomValue( );
    const auto h = this->getRandomValue( );

    Point< TypeParam, 2 > tl( x, y );
    Size< TypeParam > size( w, h );

    Point< TypeParam, 2 > tr( static_cast< TypeParam >( x + w - 1 ), y );
    Point< TypeParam, 2 > br( static_cast< TypeParam >( x + w - 1 ),
                              static_cast< TypeParam >( y + h - 1 ) );
    Point< TypeParam, 2 > bl( x, static_cast< TypeParam >( y + h - 1 ) );

    const Rectangle< TypeParam > rect1( tl, size );
    Rectangle< TypeParam > rect2;

    rect2 = rect1;

    ASSERT_EQ( rect1.getSize( ), rect2.getSize( ) );
    ASSERT_EQ( rect1.getTopLeft( ), rect2.getTopLeft( ) );
    ASSERT_EQ( rect1.getTopRight( ), rect2.getTopRight( ) );
    ASSERT_EQ( rect1.getBottomRight( ), rect2.getBottomRight( ) );
    ASSERT_EQ( rect1.getBottomLeft( ), rect2.getBottomLeft( ) );
    ASSERT_EQ( rect1.getLeft( ), rect2.getLeft( ) );
    ASSERT_EQ( rect1.getRight( ), rect2.getRight( ) );
    ASSERT_EQ( rect1.getTop( ), rect2.getTop( ) );
    ASSERT_EQ( rect1.getBottom( ), rect2.getBottom( ) );
    ASSERT_EQ( rect1.getWidth( ), rect2.getWidth( ) );
    ASSERT_EQ( rect1.getHeight( ), rect2.getHeight( ) );
}

TYPED_TEST( TestCvlCoreRectangle, MoveOperatorTyped )
{
    const auto x = this->getRandomValue( );
    const auto y = this->getRandomValue( );
    const auto w = this->getRandomValue( );
    const auto h = this->getRandomValue( );

    Point< TypeParam, 2 > tl( x, y );
    Size< TypeParam > size( w, h );

    Point< TypeParam, 2 > tr( static_cast< TypeParam >( x + w - 1 ), y );
    Point< TypeParam, 2 > br( static_cast< TypeParam >( x + w - 1 ),
                              static_cast< TypeParam >( y + h - 1 ) );
    Point< TypeParam, 2 > bl( x, static_cast< TypeParam >( y + h - 1 ) );

    const Rectangle< TypeParam > rect1( tl, size );
    Rectangle< TypeParam > rectToMove( rect1 );
    Rectangle< TypeParam > rect2;

    rect2 = std::move( rectToMove );

    ASSERT_EQ( rect1.getSize( ), rect2.getSize( ) );
    ASSERT_EQ( rect1.getTopLeft( ), rect2.getTopLeft( ) );
    ASSERT_EQ( rect1.getTopRight( ), rect2.getTopRight( ) );
    ASSERT_EQ( rect1.getBottomRight( ), rect2.getBottomRight( ) );
    ASSERT_EQ( rect1.getBottomLeft( ), rect2.getBottomLeft( ) );
    ASSERT_EQ( rect1.getLeft( ), rect2.getLeft( ) );
    ASSERT_EQ( rect1.getRight( ), rect2.getRight( ) );
    ASSERT_EQ( rect1.getTop( ), rect2.getTop( ) );
    ASSERT_EQ( rect1.getBottom( ), rect2.getBottom( ) );
    ASSERT_EQ( rect1.getWidth( ), rect2.getWidth( ) );
    ASSERT_EQ( rect1.getHeight( ), rect2.getHeight( ) );
}

TYPED_TEST( TestCvlCoreRectangle, EqualOperatorTyped )
{
    const auto x = this->getRandomValue( );
    const auto y = this->getRandomValue( );
    const auto w = this->getRandomValue( );
    const auto h = this->getRandomValue( );

    Point< TypeParam, 2 > tl( x, y );
    Size< TypeParam > size( w, h );

    const Rectangle< TypeParam > rect1( tl, size );
    const Rectangle< TypeParam > rect2( tl, size );

    ASSERT_EQ( rect1, rect2 );
}

TYPED_TEST( TestCvlCoreRectangle, NotEqualOperatorTyped )
{
    const auto x1 = this->getRandomValue( );
    const auto y1 = this->getRandomValue( );
    const auto w1 = this->getRandomValue( );
    const auto h1 = this->getRandomValue( );

    const auto x2 = this->getRandomValue( );
    const auto y2 = this->getRandomValue( );
    const auto w2 = this->getRandomValue( );
    const auto h2 = this->getRandomValue( );

    Point< TypeParam, 2 > tl1( x1, y1 );
    Size< TypeParam > size1( w1, h1 );

    Point< TypeParam, 2 > tl2( x2, y2 );
    Size< TypeParam > size2( w2, h2 );

    const Rectangle< TypeParam > rect1( tl1, size1 );
    const Rectangle< TypeParam > rect2( tl2, size2 );

    ASSERT_NE( rect1, rect2 );
}

//TYPED_TEST( TestCvlCoreRectangle, AddOperatorNewObjectTyped )
//{
//    const auto x1 = this->getRandomValue( );
//    const auto y1 = this->getRandomValue( );
//    const auto w1 = this->getRandomValue( );
//    const auto h1 = this->getRandomValue( );
//
//    const auto x2 = this->getRandomValue( );
//    const auto y2 = this->getRandomValue( );
//    const auto w2 = this->getRandomValue( );
//    const auto h2 = this->getRandomValue( );
//
//    Point< TypeParam, 2 > tl1( x1, y1 );
//    Size< TypeParam > size1( w1, h1 );
//
//    Point< TypeParam, 2 > tl2( x2, y2 );
//    Size< TypeParam > size2( w2, h2 );
//
//    const Rectangle< TypeParam > rect1( tl1, size1 );
//    const Rectangle< TypeParam > rect2( tl2, size2 );
//
//    const auto rectAdded = rect1 + rect2;
//
//    ASSERT_EQ( rectAdded.getWidth( ), static_cast< TypeParam >( w1 + w2 ) );
//    ASSERT_EQ( rectAdded.getHeight( ), static_cast< TypeParam >( h1 + h2 ) );
//    ASSERT_EQ( rectAdded.getTopLeft( ).getX( ),
//               static_cast< TypeParam >( x1 + x2 ) );
//    ASSERT_EQ( rectAdded.getTopLeft( ).getY( ),
//               static_cast< TypeParam >( y1 + y2 ) );
//}

//TEST( TestCvlCoreRectangle, OperatorAddNewRect )
//{
//    constexpr int32_t x1 = 50;
//    constexpr int32_t y1 = 100;
//
//    constexpr double x2 = 50.5;
//    constexpr double y2 = 100.25;
//
//    const Point2i tl1( x1, y1 );
//    const Point2d tl2( x2, y2 );
//
//    constexpr int32_t w1 = 300;
//    constexpr int32_t h1 = 600;
//
//    constexpr double w2 = 50.75;
//    constexpr double h2 = 100.25;
//
//    const Size< int32_t > size1( w1, h1 );
//    const Size< double > size2( w2, h2 );
//
//    const Rectangle rect1( tl1, size1 );
//    const Rectangle rect2( tl2, size2 );
//
//    const auto rectAdded = rect1 + rect2;
//
//    ASSERT_EQ( rectAdded.getWidth( ), static_cast< double >( w1 + w2 ) );
//    ASSERT_EQ( rectAdded.getHeight( ), static_cast< double >( h1 + h2 ) );
//    ASSERT_EQ( rectAdded.getTopLeft( ).getX( ),
//               static_cast< double >( x1 + x2 ) );
//    ASSERT_EQ( rectAdded.getTopLeft( ).getY( ),
//               static_cast< double >( y1 + y2 ) );
//}

//TYPED_TEST( TestCvlCoreRectangle, AddAssignOperatorTyped )
//{
//    const auto x1 = this->getRandomValue( );
//    const auto y1 = this->getRandomValue( );
//    const auto w1 = this->getRandomValue( );
//    const auto h1 = this->getRandomValue( );
//
//    const auto x2 = this->getRandomValue( );
//    const auto y2 = this->getRandomValue( );
//    const auto w2 = this->getRandomValue( );
//    const auto h2 = this->getRandomValue( );
//
//    Point< TypeParam, 2 > tl1( x1, y1 );
//    Size< TypeParam > size1( w1, h1 );
//
//    Point< TypeParam, 2 > tl2( x2, y2 );
//    Size< TypeParam > size2( w2, h2 );
//
//    Rectangle< TypeParam > rect1( tl1, size1 );
//    const Rectangle< TypeParam > rect2( tl2, size2 );
//
//    rect1 += rect2;
//
//    ASSERT_EQ( rect1.getWidth( ), static_cast< TypeParam >( w1 + w2 ) );
//    ASSERT_EQ( rect1.getHeight( ), static_cast< TypeParam >( h1 + h2 ) );
//    ASSERT_EQ( rect1.getTopLeft( ).getX( ),
//               static_cast< TypeParam >( x1 + x2 ) );
//    ASSERT_EQ( rect1.getTopLeft( ).getY( ),
//               static_cast< TypeParam >( y1 + y2 ) );
//}

//TYPED_TEST( TestCvlCoreRectangle, SubtractOperatorNewObjectTyped )
//{
//    const auto x1 = this->getRandomValue( );
//    const auto y1 = this->getRandomValue( );
//    const auto w1 = this->getRandomValue( );
//    const auto h1 = this->getRandomValue( );
//
//    const auto x2 = this->getRandomValue( );
//    const auto y2 = this->getRandomValue( );
//    const auto w2 = this->getRandomValue( );
//    const auto h2 = this->getRandomValue( );
//
//    Point< TypeParam, 2 > tl1( x1, y1 );
//    Size< TypeParam > size1( w1, h1 );
//
//    Point< TypeParam, 2 > tl2( x2, y2 );
//    Size< TypeParam > size2( w2, h2 );
//
//    const Rectangle< TypeParam > rect1( tl1, size1 );
//    const Rectangle< TypeParam > rect2( tl2, size2 );
//
//    const auto rectSub = rect1 - rect2;
//
//    ASSERT_EQ( rectSub.getWidth( ), static_cast< TypeParam >( w1 - w2 ) );
//    ASSERT_EQ( rectSub.getHeight( ), static_cast< TypeParam >( h1 - h2 ) );
//    ASSERT_EQ( rectSub.getTopLeft( ).getX( ),
//               static_cast< TypeParam >( x1 - x2 ) );
//    ASSERT_EQ( rectSub.getTopLeft( ).getY( ),
//               static_cast< TypeParam >( y1 - y2 ) );
//}

//TEST( TestCvlCoreRectangle, OperatorSubtractNewRect )
//{
//    constexpr int32_t x1 = 50;
//    constexpr int32_t y1 = 100;
//
//    constexpr double x2 = 50.5;
//    constexpr double y2 = 100.25;
//
//    const Point2i tl1( x1, y1 );
//    const Point2d tl2( x2, y2 );
//
//    constexpr int32_t w1 = 300;
//    constexpr int32_t h1 = 600;
//
//    constexpr double w2 = 50.75;
//    constexpr double h2 = 100.25;
//
//    const Size< int32_t > size1( w1, h1 );
//    const Size< double > size2( w2, h2 );
//
//    const Rectangle rect1( tl1, size1 );
//    const Rectangle rect2( tl2, size2 );
//
//    const auto rectSub = rect1 - rect2;
//
//    ASSERT_EQ( rectSub.getWidth( ), static_cast< double >( w1 - w2 ) );
//    ASSERT_EQ( rectSub.getHeight( ), static_cast< double >( h1 - h2 ) );
//    ASSERT_EQ( rectSub.getTopLeft( ).getX( ),
//               static_cast< double >( x1 - x2 ) );
//    ASSERT_EQ( rectSub.getTopLeft( ).getY( ),
//               static_cast< double >( y1 - y2 ) );
//}

//TYPED_TEST( TestCvlCoreRectangle, SubtractAssignOperatorTyped )
//{
//    const auto x1 = this->getRandomValue( );
//    const auto y1 = this->getRandomValue( );
//    const auto w1 = this->getRandomValue( );
//    const auto h1 = this->getRandomValue( );
//
//    const auto x2 = this->getRandomValue( );
//    const auto y2 = this->getRandomValue( );
//    const auto w2 = this->getRandomValue( );
//    const auto h2 = this->getRandomValue( );
//
//    Point< TypeParam, 2 > tl1( x1, y1 );
//    Size< TypeParam > size1( w1, h1 );
//
//    Point< TypeParam, 2 > tl2( x2, y2 );
//    Size< TypeParam > size2( w2, h2 );
//
//    Rectangle< TypeParam > rect1( tl1, size1 );
//    const Rectangle< TypeParam > rect2( tl2, size2 );
//
//    rect1 -= rect2;
//
//    ASSERT_EQ( rect1.getWidth( ), static_cast< TypeParam >( w1 - w2 ) );
//    ASSERT_EQ( rect1.getHeight( ), static_cast< TypeParam >( h1 - h2 ) );
//    ASSERT_EQ( rect1.getTopLeft( ).getX( ),
//               static_cast< TypeParam >( x1 - x2 ) );
//    ASSERT_EQ( rect1.getTopLeft( ).getY( ),
//               static_cast< TypeParam >( y1 - y2 ) );
//}

TYPED_TEST( TestCvlCoreRectangle, ScaleOperatorNewObjectTyped )
{
    const auto x = this->getRandomValue( );
    const auto y = this->getRandomValue( );
    const auto w = this->getRandomValue( );
    const auto h = this->getRandomValue( );
    const auto s = this->getRandomValue( );

    Point< TypeParam, 2 > tl( x, y );
    Size< TypeParam > size( w, h );

    Rectangle< TypeParam > rect( tl, size );

    const auto rectScaled = rect * s;

    ASSERT_EQ( rectScaled.getWidth( ), static_cast< TypeParam >( w * s ) );
    ASSERT_EQ( rectScaled.getHeight( ), static_cast< TypeParam >( h * s ) );
    ASSERT_EQ( rectScaled.getTopLeft( ).getX( ),
               static_cast< TypeParam >( x * s ) );
    ASSERT_EQ( rectScaled.getTopLeft( ).getY( ),
               static_cast< TypeParam >( y * s ) );
}

TEST( TestCvlCoreRectangle, ScaleOperatorNewObject )
{
    constexpr auto x = 100;
    constexpr auto y = 200;
    constexpr auto w = 50;
    constexpr auto h = 25;
    constexpr auto s = 0.25;

    const Point< int32_t, 2 > tl( x, y );
    const Size size( w, h );

    const Rectangle rect( tl, size );

    const auto rectScaled = rect * s;

    ASSERT_NEAR(
        rectScaled.getWidth( ), static_cast< double >( w * s ), 0.001 );
    ASSERT_NEAR(
        rectScaled.getHeight( ), static_cast< double >( h * s ), 0.001 );
    ASSERT_NEAR( rectScaled.getTopLeft( ).getX( ),
                 static_cast< double >( x * s ),
                 0.001 );
    ASSERT_NEAR( rectScaled.getTopLeft( ).getY( ),
                 static_cast< double >( y * s ),
                 0.001 );
}
