#include <cvl/core/macros.h>

IGNORE_WARNINGS_GTEST_PUSH
#include <gtest/gtest.h>
//#include <gmock/gmock.h> // GTest Update required because of removed function
IGNORE_WARNINGS_POP

#include <cvl/core/Error.h>

using namespace cvl::core;

TEST( TestCvlCoreError, ValueConstruct )
{
    const char* FILENAME = "File";
    const unsigned int LINE = 100;
    const std::string MESSAGE = "Message";

    const Error errorA( FILENAME, LINE );
    ASSERT_EQ( FILENAME, errorA.getFileName( ) );
    ASSERT_EQ( LINE, errorA.getLine( ) );

    const Error errorB( FILENAME, LINE, MESSAGE );
    ASSERT_EQ( FILENAME, errorB.getFileName( ) );
    ASSERT_EQ( LINE, errorB.getLine( ) );
    ASSERT_EQ( MESSAGE, errorB.getMessage( ) );
}

TEST( TestCvlCoreError, OutputStream )
{
    const Error error = Error( "File", 1, "A" );

    std::stringstream ss;
    ss << error;

    std::string PREFIX = "File:1:A\n";

    ASSERT_EQ( ss.str( ), PREFIX );
}

TEST( TestCvlCoreError, What )
{
    const Error error = Error( "File", 1, "A" );

    const std::string PREFIX = "File:1:A\n";

    const std::string got = error.what( );

    ASSERT_EQ( got, PREFIX );
}