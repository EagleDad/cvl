#include <cvl/core/macros.h>

IGNORE_WARNINGS_GTEST_PUSH
#include <gtest/gtest.h>
// #include <gmock/gmock.h> // GTest Update required because of removed function
IGNORE_WARNINGS_POP

#include <cvl/core/Ellipse.h>

using namespace cvl::core;

TEST( TestCvlCoreEllipse, DefaultConstructNoThrow )
{
    EXPECT_NO_THROW( Ellipse ellipse; );
}

TEST( TestCvlCoreEllipse, DefaultConstruct )
{
    constexpr Ellipse ellipse;

    EXPECT_EQ( ellipse.getCenter( ), Point2d( 0.0, 0.0 ) );
    EXPECT_EQ( ellipse.getSize( ), SizeD( 0.0, 0.0 ) );
    EXPECT_EQ( ellipse.getAngle( ), 0.0 );
}

TEST( TestCvlCoreEllipse, ValueConstruct )
{
    const Ellipse ellipse( Point2d( 5.0, 10.0 ), { 50.0, 100.0 }, 0.25 );

    EXPECT_EQ( ellipse.getCenter( ), Point2d( 5.0, 10.0 ) );
    EXPECT_EQ( ellipse.getSize( ), SizeD( 50.0, 100.0 ) );
    EXPECT_EQ( ellipse.getAngle( ), 0.25 );
}