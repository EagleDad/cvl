#pragma once

// CVL includes
#include <cvl/core/export.h>

// STD includes
#include <chrono>
#include <functional>
#include <sstream>

using Clock = std::chrono::system_clock;
using TimeStamp = Clock::time_point;

namespace cvl::core
{

CVL_CORE_EXPORT
std::stringstream getTimeStamp( );

CVL_CORE_EXPORT
std::stringstream getTimeStamp( const TimeStamp& timestamp );

/**
 * This function measures the execution time of the passed function.
 *
 * E.g.:
 *
 * Ellipse ellipse;
 *
 * measureFunction(
 *      [ & ]( ) {
 *          auto file = readFile( fileName );
 *      } );
 *
 * @param [in]   function   The function to measure.
 *
 * @return The elapsed time in micro seconds.
 *
 */
template < typename TimeType >
TimeType measureFunction( const std::function< void( ) >& function )
{
    const auto start = std::chrono::system_clock::now( );

    function( );

    const auto end = std::chrono::system_clock::now( );
    const auto elapsed = std::chrono::duration_cast< TimeType >( end - start );

    return elapsed;
}

} // namespace cvl::core