#pragma once

// OWN includes
#include <cvl/core/Compare.h>
#include <cvl/core/Size.h>
#include <cvl/processing/export.h>

// STD includes
#include <cstdint>
#include <vector>

namespace cvl::processing
{

/*
 * Function that calculates a sigma for a given kernel size
 *
 * @param [in]  kernelSize   The kernel size
 *
 * @return The sigma for the kernel size
 */
inline double getSigmaFromKernelSize( int32_t kernelSize )
{
    return 0.3 * ( ( kernelSize - 1 ) * 0.5 - 1 ) + 0.8;
}

/*
 * Function that calculates a kernel size for a given sigma
 *
 * @param [in]  sigma   The sigma
 *
 * @return The kernel size for the sigma
 */
inline int32_t getKernelSizeFromSigma( double sigma )
{
    auto kernelSize = std::abs(
        static_cast< int32_t >( 1.0 + ( 1.0 + ( sigma - 0.8 ) / 0.3 ) / 0.5 ) );

    kernelSize = kernelSize % 2 != 1 ? kernelSize + 1 : kernelSize;

    return kernelSize;
}

/**
 *                                                      | n |
 * Function that calculates the binomial coefficient    |   |
 *                                                      | k |
 *                      1
 *                     2 2
 *                    1 2 1
 *                   1 3 3 1
 *                  1 4 6 4 1
 *
 *
 * | n |       n!
 * |   | = ---------
 * | k |    k!(n-k)!
 *
 * @param [in]  n   The input coefficient n (row)
 * @param [in]  k   The input coefficient k (column)
 *
 * @return The current coefficient for n and k. e.g. n = 2, k = 1 returns 1
 */
CVL_PROCESSING_EXPORT int32_t binomialCoefficient( int32_t n, int32_t k );

/*
 * Function that calculates the binomial filter kernel defined by a given size
 *
 * @param [in]  kernelSize   The size of the filter kernel
 *
 * @return The 2-dimensional filter kernel
 */
CVL_PROCESSING_EXPORT std::vector< std::vector< int32_t > >
getBinomialKernel( const core::SizeI& kernelSize );

/*
 * Function that calculates the box filter kernel defined by a given size
 *
 * @param [in]  kernelSize   The size of the filter kernel
 *
 * @return The 2-dimensional filter kernel
 */
CVL_PROCESSING_EXPORT std::vector< std::vector< int32_t > >
getBoxKernel( const core::SizeI& kernelSize );

/*
 * Function that calculates the sobel filter kernel defined by a given size
 *
 * @param [in]  kernelSize   The size of the filter kernel
 *
 * @return The 2-dimensional filter kernel
 */
CVL_PROCESSING_EXPORT std::vector< std::vector< int32_t > >
getSobelKernel( int32_t kernelSize, core::PixelDirection direction );

/*
 * Function that calculates the prewitt filter kernel defined by a given size
 *
 * @param [in]  kernelSize   The size of the filter kernel
 *
 * @return The 2-dimensional filter kernel
 */
CVL_PROCESSING_EXPORT std::vector< std::vector< int32_t > >
getPrewittKernel( int32_t kernelSize, core::PixelDirection direction );

/*
 * Function that calculates the filter kernel  for the first derivation defined
 * by a given size
 *
 * @param [in]  kernelSize   The size of the filter kernel
 *
 * @return The 1-dimensional filter kernel
 */
CVL_PROCESSING_EXPORT std::vector< int32_t >
getFirstDerivativeKernel( int32_t kernelSize );


/*
 * Function that check if a 2D filter kernel is separable.
 *
 * A two-dimensional filter kernel is separable if it can be expressed as the
 * outer product of two vectors. For example, let's look at a binomial kernel
 *
 * | 1 2 1 |   | 1 |
 * | 2 4 2 | = | 2 | * | 1 2 1 |
 * | 1 2 1 |   | 1 |
 *
 *
 * @param [in]  kernel  The 2D input kernel to check
 *
 * @return Returns true if separable, else false
 */
template < typename T >
bool isSeparableFilter( const std::vector< std::vector< T > >& kernel );

// Template implementations
template < typename T >
bool isSeparableFilter( const std::vector< std::vector< T > >& kernel )
{
    if ( kernel.size( ) == 0 )
    {
        return false;
    }

    std::vector< T > kernelX( kernel[ 0 ].size( ) );
    std::vector< T > kernelY( kernel.size( ) );

    for ( size_t i = 0; i < kernel[ 0 ].size( ); i++ )
    {
        kernelX[ i ] = kernel[ 0 ][ i ];
    }

    for ( size_t i = 0; i < kernel.size( ); i++ )
    {
        kernelY[ i ] = kernel[ i ][ 0 ];
    }

    bool separable { true };

    for ( size_t ky = 0; ky < kernel.size( ); ky++ )
    {
        for ( size_t kx = 0; kx < kernel[ ky ].size( ); kx++ )
        {
            const auto coefficient = kernelX[ kx ] * kernelY[ ky ];
            const auto compare = kernel[ ky ][ kx ];

            if ( ! core::isEqual( coefficient, compare ) )
            {
                separable = false;
                break;
            }
        }
    }

    return separable;
}

} // namespace cvl::processing