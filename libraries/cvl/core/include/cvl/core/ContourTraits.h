#pragma once

// CVL includes
#include <cvl/core/AlignedAllocator.h>
#include <cvl/core/Types.h>

// STD includes
#include <cstdint>
#include <memory>
#include <memory_resource>

namespace cvl::core
{

template < Arithmetic PixelType, typename Allocator >
class ContourTypeTrait;

#define DECLARE_CONTOUR_TYPE_TRAIT( pixelType, allocator )                     \
    template <>                                                                \
    class ContourTypeTrait< pixelType, allocator >                             \
    {                                                                          \
    public:                                                                    \
        using value_type = pixelType;                                          \
        using reference_type = pixelType&;                                     \
        using pointer_type = pixelType*;                                       \
        using const_reference_type = const pixelType&;                         \
        using allocator_type = allocator;                                      \
        using allocator_traits = std::allocator_traits< allocator_type >;      \
    }

DECLARE_CONTOUR_TYPE_TRAIT( uint8_t, std::allocator< uint8_t > );
DECLARE_CONTOUR_TYPE_TRAIT( uint8_t, cvl::core::AlignedAllocator< uint8_t > );
DECLARE_CONTOUR_TYPE_TRAIT( uint8_t,
                            std::pmr::polymorphic_allocator< uint8_t > );

} // namespace cvl::core