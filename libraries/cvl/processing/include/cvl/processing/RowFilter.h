#pragma once

// CVL includes
#include <cvl/core/Image.h>
#include <cvl/core/Types.h>
#include <cvl/processing/FilterOperation.h>

namespace cvl::processing
{

//
// FULL TEMPLATE VERSION
//
template < Arithmetic PixelTypeIn, int32_t Channels, typename Allocator,
           Arithmetic PixelTypeOut, Arithmetic KernelType >
struct FilterOperation< PixelTypeIn, Channels, Allocator, PixelTypeOut,
                        KernelType, core::FilterDirection::Row >
{
    static void applyFilter(
        const core::Image< PixelTypeIn, Channels, Allocator >& imageIn,
        core::Image< PixelTypeOut, Channels,
                     typename std::allocator_traits< Allocator >::
                         template rebind_alloc< PixelTypeIn > >& imageOut,
        const std::vector< KernelType >& kernel )
    {
        using sumType = decltype( std::declval< PixelTypeIn >( ) +
                                  std::declval< PixelTypeOut >( ) );

        using allocator_traits = std::allocator_traits< Allocator >;
        using OutAllocator =
            typename allocator_traits::template rebind_alloc< PixelTypeOut >;

        const auto width = imageIn.getWidth( );
        const auto height = imageIn.getHeight( );
        const auto kernelStart = kernel.begin( );
        const auto anchorX = static_cast< int32_t >( kernel.size( ) / 2 );

        if ( imageOut.getSize( ) != imageIn.getSize( ) )
        {
            imageOut = core::Image< PixelTypeOut, 1, OutAllocator >(
                width, height, true );
        }

        KernelType divisor = { };
        for ( const auto& elem : kernel )
        {
            divisor += elem;
        }

        float scale { 1.0f };
        if ( divisor != 0 )
        {
            scale = 1.0f / static_cast< float >( divisor );
        }

        // Left columns
        for ( int32_t c = 0; c < Channels; c++ )
        {
            for ( int32_t y = 0; y < height; y++ )
            {
                auto srcPtr = imageIn.getRowPointer( y, c );
                auto dstPtr = imageOut.getRowPointer( y, c );

                for ( int32_t x = 0; x < anchorX; x++ )
                {
                    sumType sum { };
                    auto kernelIt = kernelStart;

                    for ( int32_t kx = -anchorX; kx <= anchorX; kx++ )
                    {
                        const auto px = std::abs( x + kx ) - x;

                        sum += *( srcPtr + x + px ) *
                               static_cast< sumType >( *kernelIt++ );
                    }

                    sum = static_cast< sumType >( static_cast< float >( sum ) *
                                                  scale );

                    *( dstPtr + x ) = static_cast< uint8_t >( sum );
                }
            }
        }

        // Right columns
        for ( int32_t c = 0; c < Channels; c++ )
        {
            for ( int32_t y = 0; y < height; y++ )
            {
                auto srcPtr = imageIn.getRowPointer( y, c );
                auto dstPtr = imageOut.getRowPointer( y, c );

                for ( int32_t x = width - anchorX; x < width; x++ )
                {
                    sumType sum { };
                    auto kernelIt = kernelStart;

                    for ( int32_t kx = -anchorX; kx <= anchorX; kx++ )
                    {
                        auto idx = x + kx;
                        if ( idx > width - 1 )
                        {
                            const auto delta = idx - ( width - 1 );
                            idx = width - 1 - delta;
                        }
                        const auto px = idx - x;

                        sum += *( srcPtr + x + px ) *
                               static_cast< sumType >( *kernelIt++ );
                    }

                    sum = static_cast< sumType >( static_cast< float >( sum ) *
                                                  scale );

                    *( dstPtr + x ) = static_cast< uint8_t >( sum );
                }
            }
        }

        // Columns between left and right
        for ( int32_t c = 0; c < Channels; c++ )
        {
            for ( int32_t y = 0; y < height; y++ )
            {
                auto srcPtr = imageIn.getRowPointer( y, c );
                auto dstPtr = imageOut.getRowPointer( y, c );

                for ( int32_t x = anchorX; x < width - anchorX; ++x )
                {
                    sumType sum { };
                    auto kernelIt = kernelStart;

                    for ( int32_t kx = -anchorX; kx <= anchorX; ++kx )
                    {
                        sum += *( srcPtr + x + kx ) *
                               static_cast< sumType >( *kernelIt++ );
                    }

                    sum = static_cast< sumType >( static_cast< float >( sum ) *
                                                  scale );

                    *( dstPtr + x ) = static_cast< uint8_t >( sum );
                }
            }
        }
    }
};

} // namespace cvl::processing