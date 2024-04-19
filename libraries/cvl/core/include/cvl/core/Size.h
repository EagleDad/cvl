#pragma once

// STD includes

// CVL includes
#include "Point.h"

#include <cvl/core/Types.h>

namespace cvl::core
{
/**
 * @brief Template implementation for a size object
 *
 *  Width describes the size in x direction
 *  Height describes the size in y direction
 *
 */
template < Arithmetic T >
class Size
{
public:
    using value_type = T;
    using reference_type = T&;
    using const_reference_type = const T&;

    /**
     * Default constructor
     *
     */
    Size( ) = default;

    /**
     * Create a Size from width and height
     *
     * @param [in]  width       The width of the rectangle
     * @param [in]  height      The height of the rectangle
     */
    Size( const T& width, const T& height );

    /**
     * Copy constructor
     *
     * @param [in] other   The object to copy from
     *
     */
    Size( const Size& other );

    /**
     * Move constructor
     *
     * @param [in]  other     The object to move
     */
    Size( Size&& other ) noexcept;

    /**
     * Destructor
     *
     */
    ~Size( ) = default;

    /**
     * Assignment operator
     *
     * @param [in]  other     The size to assign from
     */
    constexpr Size& operator=( const Size& other ) noexcept;

    /**
     * Move operator
     *
     * @param [in]  other  The size to move from
     */
    constexpr Size& operator=( Size&& other ) noexcept;

    /**
     * Equal operator
     *
     * @param [in]  other  The size to compare
     */
    constexpr bool operator==( const Size& other ) const noexcept;

    /**
     * Not equal operator
     *
     * @param [in]  other  The size to compare with
     */
    constexpr bool operator!=( const Size& other ) const noexcept;

    /**
     * Add operator
     *
     * @param [in]  other  The size to add
     */
    template < typename U >
    constexpr Size< std::common_type_t< T, U > >
    operator+( const Size< U >& other ) const noexcept;

    /**
     * Add operator
     *
     * @param [in]  other  The size to add
     */
    constexpr Size& operator+=( const Size& other ) noexcept;

    /**
     * Subtract operator
     *
     * @param [in]  other  The size to subtract
     */
    template < typename U >
    constexpr Size< std::common_type_t< T, U > >
    operator-( const Size< U >& other ) const noexcept;

    /**
     * Subtract operator
     *
     * @param [in]  other  The size to subtract
     */
    constexpr Size& operator-=( const Size& other ) noexcept;

    /**
     * Scale operator
     *
     * @param [in]  scale  The scale factor
     */
    template < typename U >
    constexpr Size< std::common_type_t< T, U > >
    operator*( U scale ) const noexcept;

    /**
     * Accessor width
     *
     * @returns The width
     */
    [[nodiscard]] constexpr T getWidth( ) const;

    /**
     * Accessor height
     *
     *
     * @returns height
     */
    [[nodiscard]] constexpr T getHeight( ) const;

private:
    void swap( Size& other ) noexcept;

private:
    value_type mWidth { };
    value_type mHeight { };
};

template < Arithmetic T >
Size< T >::Size( const T& width, const T& height )
    : mWidth( width )
    , mHeight( height )
{
}

template < Arithmetic T >
Size< T >::Size( const Size& other )
    : mWidth( other.mWidth )
    , mHeight( other.mHeight )
{
}

template < Arithmetic T >
Size< T >::Size( Size&& other ) noexcept
    : mWidth( std::move( other.mWidth ) )
    , mHeight( std::move( other.mHeight ) )
{
}

template < Arithmetic T >
constexpr Size< T >& Size< T >::operator=( const Size& other ) noexcept
{
    if ( this != &other )
    {
        Size tmp( other );
        tmp.swap( *this );
    }

    return *this;
}

template < Arithmetic T >
constexpr Size< T >& Size< T >::operator=( Size&& other ) noexcept
{
    if ( this != &other )
    {
        // Transfer ownership
        this->mWidth = std::move( other.mWidth );
        this->mHeight = std::move( other.mHeight );
    }

    return *this;
}

template < Arithmetic T >
constexpr bool Size< T >::operator==( const Size& other ) const noexcept
{
    // TODO: Use a compare function since floating point compare might not work
    // in small cases
    return this->mWidth == other.mWidth && this->mHeight == other.mHeight;
}

template < Arithmetic T >
constexpr bool Size< T >::operator!=( const Size& other ) const noexcept
{
    return ! operator==( other );
}

template < Arithmetic T >
template < typename U >
constexpr Size< std::common_type_t< T, U > >
Size< T >::operator+( const Size< U >& other ) const noexcept
{
    using common_type = std::common_type_t< T, U >;

    common_type w = static_cast< common_type >( this->mWidth ) +
                    static_cast< common_type >( other.getWidth( ) );

    common_type h = static_cast< common_type >( this->mHeight ) +
                    static_cast< common_type >( other.getHeight( ) );

    return Size< common_type >( w, h );
}

template < Arithmetic T >
constexpr Size< T >& Size< T >::operator+=( const Size& other ) noexcept
{
    this->mWidth += other.mWidth;
    this->mHeight += other.mHeight;

    return *this;
}

template < Arithmetic T >
template < typename U >
constexpr Size< std::common_type_t< T, U > >
Size< T >::operator-( const Size< U >& other ) const noexcept
{
    using common_type = std::common_type_t< T, U >;

    common_type w = static_cast< common_type >( this->mWidth ) -
                    static_cast< common_type >( other.getWidth( ) );

    common_type h = static_cast< common_type >( this->mHeight ) -
                    static_cast< common_type >( other.getHeight( ) );

    return Size< common_type >( w, h );
}

template < Arithmetic T >
constexpr Size< T >& Size< T >::operator-=( const Size& other ) noexcept
{
    this->mWidth -= other.mWidth;
    this->mHeight -= other.mHeight;

    return *this;
}

template < Arithmetic T >
template < typename U >
constexpr Size< std::common_type_t< T, U > >
Size< T >::operator*( U scale ) const noexcept
{
    using common_type = std::common_type_t< T, U >;

    common_type w = static_cast< common_type >( this->mWidth ) * scale;

    common_type h = static_cast< common_type >( this->mHeight ) * scale;

    return Size< common_type >( w, h );
}

template < Arithmetic T >
constexpr T Size< T >::getWidth( ) const
{
    return mWidth;
}

template < Arithmetic T >
constexpr T Size< T >::getHeight( ) const
{
    return mHeight;
}

template < Arithmetic T >
void Size< T >::swap( Size& other ) noexcept
{
    std::swap( this->mWidth, other.mWidth );
    std::swap( this->mHeight, other.mHeight );
}

template < Arithmetic T >
std::ostream& operator<<( std::ostream& os, const Size< T >& size )
{
    return os << "(w: " << size.getWidth( ) << ", h: " << size.getHeight( )
              << ")";
}

using SizeD = Size< double >;
using SizeF = Size< float >;
using SizeI = Size< int32_t >;

} // namespace cvl::core