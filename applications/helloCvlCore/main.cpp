#pragma warning( push )
#pragma warning( disable : 4868 )

// CVL Includes
#include <cvl/core/ConsoleLoggingBackend.h>
#include <cvl/core/DebugMemoryResource.h>
#include <cvl/core/ILogger.h>
#include <cvl/core/Image.h>

// STD includes
#include <algorithm>
#include <array>

template < typename PixelType,
           typename Allocator = cvl::core::AlignedAllocator< PixelType > >
cvl::core::Image< PixelType, 1,
                  typename std::allocator_traits<
                      Allocator >::template rebind_alloc< PixelType > >
threshold( const cvl::core::Image< PixelType, 1, Allocator >& imageIn,
           PixelType threshold, uint8_t maxValue = uint8_t { 1 } )
{
    using allocator_traits = std::allocator_traits< Allocator >;
    using OutAllocator =
        typename allocator_traits::template rebind_alloc< PixelType >;

    auto segmented = cvl::core::getCalculationImage< PixelType >( imageIn );

    for ( int32_t y = 0; y < imageIn.getHeight( ); y++ )
    {
        const auto srcPtr = imageIn.getRowPointer( y );
        const auto dstPtr = segmented.getRowPointer( y );

        for ( int32_t x = 0; x < imageIn.getWidth( ); x++ )
        {
            if ( srcPtr[ x ] > threshold )
            {
                dstPtr[ x ] = maxValue;
            }
        }
    }

    return segmented;
}

int32_t main( [[maybe_unused]] int32_t argc, [[maybe_unused]] char** argv )
{
    auto logger = cvl::core::getLoggerInterface( );
    cvl::core::ConsoleLoggingBackend consoleLogger;
    logger->setSeverity( cvl::core::ILogger::Severity::Debug );

    LOG_INFO( "PMR Test" );

    cvl::core::DebugMemoryResource defaultAlloc {
        "Rogue PMR Allocation!", std::pmr::null_memory_resource( ) };
    std::pmr::set_default_resource( &defaultAlloc );

    cvl::core::DebugMemoryResource oom { "Out of Memory",
                                         std::pmr::null_memory_resource( ) };

    std::array< unsigned char, 4096 > buffer { };
    void* ptr = buffer.data( );
    std::ranges::fill( buffer, uint8_t { 0x80 } );

    std::pmr::monotonic_buffer_resource underlying_bytes(
        buffer.data( ), buffer.size( ), &oom );

    cvl::core::DebugMemoryResource monotonic { "Monotonic Array",
                                               &underlying_bytes };

    std::pmr::pool_options opt;
    opt.largest_required_pool_block = 256;
    opt.max_blocks_per_chunk = 3;
    std::pmr::unsynchronized_pool_resource unsync_pool { opt, &monotonic };

    cvl::core::DebugMemoryResource pool { "Pool", &unsync_pool };

    LOG_INFO( "Data at: " << ptr );

    for ( int32_t i = 0; i < 10; i++ )
    {
        LOG_INFO( "Iteration " << i );

        cvl::core::
            Image< uint8_t, 1, std::pmr::polymorphic_allocator< uint8_t > >
                pmrImag1( &pool );

        pmrImag1.allocate( 15, 15, true );

        void* p1 = pmrImag1.getData( );
        LOG_INFO( "Allocated at: " << p1 );
        const auto adr = reinterpret_cast< uintptr_t >( p1 ) % 64;
        LOG_INFO( "Align1: " << adr );

        LOG_INFO( "Thresholding" );
        auto reg = threshold( pmrImag1, uint8_t { 45 } );

        LOG_INFO( "Vector" );
        std::pmr::vector< int32_t > myVec( &pool );

        LOG_WARNING( "Push_Back" );
        myVec.push_back( 10 );

        cvl::core::
            Image< uint8_t, 1, std::pmr::polymorphic_allocator< uint8_t > >
                pmrImag2( &pool );

        pmrImag2.allocate( 15, 15, true );

        void* p2 = pmrImag2.getData( );
        LOG_INFO( "Allocated at: " << p2 );
        const auto adr2 = reinterpret_cast< uintptr_t >( p2 ) % 64;
        LOG_INFO( "Align2: " << adr2 );
    }

    return 0;
}

#pragma warning( pop )