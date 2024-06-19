#pragma once

// CVL includes
#include <cvl/core/Image.h>
#include <cvl/core/macros.h>
#include <cvl/processing/Filter1D.h>
#include <cvl/processing/FilterCoefficients.h>

namespace cvl::processing
{

template < Arithmetic PixelTypeIn, int32_t Channels, typename Allocator,
           Arithmetic PixelTypeOut, Arithmetic KernelType >
void separableFilter2D(
    const core::Image< PixelTypeIn, Channels, Allocator >& imageIn,
    core::Image< PixelTypeOut, Channels,
                 typename std::allocator_traits< Allocator >::
                     template rebind_alloc< PixelTypeIn > >& imageOut,
    const std::vector< KernelType >& rowKernel,
    const std::vector< KernelType >& columnKernel )
{
    using allocator_traits = std::allocator_traits< Allocator >;
    using OutAllocator =
        typename allocator_traits::template rebind_alloc< PixelTypeOut >;

    EXPECT_MSG( ! rowKernel.empty( ),
                "Row kernel size(" << rowKernel.size( )
                                   << "). Kernel cannot be 0" );

    EXPECT_MSG( ! columnKernel.empty( ),
                "Row kernel size(" << columnKernel.size( )
                                   << "). Kernel cannot be 0" );

    if ( imageOut.getSize( ) != imageIn.getSize( ) )
    {
        imageOut = core::Image< PixelTypeOut, 1, OutAllocator >(
            imageIn.getSize( ), true );
    }

    auto imageIntermediate = core::Image< PixelTypeOut, 1, OutAllocator >(
        imageIn.getSize( ), true );

    // x
    // x vertical column filter
    // x

    // Transpose

    // x x x horizontal row filter

    // Transpose

    filter1D< core::FilterDirection::Column >(
        imageIn, imageIntermediate, rowKernel );

    filter1D< core::FilterDirection::Row >(
        imageIntermediate, imageOut, columnKernel );
}

template < Arithmetic PixelTypeIn, int32_t Channels, typename Allocator,
           Arithmetic PixelTypeOut, Arithmetic KernelType >
void filter2D(
    const core::Image< PixelTypeIn, Channels, Allocator >& imageIn,
    core::Image< PixelTypeOut, Channels,
                 typename std::allocator_traits< Allocator >::
                     template rebind_alloc< PixelTypeOut > >& imageOut,
    const std::vector< std::vector< KernelType > >& filterKernel )
{
    using allocator_traits = std::allocator_traits< Allocator >;
    using OutAllocator =
        typename allocator_traits::template rebind_alloc< PixelTypeOut >;

    EXPECT_MSG( ! filterKernel.empty( ),
                "Invalid kernel size(" << filterKernel.size( )
                                       << "). Kernel cannot be 0" );

    EXPECT_MSG( filterKernel[ 0 ].size( ) != 0,
                "Invalid kernel size(" << filterKernel[ 0 ].size( )
                                       << "). Kernel cannot be 0" );

    EXPECT_MSG( static_cast< void* >( imageIn.getData( ) ) !=
                    static_cast< void* >( imageOut.getData( ) ),
                "Input image cannot be the output image" );

    if ( isSeparableFilter( filterKernel ) )
    {
        std::vector< KernelType > rowKernel;
        std::vector< KernelType > colKernel;

        rowKernel.reserve( filterKernel.size( ) );
        colKernel.reserve( filterKernel[ 0 ].size( ) );

        for ( const auto& row : filterKernel )
        {
            rowKernel.emplace_back( row[ 0 ] );
        }

        for ( const auto& col : filterKernel[ 0 ] )
        {
            colKernel.emplace_back( col );
        }

        separableFilter2D( imageIn, imageOut, rowKernel, colKernel );

        return;
    }

    const auto anchorX = static_cast< int32_t >( filterKernel.size( ) / 2 );
    const auto anchorY =
        static_cast< int32_t >( filterKernel[ 0 ].size( ) / 2 );

    if ( imageOut.getSize( ) != imageIn.getSize( ) )
    {
        imageOut = core::Image< PixelTypeOut, 1, OutAllocator >(
            imageIn.getSize( ), true );
    }

    using sumType = decltype( std::declval< PixelTypeIn >( ) +
                              std::declval< PixelTypeOut >( ) );

    sumType sum { };
    KernelType divisor = { };

    for ( size_t y = 0; y < filterKernel.size( ); y++ )
    {
        for ( size_t x = 0; x < filterKernel[ y ].size( ); x++ )
        {
            // NO ABS
            divisor += std::abs( filterKernel[ y ][ x ] );
        }
    }

    const auto width = imageIn.getWidth( );
    const auto height = imageIn.getHeight( );

    // First apply the filter to the border related pixels
    for ( int32_t c = 0; c < Channels; c++ )
    {
        for ( int32_t y = 0; y < height; y++ )
        {
            for ( int32_t x = 0; x < width; x++ )
            {
                if ( y >= anchorY && y <= height - 2 * anchorY && x == anchorX )
                {
                    x += width - 2 * anchorX - 1;
                    continue;
                }

                sum = { };

                for ( int32_t ky = -anchorY; ky <= anchorY; ky++ )
                {
                    // TODO: Use row pointer here
                    for ( int32_t kx = -anchorX; kx <= anchorX; kx++ )
                    {
                        auto px = x + kx;
                        auto py = y + ky;

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

                        const auto coefficient = filterKernel[ iy ][ ix ];

                        sum += imageIn.at( py, px, c ) *
                               static_cast< sumType >( coefficient );
                    }
                }

                sum /= static_cast< sumType >( divisor );

                imageOut.at( y, x, c ) = static_cast< PixelTypeOut >( sum );
            }
        }
    }

    // TODO: Use Border Handling
    for ( int32_t c = 0; c < Channels; c++ )
    {
        for ( int32_t y = anchorY; y < height - anchorY; y++ )
        {
            for ( int32_t x = anchorX; x < width - anchorX; x++ )
            {
                sum = { };

                for ( int32_t ky = -anchorY; ky <= anchorY; ky++ )
                {
                    for ( int32_t kx = -anchorX; kx <= anchorX; kx++ )
                    {
                        auto px = x + kx;
                        auto py = y + ky;

                        const auto ix = static_cast< size_t >( kx ) + anchorX;
                        const auto iy = static_cast< size_t >( ky ) + anchorY;

                        const auto coefficient = filterKernel[ iy ][ ix ];

                        sum += imageIn.at( py, px, c ) *
                               static_cast< sumType >( coefficient );
                    }
                }

                sum /= static_cast< sumType >( divisor );

                imageOut.at( y, x, c ) = static_cast< PixelTypeOut >( sum );
            }
        }
    }
}

} // namespace cvl::processing