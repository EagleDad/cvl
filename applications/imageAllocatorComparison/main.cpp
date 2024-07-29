#pragma warning( push )
#pragma warning( disable : 4868 )

// CVL Includes
#include <cvl/core/ConsoleLoggingBackend.h>
#include <cvl/core/ILogger.h>
#include <cvl/core/Image.h>
#include <cvl/core/SpinLock.h>
#include <cvl/core/Time.h>

// STD includes
#include "cvl/core/Compare.h"

#include <algorithm>
#include <list>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>

#if defined( WIN32 )
    #include <conio.h>
#else
#include <unistd.h>
#include <termios.h>

char getch(void)
{
    char buf = 0;
    struct termios old = {0};
    fflush(stdout);
    if(tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if(tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if(read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if(tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    printf("%c\n", buf);
    return buf;
 }
#endif

#if defined( BUILD_WITH_TBB )

// TBB includes
    #define TBB_PREVIEW_MEMORY_POOL 1
    #include <oneapi/tbb/cache_aligned_allocator.h>
    #include <oneapi/tbb/memory_pool.h>
    #include <oneapi/tbb/scalable_allocator.h>
    #include <oneapi/tbb/tbb_allocator.h>
    #include <oneapi/tbb/tbbmalloc_proxy.h>

tbb::memory_pool< tbb::tbb_allocator< uint8_t > > tbbPool;

#endif

constexpr auto IMAGE_WIDTH = 1024;
constexpr auto IMAGE_HEIGHT = 1024;
constexpr auto ITERATIONS = 100;
const auto RESULT_ROOT = "C:/TEMP/Fragmentation";
constexpr std::chrono::microseconds sleepTime { 15 };

struct ThreadData
{
    ThreadData( ) = default;
    ThreadData( const ThreadData& ) = default;
    ThreadData( ThreadData&& ) = default;
    ThreadData& operator=( const ThreadData& ) = default;
    ThreadData& operator=( ThreadData&& ) = default;

    ThreadData( bool run, std::string&& name )
        : mRun( run )
        , mName( std::move( name ) )
    {
    }
    bool mRun { true };
    std::string mName { };
};

struct DummyData
{
    int32_t mRandomInt { };
    double mRandomDouble { };

    bool operator==( const DummyData& rhs ) const
    {
        return cvl::core::isEqual( mRandomInt, rhs.mRandomInt ) &&
               cvl::core::isEqual( mRandomDouble, rhs.mRandomDouble );
    }

    bool operator!=( const DummyData& rhs ) const
    {
        return ! operator==( rhs );
    }
};

using namespace cvl::core;

SpinLock spinLock;
std::vector< uint8_t > poolBuffer;
std::unique_ptr< std::pmr::monotonic_buffer_resource > underlyingBytes;
std::pmr::pool_options options;
std::unique_ptr< std::pmr::unsynchronized_pool_resource > unsynchronizedPool;

template < typename Allocator >
Image< uint8_t, 1, Allocator > createImage( )
{
    if constexpr ( std::is_same_v<
                       Allocator,
                       std::pmr::polymorphic_allocator< uint8_t > > )
    {
        return Image< uint8_t, 1, std::pmr::polymorphic_allocator< uint8_t > >(
            IMAGE_WIDTH,
            IMAGE_HEIGHT,
            false,
            std::pmr::polymorphic_allocator< uint8_t >(
                unsynchronizedPool.get( ) ) );
    }
#if defined( BUILD_WITH_TBB )
    else if constexpr ( std::is_same_v<
                            Allocator,
                            tbb::memory_pool_allocator< uint8_t > > )
    {
        return Image< uint8_t, 1, Allocator > {
            IMAGE_WIDTH, IMAGE_HEIGHT, false, Allocator( tbbPool ) };
    }

#endif
    else
    {
        return Image< uint8_t, 1, Allocator >(
            IMAGE_WIDTH, IMAGE_HEIGHT, false );
    }
}

template < typename PixelType, typename Allocator >
Image< PixelType, 1,
       typename std::allocator_traits< Allocator >::template rebind_alloc<
           PixelType > >
threshold( const Image< PixelType, 1, Allocator >& imageIn, PixelType threshold,
           uint8_t maxValue = uint8_t { 1 } )
{
    using allocator_traits = std::allocator_traits< Allocator >;
    using OutAllocator =
        typename allocator_traits::template rebind_alloc< PixelType >;

    auto segmented = Image< PixelType, 1, Allocator >(
        imageIn.getSize( ), false, imageIn.getAllocator( ) );

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

template < typename Allocator >
void threadFunc( ThreadData* data )
{
    {
        Lock lock( spinLock );
        LOG_INFO( data->mName + " started" );
    }

    auto imageToCopy = createImage< Allocator >( );

    // constexpr auto bytesProcesses = static_cast< size_t >( IMAGE_HEIGHT ) *
    //                                 static_cast< size_t >( IMAGE_HEIGHT ) *
    //                                 static_cast< size_t >( ITERATIONS );
    // const auto gigaBytesProcesses =
    //     bytesProcesses / static_cast< double >( std::pow( 1024, 3 ) );

    std::filesystem::path filePathAndName( RESULT_ROOT );
    std::string fileName = data->mName;
    std::ranges::replace( fileName, ':', '_' );
    filePathAndName /= fileName + ".txt";

    std::ofstream resultFile( filePathAndName.string( ) );

    while ( data->mRun )
    {
        auto elapsedTime = measureFunction< std::chrono::microseconds >(
            [ & ]( )
            {
                for ( int32_t i = 0; i < ITERATIONS; i++ )
                {
                    // const Image< uint8_t, 1, Allocator > imgCopy( imageToCopy
                    // );
                    const auto segmented =
                        threshold( imageToCopy, uint8_t { 128 } );

                    if ( segmented.getSize( ) != imageToCopy.getSize( ) )
                    {
                        std::cout << "ERROR"
                                  << "\n";
                    }
                }
            } );

        resultFile << getTimeStamp( ).str( ) << ';' << elapsedTime.count() << '\n';

        resultFile.flush( );

        std::this_thread::sleep_for( sleepTime );

        // const auto elapsedSeconds =
        //     static_cast< double >( elapsedTime.count( ) ) /
        //     static_cast< double >( 1e6 );

        // const auto gbPerSec = gigaBytesProcesses / elapsedSeconds;
        //{
        // Lock lock( spinLock );
        // LOG_INFO( data->mName );
        // LOG_INFO( "Elapsed Time: "
        //           << static_cast< double >( elapsedTime.count( ) ) /
        //                  static_cast< double >( ITERATIONS )
        //           << "us. GB/s: " << gbPerSec );
        //}
    }

    resultFile.close( );
    {
        Lock lock( spinLock );
        LOG_INFO( data->mName + " terminates" );
    }
}

template < typename Allocator >
void threadFuncFragmentation( ThreadData* data )
{
    {
        Lock lock( spinLock );
        LOG_INFO( data->mName + " started" );
    }

    while ( data->mRun )
    {
        std::vector< DummyData, Allocator > dataVector;
        std::list< DummyData, Allocator > dataList;

        std::random_device rd;     // a seed source for the random number engine
        std::mt19937 gen( rd( ) ); // mersenne_twister_engine seeded with rd()
        std::uniform_int_distribution<> intDistribution( 1, 6 );
        std::uniform_real_distribution<> realDistribution( 1.0, 6.0 );

        for ( int32_t i = 0; i < ITERATIONS * ITERATIONS * ITERATIONS; i++ )
        {
            DummyData dummyData;
            dummyData.mRandomInt = intDistribution( gen );
            dummyData.mRandomDouble = realDistribution( gen );

            dataVector.push_back( dummyData );
            dataList.push_back( dummyData );
        }

        if ( *dataVector.begin( ) != *dataList.begin( ) )
        {
            LOG_ERROR( "Wrong elements" );
        }

        std::this_thread::sleep_for( sleepTime );
    }

    {
        Lock lock( spinLock );
        LOG_INFO( data->mName + " terminates" );
    }
}

template < typename Allocator >
void addThread( std::vector< std::thread >& threads,
                std::vector< std::unique_ptr< ThreadData > >& threadData,
                std::string name )
{
    threadData.emplace_back(
        std::make_unique< ThreadData >( true, std::move( name ) ) );

    threads.emplace_back( threadFunc< Allocator >, threadData.back( ).get( ) );
}

template < typename Allocator >
void addFragmentationThread(
    std::vector< std::thread >& threads,
    std::vector< std::unique_ptr< ThreadData > >& threadData, std::string name )
{
    threadData.emplace_back(
        std::make_unique< ThreadData >( true, std::move( name ) ) );

    threads.emplace_back(
        threadFuncFragmentation< std::allocator< DummyData > >,
        threadData.back( ).get( ) );
}

int32_t main( [[maybe_unused]] int32_t argc, [[maybe_unused]] char** argv )
{
    // Do dummy allocation
    std::vector< uint8_t > dummyVector;
    constexpr size_t kByteSize = 1024;
    constexpr size_t numGbs = 4;
    constexpr size_t dummySize = kByteSize * kByteSize * kByteSize * numGbs;
    dummyVector.resize( dummySize );

    const auto logger = getLoggerInterface( );
    ConsoleLoggingBackend consoleLogger;
    logger->setSeverity( ILogger::Severity::Debug );

    poolBuffer.resize( 40 * IMAGE_WIDTH * IMAGE_HEIGHT );

    underlyingBytes = std::make_unique< std::pmr::monotonic_buffer_resource >(
        poolBuffer.data( ),
        poolBuffer.size( ),
        std::pmr::null_memory_resource( ) );

    options.largest_required_pool_block = IMAGE_WIDTH * IMAGE_HEIGHT;
    options.max_blocks_per_chunk = 4;

    unsynchronizedPool =
        std::make_unique< std::pmr::unsynchronized_pool_resource >(
            options, underlyingBytes.get( ) );

    LOG_INFO( "IMAGE ALLOCATOR TEST" );

    LOG_INFO( "Image Size: " << SizeI( IMAGE_WIDTH, IMAGE_HEIGHT ) );

    LOG_INFO( "Cycles: " << ITERATIONS );

    std::vector< std::thread > threads;
    std::vector< std::unique_ptr< ThreadData > > threadData;

    //
    // std::allocator
    //

    addThread< std::allocator< uint8_t > >(
        threads, threadData, "std::allocator_0" );

    addThread< std::allocator< uint8_t > >(
        threads, threadData, "std::allocator_1" );

    /*std::allocator< int > a;
    std::allocator< int > b;
    b = std::move( a );*/

    //
    // cvl::core::AlignedAllocator
    //

    addThread< AlignedAllocator< uint8_t > >(
        threads, threadData, "cvl::core::AlignedAllocator_0" );

    addThread< AlignedAllocator< uint8_t > >(
        threads, threadData, "cvl::core::AlignedAllocator_1" );

#if defined( BUILD_WITH_TBB )

    //
    // tbb::tbb_allocator
    //

    addThread< tbb::tbb_allocator< uint8_t > >(
        threads, threadData, "tbb::tbb_allocator_0" );

    addThread< tbb::tbb_allocator< uint8_t > >(
        threads, threadData, "tbb::tbb_allocator_1" );

    //
    // tbb::cache_aligned_allocator
    //

    addThread< tbb::cache_aligned_allocator< uint8_t > >(
        threads, threadData, "tbb::cache_aligned_allocator_0" );

    addThread< tbb::cache_aligned_allocator< uint8_t > >(
        threads, threadData, "tbb::cache_aligned_allocator_1" );

    //
    // tbb::scalable_allocator
    //

    addThread< tbb::scalable_allocator< uint8_t > >(
        threads, threadData, "tbb::scalable_allocator_0" );

    addThread< tbb::scalable_allocator< uint8_t > >(
        threads, threadData, "tbb::scalable_allocator_1" );

    //
    // tbb::memory_pool_allocator
    //

    addThread< tbb::memory_pool_allocator< uint8_t > >(
        threads, threadData, "tbb::memory_pool_allocator_0" );

    addThread< tbb::memory_pool_allocator< uint8_t > >(
        threads, threadData, "tbb::memory_pool_allocator_1" );

#endif

    //
    // std::pmr::polymorphic_allocator
    //

    addThread< std::pmr::polymorphic_allocator< uint8_t > >(
        threads, threadData, "std::pmr::polymorphic_allocator_0" );

    addThread< std::pmr::polymorphic_allocator< uint8_t > >(
        threads, threadData, "std::pmr::polymorphic_allocator_1" );

    //
    // Add thread that do heap fragmentation
    //

    addFragmentationThread< std::allocator< DummyData > >(
        threads, threadData, "Fragmentation_0" );

    addFragmentationThread< std::allocator< DummyData > >(
        threads, threadData, "Fragmentation_1" );

    //
    //
    //

    int32_t key { };
    std::cout << "Press 'c' to cancel application\n";

    while ( key != 32 )
    {
#if defined( WIN32 )        
        key = _getch( );
#else
        key = getch( );
#endif

        if ( key == 'c' )
        {
            break;
        }
    }
    for ( size_t i = 0; i < threads.size( ); i++ )
    {
        threadData[ i ]->mRun = false;

        if ( threads[ i ].joinable( ) )
        {
            threads[ i ].join( );
        }
    }

    return 0;
}

#pragma warning( pop )