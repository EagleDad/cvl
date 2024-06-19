// OWN includes
#include <cvl/core/macros.h>
#include <cvl/processing/FilterCoefficients.h>

// STD includes
#include <vector>

namespace cvl::processing
{

template < Arithmetic T >
std::vector< std::vector< T > > convolution( const std::vector< T >& row,
                                             const std::vector< T >& col )
{
    std::vector< std::vector< T > > kernel;
    kernel.resize( row.size( ) );

    for ( auto& rowKernel : kernel )
    {
        rowKernel.resize( col.size( ) );
    }

    for ( size_t y = 0; y < row.size( ); y++ )
    {
        for ( size_t x = 0; x < col.size( ); x++ )
        {
            const auto coefficient = col[ x ] * row[ y ];

            kernel[ y ][ x ] = coefficient;
        }
    }

    return kernel;
}

template < Arithmetic T >
std::vector< T > convolution1D( const std::vector< T >& lhs,
                                const std::vector< T >& rhs )
{
    using sumType = decltype( std::declval< T >( ) + std::declval< T >( ) );

    sumType sum { };

    const int32_t lhsSize = static_cast< int32_t >( lhs.size( ) );
    const int32_t rhsSize = static_cast< int32_t >( rhs.size( ) );
    const int32_t newSize = lhsSize + rhsSize - 1;
    std::vector< T > kernel( static_cast< size_t >( newSize ) );
    auto offset = -rhsSize + 1;

    for ( auto& kernelElement : kernel )
    {
        sum = { };

        for ( int32_t i = 0; i < rhsSize; i++ )
        {
            const auto idx = offset + i;

            if ( idx >= 0 && idx < lhsSize )
            {
                sum += lhs[ static_cast< size_t >( idx ) ] *
                       rhs[ static_cast< size_t >( i ) ];
            }
        }

        ++offset;
        kernelElement = sum;
    }

    return kernel;
}

// TODO: Perform Benchmark between these approaches
double binomialCoefficient( size_t n, size_t k )
{
    // EXPECT_MSG( n < 68, "N is too high to calculate" );

    double ret { 1.0 };
    for ( size_t i = 0; i < k; i++ )
    {
        ret *= static_cast< double >( n - i );
        ret /= static_cast< double >( i + 1 );
    }

    return ret;
}

int binomialCoefficients( int n, int k )
{
    if ( k > n )
        return 0;
    if ( k == 0 || k == n )
        return 1;

    return binomialCoefficients( n - 1, k - 1 ) +
           binomialCoefficients( n - 1, k );
}

int32_t binomialCoefficient( int32_t n, int32_t k )
{
    const auto sizeN = static_cast< size_t >( n ) + 1;
    const auto sizeK = static_cast< size_t >( k ) + 1;

    std::vector< std::vector< int32_t > > C;
    C.resize( sizeN );

    for ( auto& cn : C )
    {
        cn.resize( sizeK );
    }

    // Calculate value of Binomial Coefficient
    // in bottom up manner
    for ( size_t i = 0; i <= static_cast< size_t >( n ); i++ )
    {
        for ( size_t j = 0; j <= static_cast< size_t >( std::min(
                                     i, static_cast< size_t >( k ) ) );
              j++ )
        {
            // Base Cases
            if ( j == 0 || j == i )
                C[ i ][ j ] = 1;

            // Calculate value using previously
            // stored values
            else
                C[ i ][ j ] = C[ i - 1 ][ j - 1 ] + C[ i - 1 ][ j ];
        }
    }

    return C[ static_cast< size_t >( n ) ][ static_cast< size_t >( k ) ];
}

std::vector< std::vector< int32_t > >
getBinomialKernel( const core::SizeI& kernelSize )
{
    EXPECT_MSG( kernelSize.getWidth( ) % 2 != 0,
                "Invalid kernel size("
                    << kernelSize << ")  Only odd kernel size is allowed." );

    EXPECT_MSG( kernelSize.getHeight( ) % 2 != 0,
                "Invalid kernel size("
                    << kernelSize << ")  Only odd kernel size is allowed." );

    std::vector< int32_t > kernelX(
        static_cast< size_t >( kernelSize.getWidth( ) ) );
    std::vector< int32_t > kernelY(
        static_cast< size_t >( kernelSize.getHeight( ) ) );

    for ( int32_t kx = 0; kx < kernelSize.getWidth( ); kx++ )
    {
        const auto coefficient =
            binomialCoefficient( kernelSize.getWidth( ) - 1, kx );

        kernelX[ static_cast< size_t >( kx ) ] = coefficient;
    }

    for ( int32_t ky = 0; ky < kernelSize.getHeight( ); ky++ )
    {
        const auto coefficient =
            binomialCoefficient( kernelSize.getHeight( ) - 1, ky );

        kernelY[ static_cast< size_t >( ky ) ] = coefficient;
    }

    return convolution( kernelY, kernelX );
}

std::vector< std::vector< int32_t > >
getBoxKernel( const core::SizeI& kernelSize )
{
    EXPECT_MSG( kernelSize.getWidth( ) % 2 != 0,
                "Invalid kernel size("
                    << kernelSize << ")  Only odd kernel size is allowed." );

    EXPECT_MSG( kernelSize.getHeight( ) % 2 != 0,
                "Invalid kernel size("
                    << kernelSize << ")  Only odd kernel size is allowed." );

    std::vector< std::vector< int32_t > > kernel;
    kernel.resize( static_cast< size_t >( kernelSize.getHeight( ) ) );

    for ( auto& rowKernel : kernel )
    {
        rowKernel.resize( static_cast< size_t >( kernelSize.getWidth( ) ), 1 );
    }

    return kernel;
}

std::vector< std::vector< int32_t > >
getSobelKernel( int32_t kernelSize, core::PixelDirection direction )
{
    EXPECT_MSG( kernelSize % 2 != 0,
                "Invalid kernel size("
                    << kernelSize << ")  Only odd kernel size is allowed." );

    const auto binomialKernel = getBinomialKernel( { kernelSize, 1 } );
    const auto derivativeKernel = getFirstDerivativeKernel( kernelSize );

    if ( direction == core::PixelDirection::dX )
    {
        return convolution( binomialKernel[ 0 ], derivativeKernel );
    }

    return convolution( derivativeKernel, binomialKernel[ 0 ] );
}

std::vector< std::vector< int32_t > >
getPrewittKernel( int32_t kernelSize, core::PixelDirection direction )
{
    EXPECT_MSG( kernelSize % 2 != 0,
                "Invalid kernel size("
                    << kernelSize << ")  Only odd kernel size is allowed." );

    const auto boxKernel = getBoxKernel( { kernelSize, 1 } );
    const auto derivativeKernel = getFirstDerivativeKernel( kernelSize );

    if ( direction == core::PixelDirection::dX )
    {
        return convolution( boxKernel[ 0 ], derivativeKernel );
    }

    return convolution( derivativeKernel, boxKernel[ 0 ] );
}

std::vector< int32_t > getFirstDerivativeKernel( int32_t kernelSize )
{
    std::vector< int32_t > kernel( static_cast< size_t >( kernelSize ) );

    int32_t startValue = 1;
    size_t lastIdx = kernel.size( ) - 1;

    for ( size_t i = 0; i < static_cast< size_t >( kernelSize / 2 ); i++ )
    {
        kernel[ i ] = -startValue;
        kernel[ lastIdx-- ] = startValue;

        startValue++;
    }

    return kernel;
}

} // namespace cvl::processing