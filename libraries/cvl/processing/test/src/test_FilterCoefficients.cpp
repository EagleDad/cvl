// OWN includes
#include <cvl/core/macros.h>
#include <cvl/processing/FilterCoefficients.h>

// GTest includes
IGNORE_WARNINGS_GTEST_PUSH
#include <gtest/gtest.h>
// #include <gmock/gmock.h> // GTest Update required because of removed function
//  in C++20
IGNORE_WARNINGS_POP

using namespace cvl::processing;
using namespace cvl::core;

TEST( TestCvlProcessingFilterCoefficients, Binomial_N0 )
{
    constexpr auto N = 0;

    EXPECT_EQ( binomialCoefficient( N, 0 ), 1 );
}

TEST( TestCvlProcessingFilterCoefficients, Binomial_N1 )
{
    constexpr auto N = 1;

    EXPECT_EQ( binomialCoefficient( N, 0 ), 1 );
    EXPECT_EQ( binomialCoefficient( N, 1 ), 1 );
}

TEST( TestCvlProcessingFilterCoefficients, Binomial_N2 )
{
    constexpr auto N = 2;

    EXPECT_EQ( binomialCoefficient( N, 0 ), 1 );
    EXPECT_EQ( binomialCoefficient( N, 1 ), 2 );
    EXPECT_EQ( binomialCoefficient( N, 2 ), 1 );
}

TEST( TestCvlProcessingFilterCoefficients, Binomial_N3 )
{
    constexpr auto N = 3;

    EXPECT_EQ( binomialCoefficient( N, 0 ), 1 );
    EXPECT_EQ( binomialCoefficient( N, 1 ), 3 );
    EXPECT_EQ( binomialCoefficient( N, 2 ), 3 );
    EXPECT_EQ( binomialCoefficient( N, 3 ), 1 );
}

TEST( TestCvlProcessingFilterCoefficients, Binomial_N4 )
{
    constexpr auto N = 4;

    EXPECT_EQ( binomialCoefficient( N, 0 ), 1 );
    EXPECT_EQ( binomialCoefficient( N, 1 ), 4 );
    EXPECT_EQ( binomialCoefficient( N, 2 ), 6 );
    EXPECT_EQ( binomialCoefficient( N, 3 ), 4 );
    EXPECT_EQ( binomialCoefficient( N, 4 ), 1 );
}

TEST( TestCvlProcessingFilterCoefficients, BinomialKernel3x3 )
{
    const std::vector< std::vector< int32_t > > compare {
        { 1, 2, 1 }, { 2, 4, 2 }, { 1, 2, 1 } };

    const auto binomialKernel = getBinomialKernel( { 3, 3 } );

    EXPECT_EQ( binomialKernel.size( ), compare.size( ) );

    for ( size_t y = 0; y < compare.size( ); y++ )
    {
        for ( size_t x = 0; x < compare[ y ].size( ); x++ )
        {
            EXPECT_EQ( binomialKernel[ y ][ x ], compare[ y ][ x ] );
        }
    }
}

TEST( TestCvlProcessingFilterCoefficients, BinomialKernel5x5 )
{
    const std::vector< std::vector< int32_t > > compare { { 1, 4, 6, 4, 1 },
                                                          { 4, 16, 24, 16, 4 },
                                                          { 6, 24, 36, 24, 6 },
                                                          { 4, 16, 24, 16, 4 },
                                                          { 1, 4, 6, 4, 1 } };

    const auto binomialKernel = getBinomialKernel( { 5, 5 } );

    EXPECT_EQ( binomialKernel.size( ), compare.size( ) );

    for ( size_t y = 0; y < compare.size( ); y++ )
    {
        for ( size_t x = 0; x < compare[ y ].size( ); x++ )
        {
            EXPECT_EQ( binomialKernel[ y ][ x ], compare[ y ][ x ] );
        }
    }
}

TEST( TestCvlProcessingFilterCoefficients, IsSeparableBinomialKernel3x3 )
{
    const std::vector< std::vector< int32_t > > compare {
        { 1, 2, 1 }, { 2, 4, 2 }, { 1, 2, 1 } };

    EXPECT_EQ( isSeparableFilter( compare ), true );
}

TEST( TestCvlProcessingFilterCoefficients, IsSeparableBinomialKernel5x5 )
{
    const std::vector< std::vector< int32_t > > compare { { 1, 4, 6, 4, 1 },
                                                          { 4, 16, 24, 16, 4 },
                                                          { 6, 24, 36, 24, 6 },
                                                          { 4, 16, 24, 16, 4 },
                                                          { 1, 4, 6, 4, 1 } };

    EXPECT_EQ( isSeparableFilter( compare ), true );
}

TEST( TestCvlProcessingFilterCoefficients, BoxKernel3x3 )
{
    const std::vector< std::vector< int32_t > > compare {
        { 1, 1, 1 }, { 1, 1, 1 }, { 1, 1, 1 } };

    const auto boxKernel = getBoxKernel( { 3, 3 } );

    EXPECT_EQ( boxKernel.size( ), compare.size( ) );

    for ( size_t y = 0; y < compare.size( ); y++ )
    {
        for ( size_t x = 0; x < compare[ y ].size( ); x++ )
        {
            EXPECT_EQ( boxKernel[ y ][ x ], compare[ y ][ x ] );
        }
    }
}

TEST( TestCvlProcessingFilterCoefficients, BoxKernel5x5 )
{
    const std::vector< std::vector< int32_t > > compare { { 1, 1, 1, 1, 1 },
                                                          { 1, 1, 1, 1, 1 },
                                                          { 1, 1, 1, 1, 1 },
                                                          { 1, 1, 1, 1, 1 },
                                                          { 1, 1, 1, 1, 1 } };

    const auto boxKernel = getBoxKernel( { 5, 5 } );

    EXPECT_EQ( boxKernel.size( ), compare.size( ) );

    for ( size_t y = 0; y < compare.size( ); y++ )
    {
        for ( size_t x = 0; x < compare[ y ].size( ); x++ )
        {
            EXPECT_EQ( boxKernel[ y ][ x ], compare[ y ][ x ] );
        }
    }
}

TEST( TestCvlProcessingFilterCoefficients, FirstDerivative1x3 )
{
    const std::vector< int32_t > compare { -1, 0, 1 };

    const auto derivativeKernel = getFirstDerivativeKernel( 3 );

    EXPECT_EQ( derivativeKernel.size( ), compare.size( ) );

    for ( size_t i = 0; i < compare.size( ); i++ )
    {
        EXPECT_EQ( derivativeKernel[ i ], compare[ i ] );
    }
}

TEST( TestCvlProcessingFilterCoefficients, FirstDerivative1x5 )
{
    const std::vector< int32_t > compare { -1, -2, 0, 2, 1 };

    const auto derivativeKernel = getFirstDerivativeKernel( 5 );

    EXPECT_EQ( derivativeKernel.size( ), compare.size( ) );

    for ( size_t i = 0; i < compare.size( ); i++ )
    {
        EXPECT_EQ( derivativeKernel[ i ], compare[ i ] );
    }
}

TEST( TestCvlProcessingFilterCoefficients, SobelKernelDx3x3 )
{
    const std::vector< std::vector< int32_t > > compare {
        { -1, 0, 1 }, { -2, 0, 2 }, { -1, 0, 1 } };

    const auto sobelKernel = getSobelKernel( 3, PixelDirection::dX );

    EXPECT_EQ( sobelKernel.size( ), compare.size( ) );

    for ( size_t y = 0; y < compare.size( ); y++ )
    {
        for ( size_t x = 0; x < compare[ y ].size( ); x++ )
        {
            EXPECT_EQ( sobelKernel[ y ][ x ], compare[ y ][ x ] );
        }
    }
}

TEST( TestCvlProcessingFilterCoefficients, SobelKernelDy3x3 )
{
    const std::vector< std::vector< int32_t > > compare {
        { -1, -2, -1 }, { 0, 0, 0 }, { 1, 2, 1 } };

    const auto sobelKernel = getSobelKernel( 3, PixelDirection::dY );

    EXPECT_EQ( sobelKernel.size( ), compare.size( ) );

    for ( size_t y = 0; y < compare.size( ); y++ )
    {
        for ( size_t x = 0; x < compare[ y ].size( ); x++ )
        {
            EXPECT_EQ( sobelKernel[ y ][ x ], compare[ y ][ x ] );
        }
    }
}

TEST( TestCvlProcessingFilterCoefficients, SobelKernelDx5x5 )
{
    const std::vector< std::vector< int32_t > > compare { { -1, -2, 0, 2, 1 },
                                                          { -4, -8, 0, 8, 4 },
                                                          { -6, -12, 0, 12, 6 },
                                                          { -4, -8, 0, 8, 4 },
                                                          { -1, -2, 0, 2, 1 } };

    const auto sobelKernel = getSobelKernel( 5, PixelDirection::dX );

    EXPECT_EQ( sobelKernel.size( ), compare.size( ) );

    for ( size_t y = 0; y < compare.size( ); y++ )
    {
        for ( size_t x = 0; x < compare[ y ].size( ); x++ )
        {
            EXPECT_EQ( sobelKernel[ y ][ x ], compare[ y ][ x ] );
        }
    }
}

TEST( TestCvlProcessingFilterCoefficients, SobelKernelDy5x5 )
{
    const std::vector< std::vector< int32_t > > compare {
        { -1, -4, -6, -4, -1 },
        { -2, -8, -12, -8, -2 },
        { 0, 0, 0, 0, 0 },
        { 2, 8, 12, 8, 2 },
        { 1, 4, 6, 4, 1 } };

    const auto sobelKernel = getSobelKernel( 5, PixelDirection::dY );

    EXPECT_EQ( sobelKernel.size( ), compare.size( ) );

    for ( size_t y = 0; y < compare.size( ); y++ )
    {
        for ( size_t x = 0; x < compare[ y ].size( ); x++ )
        {
            EXPECT_EQ( sobelKernel[ y ][ x ], compare[ y ][ x ] );
        }
    }
}

TEST( TestCvlProcessingFilterCoefficients, PrewittKernelDx3x3 )
{
    const std::vector< std::vector< int32_t > > compare {
        { -1, 0, 1 }, { -1, 0, 1 }, { -1, 0, 1 } };

    const auto prewittKernel = getPrewittKernel( 3, PixelDirection::dX );

    EXPECT_EQ( prewittKernel.size( ), compare.size( ) );

    for ( size_t y = 0; y < compare.size( ); y++ )
    {
        for ( size_t x = 0; x < compare[ y ].size( ); x++ )
        {
            EXPECT_EQ( prewittKernel[ y ][ x ], compare[ y ][ x ] );
        }
    }
}

TEST( TestCvlProcessingFilterCoefficients, PrewittKernelDy3x3 )
{
    const std::vector< std::vector< int32_t > > compare {
        { -1, -1, -1 }, { 0, 0, 0 }, { 1, 1, 1 } };

    const auto prewittKernel = getPrewittKernel( 3, PixelDirection::dY );

    EXPECT_EQ( prewittKernel.size( ), compare.size( ) );

    for ( size_t y = 0; y < compare.size( ); y++ )
    {
        for ( size_t x = 0; x < compare[ y ].size( ); x++ )
        {
            EXPECT_EQ( prewittKernel[ y ][ x ], compare[ y ][ x ] );
        }
    }
}

TEST( TestCvlProcessingFilterCoefficients, PrewittKernelDx5x5 )
{
    const std::vector< std::vector< int32_t > > compare { { -1, -2, 0, 2, 1 },
                                                          { -1, -2, 0, 2, 1 },
                                                          { -1, -2, 0, 2, 1 },
                                                          { -1, -2, 0, 2, 1 },
                                                          { -1, -2, 0, 2, 1 } };

    const auto prewittKernel = getPrewittKernel( 5, PixelDirection::dX );

    EXPECT_EQ( prewittKernel.size( ), compare.size( ) );

    for ( size_t y = 0; y < compare.size( ); y++ )
    {
        for ( size_t x = 0; x < compare[ y ].size( ); x++ )
        {
            EXPECT_EQ( prewittKernel[ y ][ x ], compare[ y ][ x ] );
        }
    }
}

TEST( TestCvlProcessingFilterCoefficients, PrewittKernelDy5x5 )
{
    const std::vector< std::vector< int32_t > > compare {
        { -1, -1, -1, -1, -1 },
        { -2, -2, -2, -2, -2 },
        { 0, 0, 0, 0, 0 },
        { 2, 2, 2, 2, 2 },
        { 1, 1, 1, 1, 1 } };

    const auto prewittKernel = getPrewittKernel( 5, PixelDirection::dY );

    EXPECT_EQ( prewittKernel.size( ), compare.size( ) );

    for ( size_t y = 0; y < compare.size( ); y++ )
    {
        for ( size_t x = 0; x < compare[ y ].size( ); x++ )
        {
            EXPECT_EQ( prewittKernel[ y ][ x ], compare[ y ][ x ] );
        }
    }
}