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

    // CVL includes
    #include <cvl/core/Image.h>
    #include <cvl/core/Rectangle.h>
    #include <cvl/core/macros.h>

using namespace cvl::core;

    // Benchmark includes
    #include <benchmark/benchmark.h>

constexpr int32_t ImageWidth { 1024 };
constexpr int32_t ImageHeight { 1024 };

class ImageFixture : public benchmark::Fixture
{
public:
    CVL_DEFAULT_ONLY( ImageFixture );

    void SetUp( [[maybe_unused]] const ::benchmark::State& state ) override {}

    void TearDown( [[maybe_unused]] const ::benchmark::State& state ) override
    {
    }

    static constexpr int32_t numberElements( )
    {
        return ImageWidth * ImageHeight;
    }

private:
};

    #pragma warning( disable : 4365 )

static void BM_memcpy( benchmark::State& state )
{
    const auto src = new char[ state.range( 0 ) ];
    const auto dst = new char[ state.range( 0 ) ];
    memset( src, 'x', state.range( 0 ) );
    for ( auto _ : state ) memcpy( dst, src, state.range( 0 ) );
    state.SetBytesProcessed( static_cast< int64_t >( state.iterations( ) ) *
                             static_cast< int64_t >( state.range( 0 ) ) );

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
    ->Complexity( benchmark::oN );

static void BM_ImageCopy( benchmark::State& state )
{
    const auto size = static_cast< int32_t >( state.range( 0 ) );
    const Image< uint8_t, 1 > imgToCopy( size, size, false );

    for ( auto _ : state )
    {
        Image< uint8_t, 1 > imgCopy( imgToCopy );
    }

    const auto bytesProcesses = static_cast< int64_t >( state.iterations( ) ) *
                                static_cast< int64_t >( size * size );

    state.SetBytesProcessed( bytesProcesses );

    state.SetComplexityN( size * size );
}

BENCHMARK( BM_ImageCopy )
    ->Arg( 256 )
    ->Arg( 512 )
    ->Arg( 1024 )
    ->Arg( 2048 )
    ->Arg( 4096 )
    ->Complexity( benchmark::oN );

static void BM_ImageCreateZeroInit( benchmark::State& state )
{
    const auto size = static_cast< int32_t >( state.range( 0 ) );

    for ( auto _ : state )
    {
        Image< uint8_t, 1 > imgCopy( size, size, true );
    }

    state.SetComplexityN( size * size );
}
BENCHMARK( BM_ImageCreateZeroInit )
    ->Arg( 256 )
    ->Arg( 512 )
    ->Arg( 1024 )
    ->Arg( 2048 )
    ->Arg( 4096 )
    ->Complexity( benchmark::oN );

static void BM_ImageCreateNoZeroInit( benchmark::State& state )
{
    const auto size = static_cast< int32_t >( state.range( 0 ) );

    for ( auto _ : state )
    {
        Image< uint8_t, 1 > imgCopy( size, size, false );
    }

    state.SetComplexityN( size * size );
}
BENCHMARK( BM_ImageCreateNoZeroInit )
    ->Arg( 256 )
    ->Arg( 512 )
    ->Arg( 1024 )
    ->Arg( 2048 )
    ->Arg( 4096 )
    ->Complexity( benchmark::oN );

// static void BM_ImageSetTo( benchmark::State& state )
//{
//     const auto size = static_cast< int32_t >( state.range( 0 ) );
//
//     constexpr uint8_t val = uint8_t { 128 };
//
//     for ( auto _ : state )
//     {
//         Image< uint8_t, 1 > imgCopy( size, size, val );
//     }
//
//     state.SetComplexityN( size * size );
// }
// BENCHMARK( BM_ImageSetTo )
//     ->Arg( 256 )
//     ->Arg( 512 )
//     ->Arg( 1024 )
//     ->Arg( 2048 )
//     ->Arg( 4096 )
//     ->Complexity( benchmark::oN );

static void BM_ImageCopyTo( benchmark::State& state )
{
    const auto size = static_cast< int32_t >( state.range( 0 ) );

    // constexpr uint8_t val = uint8_t { 128 };

    const Image< uint8_t, 1 > image( size, size, false );

    for ( auto _ : state )
    {
        Image< uint8_t, 1 > imgCopy;
        image.copyTo( imgCopy );
    }

    state.SetComplexityN( size * size );
}
BENCHMARK( BM_ImageCopyTo )
    ->Arg( 256 )
    ->Arg( 512 )
    ->Arg( 1024 )
    ->Arg( 2048 )
    ->Arg( 4096 )
    ->Complexity( benchmark::oN );

static void BM_ImageGetRoi( benchmark::State& state )
{
    const auto size = static_cast< int32_t >( state.range( 0 ) );
    const auto top = static_cast< int32_t >( state.range( 1 ) );
    const auto left = static_cast< int32_t >( state.range( 1 ) );
    const auto width = static_cast< int32_t >( state.range( 2 ) );
    const auto height = static_cast< int32_t >( state.range( 2 ) );

    const Rectangle< int32_t > roi( Point2i { left, top }, { width, height } );

    // constexpr uint8_t val = uint8_t { 128 };

    const Image< uint8_t, 1 > image( size, size, false );

    for ( auto _ : state )
    {
        const Image< uint8_t, 1 > imageRoi = image( roi );
    }

    state.SetComplexityN( width * height );
}
BENCHMARK( BM_ImageGetRoi )
    ->Args( { 256, 64, 128 } )
    ->Args( { 512, 128, 256 } )
    ->Args( { 1024, 256, 512 } )
    ->Args( { 2048, 512, 1024 } )
    ->Args( { 4096, 1024, 2048 } )
    ->Complexity( benchmark::oN );

static void BM_ImageCopyRoi( benchmark::State& state )
{
    const auto size = static_cast< int32_t >( state.range( 0 ) );
    const auto top = static_cast< int32_t >( state.range( 1 ) );
    const auto left = static_cast< int32_t >( state.range( 1 ) );
    const auto width = static_cast< int32_t >( state.range( 2 ) );
    const auto height = static_cast< int32_t >( state.range( 2 ) );

    const Rectangle< int32_t > roi( Point2i { left, top }, { width, height } );

    //constexpr uint8_t val = uint8_t { 128 };

    const Image< uint8_t, 1 > image( size, size, false );

    for ( auto _ : state )
    {
        const Image< uint8_t, 1 > imageRoi = image( roi );
        const Image< uint8_t, 1 > imageRoiCopy( imageRoi );
    }

    const auto bytesProcesses = static_cast< int64_t >( state.iterations( ) ) *
                                static_cast< int64_t >( width * height );

    state.SetBytesProcessed( bytesProcesses );

    state.SetComplexityN( width * height );
}
BENCHMARK( BM_ImageCopyRoi )
    ->Args( { 256, 64, 128 } )
    ->Args( { 512, 128, 256 } )
    ->Args( { 1024, 256, 512 } )
    ->Args( { 2048, 512, 1024 } )
    ->Args( { 4096, 1024, 2048 } )
    ->Complexity( benchmark::oN );

static void BM_ImageClone( benchmark::State& state )
{
    const auto size = static_cast< int32_t >( state.range( 0 ) );
    const Image< uint8_t, 1 > imgToCopy( size, size, false );

    for ( auto _ : state )
    {
        Image< uint8_t, 1 > imgCopy = imgToCopy.clone( );
    }

    const auto bytesProcesses = static_cast< int64_t >( state.iterations( ) ) *
                                static_cast< int64_t >( size * size );

    state.SetBytesProcessed( bytesProcesses );

    state.SetComplexityN( size * size );
}

BENCHMARK( BM_ImageClone )
    ->Arg( 256 )
    ->Arg( 512 )
    ->Arg( 1024 )
    ->Arg( 2048 )
    ->Arg( 4096 )
    ->Complexity( benchmark::oN );

BENCHMARK_MAIN( );

#endif
