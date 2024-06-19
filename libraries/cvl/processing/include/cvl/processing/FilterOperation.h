#pragma once

// CVL includes
#include <cvl/core/Image.h>
#include <cvl/core/Types.h>
#include <cvl/core/macros.h>

namespace cvl::processing
{
template < Arithmetic PixelTypeIn, int32_t Channels, typename Allocator,
           Arithmetic PixelTypeOut, Arithmetic KernelType,
           core::FilterDirection Direction >
struct FilterOperation
{
    // member declaration
    static void applyFilter(
        [[maybe_unused]] const core::Image< PixelTypeIn, Channels, Allocator >&
            imageIn,
        [[maybe_unused]] core::Image<
            PixelTypeOut, Channels,
            typename std::allocator_traits< Allocator >::template rebind_alloc<
                PixelTypeIn > >& imageOut,
        [[maybe_unused]] const std::vector< KernelType >& kernel )
    {
        THROW_MSG( "NOT IMPLEMENTED IMAGE TYPE" );
    }
};
} // namespace cvl::processing