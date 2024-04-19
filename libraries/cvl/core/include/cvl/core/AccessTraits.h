#pragma once

// STD includes
#include <cstdint>

// CVL includes
#include <cvl/core/Point.h>
#include <cvl/core/Vector.h>

namespace traits
{
template < typename Object, int32_t ElementIdx >
struct access
{
};

template < int32_t ElementIdx, typename Type, int32_t Dimension >
struct access< cvl::core::Point< Type, Dimension >, ElementIdx >
{
    static Type get( const cvl::core::Point< Type, Dimension >& point )
    {
        return point.at( ElementIdx );
    }
};

template < int32_t ElementIdx, int32_t Dimension >
struct access< cvl::core::Vector< Dimension >, ElementIdx >
{
    static double get( const cvl::core::Vector< Dimension >& vector )
    {
        return vector.at( ElementIdx );
    }
};

} // namespace traits

template < int32_t ElementIdx, typename Type, int32_t Dimension >
constexpr Type get( const cvl::core::Point< Type, Dimension >& point )
{
    return traits::access< cvl::core::Point< Type, Dimension >,
                           ElementIdx >::get( point );
}

template < int32_t ElementIdx, int32_t Dimension >
constexpr double get( const cvl::core::Vector< Dimension >& vector )
{
    return traits::access< cvl::core::Vector< Dimension >, ElementIdx >::get(
        vector );
}
