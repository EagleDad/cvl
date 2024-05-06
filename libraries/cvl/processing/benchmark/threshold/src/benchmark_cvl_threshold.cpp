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


class ThresholdFixture : public benchmark::Fixture
{
public:
    CVL_DEFAULT_ONLY( ThresholdFixture );

    void SetUp( [[maybe_unused]] const ::benchmark::State& state ) override {}

    void TearDown( [[maybe_unused]] const ::benchmark::State& state ) override
    {
    }

private:
};

    #pragma warning( disable : 4365 )


static void BM_FixThreshold( benchmark::State& state )
{
    /*const auto size = static_cast< int32_t >( state.range( 0 ) );
    const Image< uint8_t, 1 > imgToCopy( size, size, false );

    for ( auto _ : state )
    {
        Image< uint8_t, 1 > imgCopy( imgToCopy );
    }

    const auto bytesProcesses = static_cast< int64_t >( state.iterations( ) ) *
                                static_cast< int64_t >( size * size );

    state.SetBytesProcessed( bytesProcesses );

    state.SetComplexityN( size * size );*/
}

BENCHMARK( BM_FixThreshold )
    ->Arg( 256 )
    ->Arg( 512 )
    ->Arg( 1024 )
    ->Arg( 2048 )
    ->Arg( 4096 )
    ->Complexity( benchmark::oN );


#endif
