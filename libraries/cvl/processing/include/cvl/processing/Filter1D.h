#pragma once

// CVL includes
#include <cvl/core/Image.h>
#include <cvl/core/Types.h>
#include <cvl/core/macros.h>
#include <cvl/processing/ColumnFilter.h>
#include <cvl/processing/FilterCoefficients.h>
#include <cvl/processing/FilterOperation.h>
#include <cvl/processing/RowFilter.h>

namespace cvl::processing
{

template < core::FilterDirection Direction, Arithmetic PixelTypeIn,
           int32_t Channels, typename Allocator, Arithmetic PixelTypeOut,
           Arithmetic KernelType >
void filter1D(
    const core::Image< PixelTypeIn, Channels, Allocator >& imageIn,
    core::Image< PixelTypeOut, Channels,
                 typename std::allocator_traits< Allocator >::
                     template rebind_alloc< PixelTypeOut > >& imageOut,
    const std::vector< KernelType >& kernel )
{
    EXPECT_MSG( ! kernel.empty( ),
                "Kernel size(" << kernel.size( ) << "). Kernel cannot be 0" );

    EXPECT_MSG( kernel.size( ) % 2 != 0,
                "Invalid kernel size("
                    << kernel.size( )
                    << ")  Only odd kernel size is allowed for boxBlur" );

    FilterOperation< PixelTypeIn,
                     Channels,
                     Allocator,
                     PixelTypeOut,
                     KernelType,
                     Direction >::applyFilter( imageIn, imageOut, kernel );
}

} // namespace cvl::processing