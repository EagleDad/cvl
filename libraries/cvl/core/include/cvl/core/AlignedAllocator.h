#pragma once

// Std includes
#include <cstddef>
#include <type_traits>

#if defined( WIN32 )
    #include <malloc.h>
#else
    #include <cstdio>
    #include <cstdlib>
#endif

namespace cvl::core
{

template < typename T, std::size_t Alignment = 64 >
class AlignedAllocator
{
public:
    // type definitions
    using value_type = T;
    using pointer = T*;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using propagate_on_container_move_assignment = std::true_type;
    using is_always_equal = std::true_type;

    static constexpr std::size_t alignment( ) { return Alignment; }

    // rebind allocator to type U
    // Will be removed with C++23
    template < class U >
    struct rebind
    {
        using other = AlignedAllocator< U >;
    };

    /* constructors and destructor
     * - nothing to do because the allocator has no state
     */
    constexpr AlignedAllocator( ) noexcept = default;

    constexpr AlignedAllocator( const AlignedAllocator& ) noexcept = default;

    template < class U >
    constexpr AlignedAllocator( const AlignedAllocator< U >& ) noexcept
    {
    }

    constexpr AlignedAllocator( AlignedAllocator&& ) noexcept = delete;

    constexpr ~AlignedAllocator( ) noexcept = default;

    AlignedAllocator& operator=( const AlignedAllocator& ) = default;

    AlignedAllocator& operator=( AlignedAllocator&& ) = delete;

    // deallocate storage p of deleted elements
    // ReSharper disable once CppMemberFunctionMayBeStatic
    void deallocate( pointer p, std::size_t num )
    {
        if ( num != 0 && p != nullptr )
        {
#if defined( WIN32 )
            _aligned_free( p );
#else
            std::free( p );
#endif
        }
    }

    // allocate but don't initialize num elements of type T
    // ReSharper disable once CppMemberFunctionMayBeStatic
    pointer allocate( std::size_t num )
    {
        if ( num > 0 )
        {
            void* ptr { };
#if defined( WIN32 )
            ptr = _aligned_malloc( num * sizeof( value_type ), Alignment );
#else
            ptr = std::aligned_alloc(1024, num * sizeof( value_type ));
#endif

            return static_cast< pointer >( ptr );
        }

        return nullptr;
    }
};

// return that all specializations of this allocator are interchangeable
template < class T1, class T2 >
bool operator==( const AlignedAllocator< T1 >&,
                 const AlignedAllocator< T2 >& ) noexcept
{
    return true;
}

} // namespace cvl::core