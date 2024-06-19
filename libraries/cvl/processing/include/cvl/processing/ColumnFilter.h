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
                        KernelType, core::FilterDirection::Column >

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
        const auto stride = imageIn.getStride( );
        const auto kernelStart = kernel.begin( );
        const auto anchorY = static_cast< int32_t >( kernel.size( ) / 2 );

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

        // Top lines
        for ( int32_t c = 0; c < Channels; c++ )
        {
            for ( int32_t y = 0; y < anchorY; y++ )
            {
                auto srcPtr = imageIn.getRowPointer( y, c );
                auto dstPtr = imageOut.getRowPointer( y, c );

                for ( int32_t x = 0; x < width; x++ )
                {
                    sumType sum { };
                    auto kernelIt = kernelStart;

                    for ( int32_t ky = -anchorY; ky <= anchorY; ++ky )
                    {
                        const auto py = std::abs( y + ky ) - y;

                        sum += *( srcPtr + py * stride + x ) *
                               static_cast< sumType >( *kernelIt++ );
                    }

                    sum = static_cast< sumType >( static_cast< float >( sum ) *
                                                  scale );

                    *( dstPtr + x ) = static_cast< uint8_t >( sum );
                }
            }
        }

        // Bottom lines
        for ( int32_t c = 0; c < Channels; c++ )
        {
            for ( int32_t y = height - anchorY; y < height; y++ )
            {
                auto srcPtr = imageIn.getRowPointer( y, c );
                auto dstPtr = imageOut.getRowPointer( y, c );

                for ( int32_t x = 0; x < width; x++ )
                {
                    sumType sum { };
                    auto kernelIt = kernelStart;

                    for ( int32_t ky = -anchorY; ky <= anchorY; ++ky )
                    {
                        auto idx = y + ky;
                        if ( idx > height - 1 )
                        {
                            const auto delta = idx - ( height - 1 );
                            idx = height - 1 - delta;
                        }
                        const auto py = idx - y;

                        sum += *( srcPtr + py * stride + x ) *
                               static_cast< sumType >( *kernelIt++ );
                    }

                    sum = static_cast< sumType >( static_cast< float >( sum ) *
                                                  scale );

                    *( dstPtr + x ) = static_cast< uint8_t >( sum );
                }
            }
        }

        // Lines between top and bottom
        for ( int32_t c = 0; c < Channels; c++ )
        {
            for ( int32_t y = anchorY; y < height - anchorY; y++ )
            {
                auto srcPtr = imageIn.getRowPointer( y, c );
                auto dstPtr = imageOut.getRowPointer( y, c );

                for ( int32_t x = 0; x < width; x++ )
                {
                    sumType sum { };
                    auto kernelIt = kernelStart;

                    for ( int32_t ky = -anchorY; ky <= anchorY; ++ky )
                    {
                        sum += *( srcPtr + ky * stride + x ) *
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