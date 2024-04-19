#pragma once

// STD includes
#include <cstdint>

// CVL includes
#include <cvl/core/Point.h>
#include <cvl/core/Vector.h>

namespace traits
{

template < typename Object >
struct dimension
{
};

template < typename Type, int32_t Dimension >
struct dimension< cvl::core::Point< Type, Dimension > >
    : std::integral_constant< int32_t, Dimension >
{
};

template < int32_t Dimension >
struct dimension< cvl::core::Vector< Dimension > >
    : std::integral_constant< int32_t, Dimension >
{
};

} // namespace traits

template < typename Object >
struct dimension : traits::dimension< Object >
{
};