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
#include <cvl/core/Line.h>

using namespace cvl::core;
using testing::Eq;

TEST( TestCvlCoreLine, DefaultConstruct )
{
    constexpr Vector< 2 > loc;

    constexpr Line2d line2D;

    ASSERT_EQ( line2D.getLocation( ), loc );
    ASSERT_EQ( line2D.getDirection( ), loc );
}

TEST( TestCvlCoreLine, ValueConstructVectors )
{
    const Vector< 2 > loc( 5.2, 3.5 );
    const Vector< 2 > dir( 0.15, 0.85 );

    const Line2d line2D( loc, dir );

    ASSERT_EQ( line2D.getLocation( ), loc );
    ASSERT_EQ( line2D.getDirection( ), dir );
}

TEST( TestCvlCoreLine, ValueConstructPoints )
{
    const Point2i point1( 0, 0 );
    const Point2i point2( 1, 0 );

    const Line2d line2D( point1, point2 );

    ASSERT_EQ( line2D.getLocation( ), Vector< 2 >( 0.0, 0.0 ) );
    ASSERT_EQ( line2D.getDirection( ), Vector< 2 >( 1.0, 0.0 ) );
}

TEST( TestCvlCoreLine, CopyConstruct )
{
    const Point2i point1( 0, 0 );
    const Point2i point2( 1, 0 );

    const Line2d line2D( point1, point2 );

    const Line2d copiedLine( line2D );

    ASSERT_EQ( line2D, copiedLine );
}

TEST( TestCvlCoreLine, MoveConstruct )
{
    const Point2i point1( 0, 0 );
    const Point2i point2( 1, 0 );

    const Line2d line2D( point1, point2 );

    Line2d lineToMove( line2D );

    const Line2d movedLine( std::move( lineToMove ) );

    ASSERT_EQ( line2D, movedLine );
}

TEST( TestCvlCoreLine, AssignmentOperator )
{
    const Point2i point1( 0, 0 );
    const Point2i point2( 1, 0 );

    const Line2d line2D( point1, point2 );

    Line2d lineGetAssigned;

    lineGetAssigned = line2D;

    ASSERT_EQ( line2D, lineGetAssigned );
}

TEST( TestCvlCoreLine, MoveOperator )
{
    const Point2i point1( 0, 0 );
    const Point2i point2( 1, 0 );

    Line2d line2D( point1, point2 );

    Line2d lineToMove( line2D );

    Line2d lineGetAssigned;

    lineGetAssigned = std::move( line2D );

    ASSERT_EQ( line2D, lineGetAssigned );
}

TEST( TestCvlCoreLine, NotEqual )
{
    const Point2i point1( 0, 0 );
    const Point2i point2( 1, 0 );
    const Point2i point3( 1, 0 );

    Line2d line2DA( point1, point2 );
    Line2d line2DB( point2, point3 );

    ASSERT_NE( line2DA, line2DB );
}

TEST( TestCvlCoreLine, GetSlope )
{
    const Point2i point1( -1, -1 );
    const Point2i point2( 5, 5 );
    const Line2d line2D( point1, point2 );

    const auto dx = static_cast< double >( point2.getX( ) - point1.getX( ) );
    const auto dy = static_cast< double >( point2.getY( ) - point1.getY( ) );

    ASSERT_NEAR( line2D.getSlope( ), dy / dx, 0.001 );
}

TEST( TestCvlCoreLine, GetAxisOffset )
{
    const Point2i point1( -1, -1 );
    const Point2i point2( 5, 5 );
    const Line2d line2D( point1, point2 );

    // y = mx + b

    const auto dx = static_cast< double >( point2.getX( ) - point1.getX( ) );
    const auto dy = static_cast< double >( point2.getY( ) - point1.getY( ) );

    const auto m = dy / dx;

    // b = y - mx
    const auto b = static_cast< double >( point1.getY( ) ) -
                   m * static_cast< double >( point1.getX( ) );

    ASSERT_NEAR( line2D.getAxisOffset( ), b, 0.001 );
}

TEST( TestCvlCoreLine, GetNormal )
{
    const Point2i point1( 0, 0 );
    const Point2i point2( 0, 5 );
    const Line2d line2D( point1, point2 );

    const auto normal = line2D.getNormal( );

    ASSERT_NEAR( normal.at( 0 ), 5.0, 0.001 );
    ASSERT_NEAR( normal.at( 1 ), 0.0, 0.001 );
}

TEST( TestCvlCoreLine, CoordinateForm01 )
{
    const Point2i point1( 2, -1 );
    const Point2i point2( -1, 3 );
    const Line2d line2D( point1, point2 );

    ASSERT_NEAR( line2D.getParameterA( ), 4.0, 0.001 );
    ASSERT_NEAR( line2D.getParameterB( ), 3.0, 0.001 );
    ASSERT_NEAR( line2D.getParameterC( ), 5.0, 0.001 );
}

Vector< 3 > cross( const Vector< 3 >& a, const Vector< 3 >& b )
{
    Vector< 3 > result;

    result[ 0 ] = a[ 1 ] * b[ 2 ] - a[ 2 ] * b[ 1 ];

    result[ 1 ] = a[ 2 ] * b[ 0 ] - a[ 0 ] * b[ 2 ];

    result[ 2 ] = a[ 0 ] * b[ 1 ] - a[ 1 ] * b[ 0 ];

    return result;
}

template < int32_t Dimension >
double dot( const Vector< Dimension >& a, const Vector< Dimension >& b )
{
    return a * b;
}

TEST( TestCvlCoreLine, CoordinateForm02 )
{
    const Point2i point1x( 1, 3 );
    const Point2i point2x( 1, 1 );

    const Point2i point1y( 3, 1 );
    const Point2i point2y( 1, 1 );

    // 1; 1

    ///
    ///
    ///

    /*const Point2i point1x( 3, 1 );
    const Point2i point2x( 6, 4 );

    const Point2i point1y( 2, 4 );
    const Point2i point2y( 5, 1 );*/

    // 4; 2

    ///
    ///
    ///

    /*const Point2i point1x( 5, 10 );
    const Point2i point2x( 20, 25 );

    const Point2i point1y( 5, 20 );
    const Point2i point2y( 15, 10 );*/

    // 10; 15

    const Line2d line2DX( point1x, point2x );

    const Line2d line2DY( point1y, point2y );

    const auto signX = std::copysign( 1.0, line2DX.getParameterC( ) );
    const auto signY = std::copysign( 1.0, line2DY.getParameterC( ) );

    std::ignore = signX;
    std::ignore = signY;

    auto ax = line2DX.getParameterA( );
    auto ay = line2DX.getParameterB( );
    auto az = line2DX.getParameterC( ) * -1.0;

   /* ax *= signX;
    ay *= signX;
    az *= signX;*/

    auto bx = line2DY.getParameterA( );
    auto by = line2DY.getParameterB( );
    auto bz = line2DY.getParameterC( ) * -1.0;

   /* bx *= signY;
    by *= signY;
    bz *= signY;*/


    Vector< 3 > L1( ax,
                    ay,
                    az );

    Vector< 3 > L2( bx,
                    by,
                    bz );

    auto cr = cross( L1, L2 );

    auto d = dot( L1, cr );
    std::ignore = d;

    cr = cr * ( 1.0 / cr[ 2 ] );

    std::ignore = cr;
}

TEST( TestCvlCoreLine, ParameterABC )
{
    const Vector< 2 > location( 0.0, 1.0 );
    const Vector< 2 > direction( 2.0, 1.0 );

    const Line2d line2D( location, direction );

    ASSERT_EQ( line2D.getParameterA( ), 1.0 );
    ASSERT_EQ( line2D.getParameterB( ), -2.0 );
    ASSERT_EQ( line2D.getParameterC( ), -2.0 );
}