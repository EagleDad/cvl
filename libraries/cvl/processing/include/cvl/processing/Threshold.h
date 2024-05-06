#pragma once

// CVL includes
#include <cvl/core/Image.h>
#include <cvl/core/Region.h>

// STD includes

namespace cvl::processing
{
template < typename Allocator >
using allocator_traits = std::allocator_traits< Allocator >;

/**
 * Segments the input image using global threshold
 *
 * @param [in]   imageIn      The input image
 * @param [in]   regionOut    The segmented binary output region
 * @param [in]   threshold    The threshold value to use
 * @param [in]   maxValue     The value to be used for the foreground
 * pixels.
 *
 * go > threshValue ? maxValue : 0x00
 */
template < Arithmetic PixelType, typename Allocator,
           template < typename > typename... RegionFeature >
void threshold( const core::Image< PixelType, 1, Allocator >& imageIn,
                core::Region< uint8_t,
                              typename allocator_traits<
                                  Allocator >::template rebind_alloc< uint8_t >,
                              RegionFeature... >& regionOut,
                PixelType threshold, uint8_t maxValue = uint8_t { 1 } )
{
    using OutAllocator = typename allocator_traits<
        Allocator >::template rebind_alloc< uint8_t >;

    if ( regionOut.getLabelImage( ).getSize( ) != imageIn.getSize( ) )
    {
        core::Image< uint8_t, 1, OutAllocator > regionImage( imageIn.getSize( ),
                                                             false );

        regionOut = core::Region< uint8_t, OutAllocator, RegionFeature... >(
            regionImage, maxValue );
    }

    if ( regionOut.getLabelNumber( ) != maxValue )
    {
        regionOut = core::Region< uint8_t, OutAllocator, RegionFeature... >(
            regionOut.getLabelImage( ), maxValue );
    }

    const auto imageWidth = imageIn.getWidth( );
    const auto imageHeight = imageIn.getHeight( );

    for ( int32_t y = 0; y < imageHeight; y++ )
    {
        const auto srcPtr = imageIn.getRowPointer( y );
        const auto dstPtr = regionOut.getLabelImage( ).getRowPointer( y );

        for ( int32_t x = 0; x < imageWidth; x++ )
        {
            if ( srcPtr[ x ] > threshold )
            {
                dstPtr[ x ] = maxValue;
            }
            else
            {
                dstPtr[ x ] = uint8_t { 0 };
            }
        }
    }
}

} // namespace cvl::processing