// CVL includes
#include <cvl/core/AlignedAllocator.h>
#include <cvl/core/macros.h>

// STD includes
#include <random>

// GTest includes
IGNORE_WARNINGS_GTEST_PUSH
#include <gtest/gtest.h>
// #include <gmock/gmock.h> // GTest Update required because of removed function
//  in C++20
IGNORE_WARNINGS_POP

//
// Typed tests
// https://google.github.io/googletest/advanced.html#typed-tests

using namespace cvl::core;
using testing::Eq;

template < typename T >
class TestCvlCoreAlignedAllocator : public testing::Test
{
public:
    using List = std::list< T >;
    static T shared_;
    T value_ { };

    CVL_DEFAULT_ONLY( TestCvlCoreAlignedAllocator );
};

using Types =
    testing::Types< int8_t, uint8_t, int16_t, uint16_t, int32_t, float >;

TYPED_TEST_SUITE(
    TestCvlCoreAlignedAllocator,
    Types ); // NOLINT(clang-diagnostic-gnu-zero-variadic-macro-arguments)

TYPED_TEST( TestCvlCoreAlignedAllocator, AllocatorNoThrow )
{
    EXPECT_NO_THROW( AlignedAllocator< TypeParam > allocator;
                     std::ignore = allocator; );
}

TYPED_TEST( TestCvlCoreAlignedAllocator, Allocate )
{
    size_t numElements = 5;
    AlignedAllocator< TypeParam > allocator;
    auto buffer = allocator.allocate( numElements );

    EXPECT_NE( buffer, nullptr );

    for ( size_t i = 0; i < numElements; i++ )
    {
        buffer[ i ] = static_cast< TypeParam >( i );
    }
}

TYPED_TEST( TestCvlCoreAlignedAllocator, DeallocateNoThrow )
{
    size_t numElements = 5;
    AlignedAllocator< TypeParam > allocator;
    auto buffer = allocator.allocate( numElements );

    EXPECT_NO_THROW( allocator.deallocate( buffer, numElements ) );
}

TYPED_TEST( TestCvlCoreAlignedAllocator, Construct )
{
    constexpr size_t numElements = 5;
    TypeParam data[ numElements ];

    for ( auto& i : data )
    {
        i = static_cast< TypeParam >( 0 );
    }

    AlignedAllocator< TypeParam > allocator;

    using allocator_traits =
        std::allocator_traits< AlignedAllocator< TypeParam > >;

    auto ptr = allocator_traits::allocate( allocator, numElements );

    for ( size_t i = 0; i < numElements; i++ )
    {
        allocator_traits::construct( allocator, ptr + i, TypeParam { 1 } );
    }

    for ( size_t i = 0; i < numElements; i++ )
    {
        EXPECT_EQ( ptr[ i ], TypeParam { 1 } );
    }
}

TYPED_TEST( TestCvlCoreAlignedAllocator, Destruct )
{
    constexpr size_t numElements = 5;
    TypeParam data[ numElements ];

    for ( auto& i : data )
    {
        i = static_cast< TypeParam >( 0 );
    }

    AlignedAllocator< TypeParam > allocator;

    using allocator_traits =
        std::allocator_traits< AlignedAllocator< TypeParam > >;

    auto ptr = allocator_traits::allocate( allocator, numElements );

    for ( size_t i = 0; i < numElements; i++ )
    {
        EXPECT_NO_THROW( allocator_traits::destroy( allocator, ptr + i ) );
    }
}

TYPED_TEST( TestCvlCoreAlignedAllocator, VectorAllocator )
{
    constexpr size_t numElements = 5;
    std::vector< TypeParam, AlignedAllocator< TypeParam > > container;

    container.resize( numElements );

    std::fill( container.begin( ), container.end( ), TypeParam( 5 ) );

    for ( size_t i = 0; i < numElements; i++ )
    {
        EXPECT_EQ( container[ i ], TypeParam( 5 ) );
    }

    container.clear( );

    container.reserve( numElements );

    for ( size_t i = 0; i < numElements; i++ )
    {
        container.push_back( TypeParam( 5 ) );
    }

    EXPECT_EQ( container.size( ), numElements );
}

TEST( TestCvlCoreAlignedAllocator, VectorAllocatorCustomType )
{
    struct TestRGB
    {
        TestRGB( )
        {
            R = 1;
            G = 2;
            B = 3;
        }

        uint8_t R { };
        uint8_t G { };
        uint8_t B { };
    };

    constexpr size_t numElements = 5;
    std::vector< TestRGB, AlignedAllocator< TestRGB > > container;

    container.resize( numElements );

    // std::fill( container.begin( ), container.end( ), TypeParam( 5 ) );

    for ( size_t i = 0; i < numElements; i++ )
    {
        EXPECT_EQ( container[ i ].R, uint8_t { 1 } );
        EXPECT_EQ( container[ i ].G, uint8_t { 2 } );
        EXPECT_EQ( container[ i ].B, uint8_t { 3 } );
    }

    container.clear( );

    container.reserve( numElements );

    for ( size_t i = 0; i < numElements; i++ )
    {
        container.push_back( TestRGB( ) );
    }

    EXPECT_EQ( container.size( ), numElements );
}

TYPED_TEST( TestCvlCoreAlignedAllocator, Check64ByteAligned )
{
    constexpr size_t BYTE_ALIGNMENT = 64;
    constexpr size_t numElements = 5;
    AlignedAllocator< TypeParam > allocator;
    auto buffer = allocator.allocate( numElements );

    const auto adr = reinterpret_cast< uintptr_t >( buffer ) % BYTE_ALIGNMENT;

    EXPECT_EQ( adr, 0 );
}