// NOTE: BENCHMARK ONLY WORKS IN RELEASE

#if defined( _DEBUG )

    #include <iostream>

int main( )
{
    std::cout << "BENCHMARK IS NOT AVAILABLE IN DEBUG MODE" << std::endl;
    return 0;
}

#else

    #pragma warning( disable : 4266 )
    #pragma warning( disable : 4625 )
    #pragma warning( disable : 5026 )
    #pragma warning( disable : 4626 )
    #pragma warning( disable : 5027 )
    #pragma warning( disable : 4868 )

// CVL includes
    #include <cvl/core/AlignedAllocator.h>
    #include <cvl/core/Image.h>
    #include <cvl/core/Rectangle.h>
    #include <cvl/core/macros.h>

    // TBB includes
    #define TBB_PREVIEW_MEMORY_POOL 1
    #include <oneapi/tbb/cache_aligned_allocator.h>
    #include <oneapi/tbb/memory_pool.h>
    #include <oneapi/tbb/scalable_allocator.h>
    #include <oneapi/tbb/tbbmalloc_proxy.h>
    // #include <oneapi/tbb.h>
    #include <oneapi/tbb/memory_pool.h>
    #include <oneapi/tbb/tbb_allocator.h>

using namespace cvl::core;

constexpr auto numThreads = 8;

    // Benchmark includes
    #include <benchmark/benchmark.h>

class ImageFixture : public benchmark::Fixture
{
public:
    CVL_DEFAULT_ONLY( ImageFixture );

    void SetUp( [[maybe_unused]] const ::benchmark::State& state ) override {}

    void TearDown( [[maybe_unused]] const ::benchmark::State& state ) override
    {
    }

private:
};

    #pragma warning( disable : 4365 )

static void BM_memcpy( benchmark::State& state )
{
    char* src = new char[ state.range( 0 ) ];
    char* dst = new char[ state.range( 0 ) ];
    memset( src, 'x', state.range( 0 ) );
    for ( auto _ : state ) memcpy( dst, src, state.range( 0 ) );
    state.SetBytesProcessed( int64_t( state.iterations( ) ) *
                             int64_t( state.range( 0 ) ) );

    state.SetComplexityN( state.range( 0 ) );

    delete[] src;
    delete[] dst;
}
BENCHMARK( BM_memcpy )
    ->Arg( 8 )
    ->Arg( 64 )
    ->Arg( 512 )
    ->Arg( 4 << 10 )
    ->Arg( 8 << 10 )
    ->Complexity( benchmark::oN )
    ->UseRealTime( )
    ->Threads( numThreads );

//
// std::allocator
//

static void BM_ImageCopyStdAllocator( benchmark::State& state )
{
    const auto size = static_cast< int32_t >( state.range( 0 ) );
    const Image< uint8_t, 1, std::allocator< uint8_t > > imgToCopy(
        size, size, false );

    for ( auto _ : state )
    {
        Image< uint8_t, 1, std::allocator< uint8_t > > imgCopy( imgToCopy );
    }

    const auto bytesProcesses = static_cast< int64_t >( state.iterations( ) ) *
                                static_cast< int64_t >( size * size );

    state.SetBytesProcessed( bytesProcesses );

    state.SetComplexityN( size * size );
}

BENCHMARK( BM_ImageCopyStdAllocator )
    ->Arg( 256 )
    ->Arg( 512 )
    ->Arg( 1024 )
    ->Arg( 2048 )
    ->Arg( 4096 )
    ->Complexity( benchmark::oN )
    ->UseRealTime( )
    ->Threads( numThreads );

//
// AlignedAllocator
//

static void BM_ImageCopyAlignedAllocator( benchmark::State& state )
{
    const auto size = static_cast< int32_t >( state.range( 0 ) );
    const Image< uint8_t, 1, AlignedAllocator< uint8_t > > imgToCopy(
        size, size, false );

    for ( auto _ : state )
    {
        Image< uint8_t, 1, AlignedAllocator< uint8_t > > imgCopy( imgToCopy );
    }

    const auto bytesProcesses = static_cast< int64_t >( state.iterations( ) ) *
                                static_cast< int64_t >( size * size );

    state.SetBytesProcessed( bytesProcesses );

    state.SetComplexityN( size * size );
}

BENCHMARK( BM_ImageCopyAlignedAllocator )
    ->Arg( 256 )
    ->Arg( 512 )
    ->Arg( 1024 )
    ->Arg( 2048 )
    ->Arg( 4096 )
    ->Complexity( benchmark::oN )
    ->UseRealTime( )
    ->Threads( numThreads );

//
// tbb::tbb_allocator
//

static void BM_ImageCopyTbbAllocator( benchmark::State& state )
{
    const auto size = static_cast< int32_t >( state.range( 0 ) );
    const Image< uint8_t, 1, tbb::tbb_allocator< uint8_t > > imgToCopy(
        size, size, false );

    for ( auto _ : state )
    {
        Image< uint8_t, 1, tbb::tbb_allocator< uint8_t > > imgCopy( imgToCopy );
    }

    const auto bytesProcesses = static_cast< int64_t >( state.iterations( ) ) *
                                static_cast< int64_t >( size * size );

    state.SetBytesProcessed( bytesProcesses );

    state.SetComplexityN( size * size );
}

BENCHMARK( BM_ImageCopyTbbAllocator )
    ->Arg( 256 )
    ->Arg( 512 )
    ->Arg( 1024 )
    ->Arg( 2048 )
    ->Arg( 4096 )
    ->Complexity( benchmark::oN )
    ->UseRealTime( )
    ->Threads( numThreads );

//
// tbb::cache_aligned_allocator
//

static void BM_ImageCopyTbbCacheAlignedAllocator( benchmark::State& state )
{
    const auto size = static_cast< int32_t >( state.range( 0 ) );
    const Image< uint8_t, 1, tbb::cache_aligned_allocator< uint8_t > >
        imgToCopy( size, size, false );

    for ( auto _ : state )
    {
        Image< uint8_t, 1, tbb::cache_aligned_allocator< uint8_t > > imgCopy(
            imgToCopy );
    }

    const auto bytesProcesses = static_cast< int64_t >( state.iterations( ) ) *
                                static_cast< int64_t >( size * size );

    state.SetBytesProcessed( bytesProcesses );

    state.SetComplexityN( size * size );
}

BENCHMARK( BM_ImageCopyTbbCacheAlignedAllocator )
    ->Arg( 256 )
    ->Arg( 512 )
    ->Arg( 1024 )
    ->Arg( 2048 )
    ->Arg( 4096 )
    ->Complexity( benchmark::oN )
    ->UseRealTime( )
    ->Threads( numThreads );

//
// tbb::scalable_allocator
//

static void BM_ImageCopyTbbScalableAllocator( benchmark::State& state )
{
    const auto size = static_cast< int32_t >( state.range( 0 ) );
    const Image< uint8_t, 1, tbb::scalable_allocator< uint8_t > > imgToCopy(
        size, size, false );

    for ( auto _ : state )
    {
        Image< uint8_t, 1, tbb::scalable_allocator< uint8_t > > imgCopy(
            imgToCopy );
    }

    const auto bytesProcesses = static_cast< int64_t >( state.iterations( ) ) *
                                static_cast< int64_t >( size * size );

    state.SetBytesProcessed( bytesProcesses );

    state.SetComplexityN( size * size );
}

BENCHMARK( BM_ImageCopyTbbScalableAllocator )
    ->Arg( 256 )
    ->Arg( 512 )
    ->Arg( 1024 )
    ->Arg( 2048 )
    ->Arg( 4096 )
    ->Complexity( benchmark::oN )
    ->UseRealTime( )
    ->Threads( numThreads );

//
// tbb::memory_pool_allocator
//

static void BM_ImageCopyTbbMemoryPoolAllocator( benchmark::State& state )
{
    using memory_pool = tbb::memory_pool< tbb::tbb_allocator< uint8_t > >;
    using pool_allocator = tbb::memory_pool_allocator< uint8_t >;
    memory_pool tbbPool;

    Image< uint8_t, 1, pool_allocator > imgPool { pool_allocator( tbbPool ) };

    const auto size = static_cast< int32_t >( state.range( 0 ) );
    const Image< uint8_t, 1, tbb::memory_pool_allocator< uint8_t > > imgToCopy(
        size, size, false, pool_allocator( tbbPool ) );

    for ( auto _ : state )
    {
        Image< uint8_t, 1, tbb::memory_pool_allocator< uint8_t > > imgCopy(
            imgToCopy );
    }

    const auto bytesProcesses = static_cast< int64_t >( state.iterations( ) ) *
                                static_cast< int64_t >( size * size );

    state.SetBytesProcessed( bytesProcesses );

    state.SetComplexityN( size * size );
}

BENCHMARK( BM_ImageCopyTbbMemoryPoolAllocator )
    ->Arg( 256 )
    ->Arg( 512 )
    ->Arg( 1024 )
    ->Arg( 2048 )
    ->Arg( 4096 )
    ->Complexity( benchmark::oN )
    ->UseRealTime( )
    ->Threads( numThreads );

//
// std::pmr::polymorphic_allocator
//

static void BM_ImageCopyStdPmrAllocator( benchmark::State& state )
{
    const auto size = static_cast< int32_t >( state.range( 0 ) );

    std::vector< uint8_t > poolBuffer( 40 * size * size );

    std::pmr::monotonic_buffer_resource underlyingBytes(
        poolBuffer.data( ),
        poolBuffer.size( ),
        std::pmr::null_memory_resource( ) );

    std::pmr::pool_options options;
    options.largest_required_pool_block = size * size;
    options.max_blocks_per_chunk = 4;
    std::pmr::unsynchronized_pool_resource unsynchronizedPool {
        options, &underlyingBytes };

    const Image< uint8_t, 1, std::pmr::polymorphic_allocator< uint8_t > >
        imgToCopy(
            size,
            size,
            false,
            std::pmr::polymorphic_allocator< uint8_t >( &unsynchronizedPool ) );

    for ( auto _ : state )
    {
        Image< uint8_t, 1, std::pmr::polymorphic_allocator< uint8_t > > imgCopy(
            imgToCopy );
    }

    const auto bytesProcesses = static_cast< int64_t >( state.iterations( ) ) *
                                static_cast< int64_t >( size * size );

    state.SetBytesProcessed( bytesProcesses );

    state.SetComplexityN( size * size );
}

BENCHMARK( BM_ImageCopyStdPmrAllocator )
    ->Arg( 256 )
    ->Arg( 512 )
    ->Arg( 1024 )
    ->Arg( 2048 )
    ->Arg( 4096 )
    ->Complexity( benchmark::oN )
    ->UseRealTime( )
    ->Threads( 1 );

//
// std::pmr::polymorphic_allocator MT
//

static void BM_ImageCopyStdPmrAllocatorMt( benchmark::State& state )
{
    const auto size = static_cast< int32_t >( state.range( 0 ) );

    std::vector< uint8_t > poolBuffer( 40 * size * size );

    std::pmr::monotonic_buffer_resource underlyingBytes(
        poolBuffer.data( ),
        poolBuffer.size( ),
        std::pmr::null_memory_resource( ) );

    std::pmr::pool_options options;
    options.largest_required_pool_block = size * size;
    options.max_blocks_per_chunk = 4;
    std::pmr::unsynchronized_pool_resource unsynchronizedPool {
        options, &underlyingBytes };

    const Image< uint8_t, 1, std::pmr::polymorphic_allocator< uint8_t > >
        imgToCopy(
            size,
            size,
            false,
            std::pmr::polymorphic_allocator< uint8_t >( &unsynchronizedPool ) );

    for ( auto _ : state )
    {
        Image< uint8_t, 1, std::pmr::polymorphic_allocator< uint8_t > > imgCopy(
            imgToCopy );
    }

    const auto bytesProcesses = static_cast< int64_t >( state.iterations( ) ) *
                                static_cast< int64_t >( size * size );

    state.SetBytesProcessed( bytesProcesses );

    state.SetComplexityN( size * size );
}

BENCHMARK( BM_ImageCopyStdPmrAllocatorMt )
    ->Arg( 256 )
    ->Arg( 512 )
    ->Arg( 1024 )
    ->Arg( 2048 )
    ->Arg( 4096 )
    ->Complexity( benchmark::oN )
    ->UseRealTime( )
    ->Threads( numThreads );

BENCHMARK_MAIN( );

#endif
