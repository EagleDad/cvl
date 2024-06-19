#pragma once

template < typename T >
concept Arithmetic = std::is_arithmetic_v< T >;

template < typename Allocator >
constexpr bool isPolymorphicAllocator =
    requires( Allocator allocator, std::size_t bytes, std::size_t align ) {
        allocator.allocate_bytes( bytes, align );
    };

namespace cvl::core
{
enum class Norm
{
    Euclidean,
    Manhattan,
    Maximum
};

enum class BorderType
{
    Replicate, // aaaaaa|abcdefgh|hhhhhhh
    Reflect,   // fedcba | abcdefgh | hgfedcb
    Wrap,      // cdefgh|abcdefgh|abcdefg
};

enum class PixelDirection
{
    dX,
    dY
};

enum class FilterDirection
{
    Row,   // Identification for a horizontal row filter
    Column // Identification for a vertical column filter
};

} // namespace cvl::core
