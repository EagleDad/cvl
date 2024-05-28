// CVL includes
#include <cvl/core/Time.h>

// STD includes
#include <iomanip>

namespace cvl::core
{

std::stringstream getTimeStamp( )
{
    return getTimeStamp( Clock::now( ) );
}

std::stringstream getTimeStamp( const TimeStamp& timestamp )
{
    std::stringstream strTimeStamp;
    const auto in_time_t = Clock::to_time_t( timestamp );
    const auto ms = std::chrono::duration_cast< std::chrono::milliseconds >(
                        timestamp.time_since_epoch( ) )
                        .count( ) %
                    1000;

#if defined( _MSC_VER )
    tm currentLocalTime { };
    const auto timeBuffer = localtime_s( &currentLocalTime, &in_time_t );
    std::ignore = timeBuffer;

    strTimeStamp << std::put_time( &currentLocalTime, "%Y-%m-%d %H:%M:%S." )
                 << std::setw( 3 ) << std::setfill( '0' ) << ms;
#else
    const auto currentLocalTime = localtime(&in_time_t );

    strTimeStamp << std::put_time( currentLocalTime, "%Y-%m-%d %H:%M:%S." )
                 << std::setw( 3 ) << std::setfill( '0' ) << ms;
#endif                 

    return strTimeStamp;
}

} // namespace cvl::core