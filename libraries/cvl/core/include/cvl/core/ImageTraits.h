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
class ImageTypeTrait;

#define DECLARE_IMAGE_TYPE_TRAIT( pixelType, allocator, promoteType )          \
    template <>                                                                \
    class ImageTypeTrait< pixelType, allocator >                               \
    {                                                                          \
    public:                                                                    \
        using value_type = pixelType;                                          \
        using reference_type = pixelType&;                                     \
        using pointer_type = pixelType*;                                       \
        using const_reference_type = const pixelType&;                         \
        using allocator_type = allocator;                                      \
        using allocator_traits = std::allocator_traits< allocator_type >;      \
        using promote_type = promoteType;                                      \
    }

DECLARE_IMAGE_TYPE_TRAIT( uint8_t, std::allocator< uint8_t >, int32_t );
DECLARE_IMAGE_TYPE_TRAIT( uint8_t, cvl::core::AlignedAllocator< uint8_t >,
                          int32_t );
DECLARE_IMAGE_TYPE_TRAIT( uint8_t, std::pmr::polymorphic_allocator< uint8_t >,
                          int32_t );

} // namespace cvl::core