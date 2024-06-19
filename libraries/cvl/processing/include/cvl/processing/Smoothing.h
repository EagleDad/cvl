#pragma once

// CVL includes
#include <cvl/core/Image.h>
#include <cvl/core/macros.h>
#include <cvl/processing/Filter2D.h>
#include <cvl/processing/FilterCoefficients.h>

namespace cvl::processing
{

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
void boxBlur( const core::Image< PixelType, Channels, Allocator >& imageIn,
              core::Image< PixelType, Channels, Allocator >& imageOut,
              const core::SizeI& kernelSize )
{
    EXPECT_MSG( kernelSize.getWidth( ) % 2 != 0,
                "Invalid kernel size("
                    << kernelSize
                    << ")  Only odd kernel size is allowed for boxBlur" );

    EXPECT_MSG( kernelSize.getHeight( ) % 2 != 0,
                "Invalid kernel size("
                    << kernelSize
                    << ")  Only odd kernel size is allowed for boxBlur" );

    const auto kernel = getBoxKernel( kernelSize );

    filter2D( imageIn, imageOut, kernel );
}

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
void binomialBlur( const core::Image< PixelType, Channels, Allocator >& imageIn,
                   core::Image< PixelType, Channels, Allocator >& imageOut,
                   const core::SizeI& kernelSize )
{
    EXPECT_MSG( kernelSize.getWidth( ) % 2 != 0,
                "Invalid kernel size("
                    << kernelSize
                    << ")  Only odd kernel size is allowed for boxBlur" );

    EXPECT_MSG( kernelSize.getHeight( ) % 2 != 0,
                "Invalid kernel size("
                    << kernelSize
                    << ")  Only odd kernel size is allowed for boxBlur" );

    const auto kernel = getBinomialKernel( kernelSize );

    filter2D( imageIn, imageOut, kernel );
}

} // namespace cvl::processing