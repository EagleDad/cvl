#pragma once

// STD includes
#include <algorithm>

namespace cvl::core
{
template < typename ValueType >
    requires std::is_integral_v< ValueType >
ValueType alignTo( ValueType value, ValueType alignment )
{
    auto val = std::max( static_cast< ValueType >( value + alignment - 1 ),
                         alignment );
    return static_cast< ValueType >( val - val % alignment );
}
} // namespace cvl::core