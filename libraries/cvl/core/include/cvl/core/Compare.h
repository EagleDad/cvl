#pragma once

// Std includes
#include <cmath>
#include <limits>
#include <type_traits>

namespace cvl::core
{

/**
 * Function that compares if to values are equal, considering floating point
 * uncertainty.
 *
 * @param [in]  a       The first value.
 * @param [in]  b       The second value.
 * @param [in]  epsilon The epsilon to use for comparison.
 *
 * @return True if they are equal, else false.
 *
 */
template < typename T >
std::enable_if_t< std::is_arithmetic_v< T >, bool >
isEqual( T a, T b, T epsilon = std::numeric_limits< T >::epsilon( ) )
{
    if constexpr ( ! std::is_floating_point_v< T > )
    {
        return a == b;
    }
    else
    {
        return std::fabs( a - b ) < epsilon;
    }
}

/**
 * Function that compares if to values are equal, considering floating point
 * uncertainty with scale factor.
 *
 * @param [in]  a       The first value.
 * @param [in]  b       The second value.
 * @param [in]  ulp     The scale factor for the epsilon value for comparison.
 * @param [in]  epsilon The epsilon to use for comparison.
 *
 * @return True if they are equal, else false.
 *
 */
template < typename T >
std::enable_if_t< std::is_floating_point_v< T >, bool >
isEqual( T a, T b, int32_t ulp,
         T epsilon = std::numeric_limits< T >::epsilon( ) )
{
    return std::fabs( a - b ) <=
               epsilon * std::fabs( a + b ) * static_cast< T >( ulp ) ||
           std::fabs( a - b ) < std::numeric_limits< T >::min( );
}

/**
 * Function that compares if a value is zero, considering floating point
 * uncertainty.
 *
 * @param [in]  val     The value to check.
 * @param [in]  epsilon The epsilon to use for comparison.
 *
 * @return True if zero, else false.
 *
 */
template < typename T >
std::enable_if_t< std::is_arithmetic_v< T >, bool >
isZero( T val, T epsilon = std::numeric_limits< T >::epsilon( ) )
{
    if constexpr ( ! std::is_floating_point_v< T > )
    {
        return val == T( );
    }
    else
    {
        return std::fabs( val ) <= epsilon;
    }
}

/**
 * Function that compares if value a is lower then value b, considering floating
 * point uncertainty.
 *
 * @param [in]  a       The first value.
 * @param [in]  b       The second value.
 * @param [in]  epsilon The epsilon to use for comparison.
 *
 * @return True if a is lower then b, else false.
 *
 */
template < typename T >
std::enable_if_t< std::is_arithmetic_v< T >, bool >
isLowerEqual( T a, T b, T epsilon = std::numeric_limits< T >::epsilon( ) )
{
    if constexpr ( ! std::is_floating_point_v< T > )
    {
        return a <= b;
    }
    else
    {
        if ( isEqual( a, b, epsilon ) )
        {
            return true;
        }

        return a < b;
    }
}

/**
 * Function that compares if value a is greater then value b, considering
 * floating point uncertainty.
 *
 * @param [in]  a       The first value.
 * @param [in]  b       The second value.
 * @param [in]  epsilon The epsilon to use for comparison.
 *
 * @return True if a is greater then b, else false.
 *
 */
template < typename T >
std::enable_if_t< std::is_arithmetic_v< T >, bool >
isGreaterEqual( T a, T b, T epsilon = std::numeric_limits< T >::epsilon( ) )
{
    if constexpr ( ! std::is_floating_point_v< T > )
    {
        return a >= b;
    }
    else
    {
        if ( isEqual( a, b, epsilon ) )
        {
            return true;
        }

        return a > b;
    }
}

} // namespace cvl::core