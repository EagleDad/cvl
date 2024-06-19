// OWN includes
#include <Processing.h>
#include <cvl/core/macros.h>

// GTest includes
IGNORE_WARNINGS_GTEST_PUSH
#include <gtest/gtest.h>
// #include <gmock/gmock.h> // GTest Update required because of removed function
//  in C++20
IGNORE_WARNINGS_POP

//
// Typed tests
// https://google.github.io/googletest/advanced.html#typed-tests

using namespace cvl::core;
using namespace cvl::processing;
using testing::Eq;

template < typename T >
class TestCvlProcessingSmoothing : public testing::Test
{
public:
    using List = std::list< T >;
    static T shared_;
    T value_ { };

    CVL_DEFAULT_ONLY( TestCvlProcessingSmoothing );
};

using Types = testing::Types< uint8_t /*, uint16_t, int32_t, float*/ >;

TYPED_TEST_SUITE(
    TestCvlProcessingSmoothing,
    Types ); // NOLINT(clang-diagnostic-gnu-zero-variadic-macro-arguments)

TYPED_TEST( TestCvlProcessingSmoothing, BoxBlur )
{
    const SizeI kernelSize( 3, 3 );

    Image< TypeParam, 1 > imageSrc( 16, 16, true );
    Image< TypeParam, 1 > imageDst( 16, 16, true );

    imageSrc.at( 0, 0 ) = TypeParam { 1 };
    imageSrc.at( 0, 1 ) = TypeParam { 2 };
    imageSrc.at( 0, 2 ) = TypeParam { 3 };

    imageSrc.at( 1, 0 ) = TypeParam { 4 };
    imageSrc.at( 1, 1 ) = TypeParam { 5 };
    imageSrc.at( 1, 2 ) = TypeParam { 6 };

    imageSrc.at( 2, 0 ) = TypeParam { 7 };
    imageSrc.at( 2, 1 ) = TypeParam { 8 };
    imageSrc.at( 2, 2 ) = TypeParam { 9 };

    boxBlur( imageSrc, imageDst, kernelSize );

    const auto kernel = getBoxKernel( kernelSize );
    int32_t divisor { };
    decltype( std::declval< TypeParam >( ) +
              std::declval< int32_t >( ) ) sum { };

    for ( size_t y = 0; y < kernel.size( ); y++ )
    {
        for ( size_t x = 0; x < kernel[ y ].size( ); x++ )
        {
            divisor += kernel[ y ][ x ];

            sum += imageSrc.at( static_cast< int32_t >( y ),
                                static_cast< int32_t >( x ) ) *
                   kernel[ y ][ x ];
        }
    }

    const auto result = imageDst.at( 1, 1 );
    const auto compare = sum /= divisor;

    EXPECT_EQ( result, compare );
}

TYPED_TEST( TestCvlProcessingSmoothing, BinomialBlur )
{
    const SizeI kernelSize( 3, 3 );

    Image< TypeParam, 1 > imageSrc( 16, 16, true );
    Image< TypeParam, 1 > imageDst( 16, 16, true );

    imageSrc.at( 0, 0 ) = TypeParam { 1 };
    imageSrc.at( 0, 1 ) = TypeParam { 2 };
    imageSrc.at( 0, 2 ) = TypeParam { 3 };

    imageSrc.at( 1, 0 ) = TypeParam { 4 };
    imageSrc.at( 1, 1 ) = TypeParam { 5 };
    imageSrc.at( 1, 2 ) = TypeParam { 6 };

    imageSrc.at( 2, 0 ) = TypeParam { 7 };
    imageSrc.at( 2, 1 ) = TypeParam { 8 };
    imageSrc.at( 2, 2 ) = TypeParam { 9 };

    binomialBlur( imageSrc, imageDst, kernelSize );

    const auto kernel = getBinomialKernel( kernelSize );
    int32_t divisor { };
    decltype( std::declval< TypeParam >( ) +
              std::declval< int32_t >( ) ) sum { };

    for ( size_t y = 0; y < kernel.size( ); y++ )
    {
        for ( size_t x = 0; x < kernel[ y ].size( ); x++ )
        {
            divisor += kernel[ y ][ x ];

            sum += imageSrc.at( static_cast< int32_t >( y ),
                                static_cast< int32_t >( x ) ) *
                   kernel[ y ][ x ];
        }
    }

    const auto result = imageDst.at( 1, 1 );
    const auto compare = sum /= divisor;

    EXPECT_EQ( result, compare );
}

TYPED_TEST( TestCvlProcessingSmoothing, BinomialBlurThrow )
{
    const SizeI kernelSize( 3, 3 );

    Image< TypeParam, 1 > imageSrc( 16, 16, true );

    EXPECT_THROW( binomialBlur( imageSrc, imageSrc, kernelSize ), Error );
}

TYPED_TEST( TestCvlProcessingSmoothing, BinomialBlurMultipleKernels16x16 )
{
    const std::vector< SizeI > kernels {
        { 3, 3 }, { 5, 5 }, { 7, 7 } /*, { 9, 9 }*/ };

    /*const std::vector< SizeI > kernels {
        { 9, 9 } };*/

    for ( const auto& kernelSize : kernels )
    {
        constexpr int32_t height = 16;
        constexpr int16_t width = 16;

        const Image< TypeParam, 1 > imageSrc( width, height, true );
        Image< TypeParam, 1 > imageDst( width, height, true );
        Image< TypeParam, 1 > imageCmp( width, height, true );

        TypeParam val { };

        for ( int32_t y = 0; y < imageSrc.getHeight( ); y++ )
        {
            const auto srcPtr = imageSrc.getRowPointer( y );
            for ( int32_t x = 0; x < imageSrc.getWidth( ); x++ )
            {
                srcPtr[ x ] = val++;
            }
        }

        binomialBlur( imageSrc, imageDst, kernelSize );

        const auto kernel = getBinomialKernel( kernelSize );
        int32_t divisor { };

        using sumType = decltype( std::declval< TypeParam >( ) +
                                  std::declval< TypeParam >( ) );
        sumType sum { };

        for ( size_t y = 0; y < kernel.size( ); y++ )
        {
            for ( size_t x = 0; x < kernel[ y ].size( ); x++ )
            {
                divisor += kernel[ y ][ x ];
            }
        }

        const int32_t anchorY = kernelSize.getHeight( ) / 2;
        const int32_t anchorX = kernelSize.getWidth( ) / 2;

        for ( int32_t y = 0; y < imageSrc.getHeight( ); y++ )
        {
            for ( int32_t x = 0; x < imageSrc.getWidth( ); x++ )
            {
                sum = { };

                for ( int32_t ky = -anchorY; ky <= anchorY; ky++ )
                {
                    for ( int32_t kx = -anchorX; kx <= anchorX; kx++ )
                    {
                        int32_t px = x + kx;
                        int32_t py = y + ky;

                        if ( px < 0 )
                        {
                            px = std::abs( px );
                        }

                        if ( px >= width )
                        {
                            const auto offset = px - ( width - 1 );
                            px = width - 1 - offset;
                        }

                        if ( py < 0 )
                        {
                            py = std::abs( py );
                        }

                        if ( py >= height )
                        {
                            const auto offset = py - ( height - 1 );
                            py = height - 1 - offset;
                        }

                        const auto ix = static_cast< size_t >( kx ) + anchorX;
                        const auto iy = static_cast< size_t >( ky ) + anchorY;

                        const auto coefficient = kernel[ iy ][ ix ];
                        const auto value = imageSrc.at( py, px, 0 );

                        sum += value * static_cast< sumType >( coefficient );
                    }
                }

                sum /= static_cast< sumType >( divisor );
                imageCmp.at( y, x, 0 ) = static_cast< TypeParam >( sum );
            }
        }

        for ( int32_t y = 0; y < imageDst.getHeight( ); y++ )
        {
            for ( int32_t x = 0; x < imageDst.getWidth( ); x++ )
            {
                EXPECT_NEAR( imageDst.at( y, x ), imageCmp.at( y, x ), 1 );
            }
        }

        // EXPECT_EQ( imageDst, imageCmp );
    }
}

TYPED_TEST( TestCvlProcessingSmoothing, BinomialBlurMultipleKernels18x18 )
{
    const std::vector< SizeI > kernels { { 3, 3 } };

    for ( const auto& kernelSize : kernels )
    {
        constexpr int32_t height = 18;
        constexpr int16_t width = 18;

        const Image< TypeParam, 1 > imageSrc( width, height, true );
        Image< TypeParam, 1 > imageDst( width, height, true );
        Image< TypeParam, 1 > imageCmp( width, height, true );

        TypeParam val { };

        for ( int32_t y = 0; y < imageSrc.getHeight( ); y++ )
        {
            const auto srcPtr = imageSrc.getRowPointer( y );
            for ( int32_t x = 0; x < imageSrc.getWidth( ); x++ )
            {
                srcPtr[ x ] = val++;
            }
        }

        binomialBlur( imageSrc, imageDst, kernelSize );

        const auto kernel = getBinomialKernel( kernelSize );
        int32_t divisor { };

        using sumType = decltype( std::declval< TypeParam >( ) +
                                  std::declval< TypeParam >( ) );
        sumType sum { };

        for ( size_t y = 0; y < kernel.size( ); y++ )
        {
            for ( size_t x = 0; x < kernel[ y ].size( ); x++ )
            {
                divisor += kernel[ y ][ x ];
            }
        }

        const int32_t anchorY = kernelSize.getHeight( ) / 2;
        const int32_t anchorX = kernelSize.getWidth( ) / 2;

        for ( int32_t y = 0; y < imageSrc.getHeight( ); y++ )
        {
            for ( int32_t x = 0; x < imageSrc.getWidth( ); x++ )
            {
                sum = { };

                for ( int32_t ky = -anchorY; ky <= anchorY; ky++ )
                {
                    for ( int32_t kx = -anchorX; kx <= anchorX; kx++ )
                    {
                        int32_t px = x + kx;
                        int32_t py = y + ky;

                        if ( px < 0 )
                        {
                            px = std::abs( px );
                        }

                        if ( px >= width )
                        {
                            const auto offset = px - ( width - 1 );
                            px = width - 1 - offset;
                        }

                        if ( py < 0 )
                        {
                            py = std::abs( py );
                        }

                        if ( py >= height )
                        {
                            const auto offset = py - ( height - 1 );
                            py = height - 1 - offset;
                        }

                        const auto ix = static_cast< size_t >( kx ) + anchorX;
                        const auto iy = static_cast< size_t >( ky ) + anchorY;

                        const auto coefficient = kernel[ iy ][ ix ];
                        const auto value = imageSrc.at( py, px, 0 );

                        sum += value * static_cast< sumType >( coefficient );
                    }
                }

                sum /= static_cast< sumType >( divisor );
                imageCmp.at( y, x, 0 ) = static_cast< TypeParam >( sum );
            }
        }

        for ( int32_t y = 0; y < imageDst.getHeight( ); y++ )
        {
            for ( int32_t x = 0; x < imageDst.getWidth( ); x++ )
            {
                EXPECT_NEAR( imageDst.at( y, x ), imageCmp.at( y, x ), 1 );
            }
        }
    }
}

TYPED_TEST( TestCvlProcessingSmoothing, BinomialBlurMultipleKernels256x256 )
{
    const std::vector< SizeI > kernels {
        { 3, 3 }, { 5, 5 }, { 7, 7 } /*, { 9, 9 }*/ };

    /*const std::vector< SizeI > kernels {
        { 9, 9 } };*/

    for ( const auto& kernelSize : kernels )
    {
        constexpr int32_t height = 256;
        constexpr int16_t width = 256;

        const Image< TypeParam, 1 > imageSrc( width, height, true );
        Image< TypeParam, 1 > imageDst( width, height, true );
        Image< TypeParam, 1 > imageCmp( width, height, true );

        for ( int32_t y = 0; y < imageSrc.getHeight( ); y++ )
        {
            const auto srcPtr = imageSrc.getRowPointer( y );
            TypeParam val { };

            for ( int32_t x = 0; x < imageSrc.getWidth( ); x++ )
            {
                srcPtr[ x ] = val++;
            }
        }

        binomialBlur( imageSrc, imageDst, kernelSize );

        const auto kernel = getBinomialKernel( kernelSize );
        int32_t divisor { };

        using sumType = decltype( std::declval< TypeParam >( ) +
                                  std::declval< TypeParam >( ) );
        sumType sum { };

        for ( size_t y = 0; y < kernel.size( ); y++ )
        {
            for ( size_t x = 0; x < kernel[ y ].size( ); x++ )
            {
                divisor += kernel[ y ][ x ];
            }
        }

        const int32_t anchorY = kernelSize.getHeight( ) / 2;
        const int32_t anchorX = kernelSize.getWidth( ) / 2;

        for ( int32_t y = 0; y < imageSrc.getHeight( ); y++ )
        {
            for ( int32_t x = 0; x < imageSrc.getWidth( ); x++ )
            {
                sum = { };

                for ( int32_t ky = -anchorY; ky <= anchorY; ky++ )
                {
                    for ( int32_t kx = -anchorX; kx <= anchorX; kx++ )
                    {
                        int32_t px = x + kx;
                        int32_t py = y + ky;

                        if ( px < 0 )
                        {
                            px = std::abs( px );
                        }

                        if ( px >= width )
                        {
                            const auto offset = px - ( width - 1 );
                            px = width - 1 - offset;
                        }

                        if ( py < 0 )
                        {
                            py = std::abs( py );
                        }

                        if ( py >= height )
                        {
                            const auto offset = py - ( height - 1 );
                            py = height - 1 - offset;
                        }

                        const auto ix = static_cast< size_t >( kx ) + anchorX;
                        const auto iy = static_cast< size_t >( ky ) + anchorY;

                        const auto coefficient = kernel[ iy ][ ix ];
                        const auto value = imageSrc.at( py, px, 0 );

                        sum += value * static_cast< sumType >( coefficient );
                    }
                }

                sum /= static_cast< sumType >( divisor );
                imageCmp.at( y, x, 0 ) = static_cast< TypeParam >( sum );
            }
        }

        for ( int32_t y = 0; y < imageDst.getHeight( ); y++ )
        {
            for ( int32_t x = 0; x < imageDst.getWidth( ); x++ )
            {
                EXPECT_NEAR( imageDst.at( y, x ), imageCmp.at( y, x ), 1 );
            }
        }
    }
}