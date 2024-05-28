#pragma once

// STD includes
#include <utility>

namespace cvl::core
{

/**
 * call_once function is only executed as long as the bool flag is false.
 * The first function calling call_once with the bool flag, will set the flag to
 * true.
 *
 * @param flag      The flag that keep the stat if the function has already been
 *                  called.
 * @param func      The function to call.
 * @param args      The arguments for the function to call.
 *
 */
template < class Callable, class... Args >
void call_once( bool& flag, Callable&& func, Args&&... args )
{
    if ( ! flag )
    {
        func( std::forward< decltype( args ) >( args )... );
        flag = true;
    }
}

} // namespace cvl::core