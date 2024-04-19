#pragma once

// STD includes

// CVL includes
#include <cvl/core/Point.h>
#include <cvl/core/Size.h>

namespace cvl::core
{
/**
 *  @brief Template implementation for a rectangle
 *
 */
template < Arithmetic T >
class Rectangle
{
public:
    using value_type = T;
    using reference_type = T&;
    using const_reference_type = const T&;

    /**
     * Default constructor
     *
     */
    Rectangle( ) = default;

    /**
     * Create a rectangle from top/left position and size
     *
     * @param [in]  topLeft   The top left position
     * @param [in]  size      The size
     */
    Rectangle( const Point< T, 2 >& topLeft, const Size< T >& size );

    /**
     * Copy constructor
     *
     * @param [in] other   The rect to copy from
     *
     */
    Rectangle( const Rectangle& other );

    /**
     * Move constructor
     *
     * @param [in]  other     The rect to move
     */
    Rectangle( Rectangle&& other ) noexcept;

    /**
     * Destructor
     *
     */
    ~Rectangle( ) = default;

    /**
     * Assignment operator
     *
     * @param [in]  other     The rect to assign from
     */
    constexpr Rectangle& operator=( const Rectangle& other ) noexcept;

    /**
     * Move operator
     *
     * @param [in]  other  The rect to move from
     */
    constexpr Rectangle& operator=( Rectangle&& other ) noexcept;

    /**
     * Equal operator
     *
     * @param [in]  other  The rect to compare
     */
    constexpr bool operator==( const Rectangle& other ) const noexcept;

    /**
     * Not operator
     *
     * @param [in]  other  The rect to compare
     */
    constexpr bool operator!=( const Rectangle& other ) const noexcept;

    /**
     * Add operator
     *
     * @param [in]  other  The rect to add
     */
    /*template < typename U >
    constexpr Rectangle< std::common_type_t< T, U > >
    operator+( const Rectangle< U >& other ) const noexcept;*/

    /**
     * Add operator
     *
     * @param [in]  other  The rect to add
     */
    //constexpr Rectangle& operator+=( const Rectangle& other ) noexcept;

    /**
     * Subtract operator
     *
     * @param [in]  other  The rect to subtract
     */
    /*template < typename U >
    constexpr Rectangle< std::common_type_t< T, U > >
    operator-( const Rectangle< U >& other ) const noexcept;*/

    /**
     * Subtract operator
     *
     * @param [in]  other  The rect to subtract
     */
    //constexpr Rectangle& operator-=( const Rectangle& other ) noexcept;

    /**
     * Scale operator
     *
     * @param [in]  scale  The scale factor
     */
    template < typename U >
    constexpr Rectangle< std::common_type_t< T, U > >
    operator*( U scale ) const noexcept;

    /**
     * Accessor size
     *
     * @returns The size of the rectangle
     */
    [[nodiscard]] constexpr Size< T > getSize( ) const;

    /**
     * Accessor top left
     *
     * @returns The top left coordinate of the rectangle
     */
    [[nodiscard]] constexpr Point< T, 2 > getTopLeft( ) const;

    /**
     * Accessor top right
     *
     * @returns The top right coordinate of the rectangle
     */
    [[nodiscard]] constexpr Point< T, 2 > getTopRight( ) const;

    /**
     * Accessor bottom left
     *
     * @returns The bottom left coordinate of the rectangle
     */
    [[nodiscard]] constexpr Point< T, 2 > getBottomLeft( ) const;

    /**
     * Accessor bottom right
     *
     * @returns The bottom right coordinate of the rectangle
     */
    [[nodiscard]] constexpr Point< T, 2 > getBottomRight( ) const;

    /**
     * Accessor left
     *
     * @returns The x coordinate of the left side
     */
    [[nodiscard]] constexpr T getLeft( ) const;

    /**
     * Accessor right
     *
     * @returns The x coordinate of the right side
     */
    [[nodiscard]] constexpr T getRight( ) const;

    /**
     * Accessor top
     *
     * @returns The y coordinate of the top side
     */
    [[nodiscard]] constexpr T getTop( ) const;

    /**
     * Accessor bottom
     *
     * @returns The y coordinate of the bottom side
     */
    [[nodiscard]] constexpr T getBottom( ) const;

    /**
     * Accessor width
     *
     * @returns The width of the rect
     */
    [[nodiscard]] constexpr T getWidth( ) const;

    /**
     * Accessor height
     *
     * @returns The height of the rect
     */
    [[nodiscard]] constexpr T getHeight( ) const;

private:
    void swap( Rectangle& other ) noexcept;

private:
    Point< T, 2 > mTopLeft;
    Size< T > mSize;
};

template < Arithmetic T >
Rectangle< T >::Rectangle( const Point< T, 2 >& topLeft, const Size< T >& size )
    : mTopLeft( topLeft )
    , mSize( size )
{
}

template < Arithmetic T >
Rectangle< T >::Rectangle( const Rectangle& other )
    : mTopLeft( other.mTopLeft )
    , mSize( other.mSize )
{
}

template < Arithmetic T >
Rectangle< T >::Rectangle( Rectangle&& other ) noexcept
    : mTopLeft( std::move( other.mTopLeft ) )
    , mSize( std::move( other.mSize ) )
{
}

template < Arithmetic T >
constexpr Rectangle< T >&
Rectangle< T >::operator=( const Rectangle& other ) noexcept
{
    if ( this != &other )
    {
        Rectangle tmp( other );
        tmp.swap( *this );
    }

    return *this;
}

template < Arithmetic T >
constexpr Rectangle< T >&
Rectangle< T >::operator=( Rectangle&& other ) noexcept
{
    if ( this != &other )
    {
        // Transfer ownership
        this->mTopLeft = std::move( other.mTopLeft );
        this->mSize = std::move( other.mSize );
    }

    return *this;
}

template < Arithmetic T >
constexpr bool
Rectangle< T >::operator==( const Rectangle& other ) const noexcept
{
    return this->mTopLeft == other.mTopLeft && this->mSize == other.mSize;
}

template < Arithmetic T >
constexpr bool
Rectangle< T >::operator!=( const Rectangle& other ) const noexcept
{
    return ! operator==( other );
}

//template < Arithmetic T >
//template < typename U >
//constexpr Rectangle< std::common_type_t< T, U > >
//Rectangle< T >::operator+( const Rectangle< U >& other ) const noexcept
//{
//    using common_type = std::common_type_t< T, U >;
//
//    const auto tl = this->mTopLeft + other.getTopLeft( );
//    const auto size = this->mSize + other.getSize( );
//
//    return Rectangle< common_type >( tl, size );
//}

//template < Arithmetic T >
//constexpr Rectangle< T >&
//Rectangle< T >::operator+=( const Rectangle& other ) noexcept
//{
//    this->mTopLeft += other.mTopLeft;
//    this->mSize += other.mSize;
//
//    return *this;
//}

//template < Arithmetic T >
//template < typename U >
//constexpr Rectangle< std::common_type_t< T, U > >
//Rectangle< T >::operator-( const Rectangle< U >& other ) const noexcept
//{
//    using common_type = std::common_type_t< T, U >;
//
//    const auto tl = this->mTopLeft - other.getTopLeft( );
//    const auto size = this->mSize - other.getSize( );
//
//    return Rectangle< common_type >( tl, size );
//}

//template < Arithmetic T >
//constexpr Rectangle< T >&
//Rectangle< T >::operator-=( const Rectangle& other ) noexcept
//{
//    this->mTopLeft -= other.mTopLeft;
//    this->mSize -= other.mSize;
//
//    return *this;
//}

template < Arithmetic T >
template < typename U >
constexpr Rectangle< std::common_type_t< T, U > >
Rectangle< T >::operator*( U scale ) const noexcept
{
    using common_type = std::common_type_t< T, U >;

    const auto tl = this->mTopLeft * scale;
    const auto size = this->mSize * scale;

    return Rectangle< common_type >( tl, size );
}

template < Arithmetic T >
constexpr Size< T > Rectangle< T >::getSize( ) const
{
    return mSize;
}

template < Arithmetic T >
constexpr Point< T, 2 > Rectangle< T >::getTopLeft( ) const
{
    return mTopLeft;
}

template < Arithmetic T >
constexpr Point< T, 2 > Rectangle< T >::getTopRight( ) const
{
    return { getRight( ), getTop( ) };
}

template < Arithmetic T >
constexpr Point< T, 2 > Rectangle< T >::getBottomLeft( ) const
{
    return { getLeft( ), getBottom( ) };
}

template < Arithmetic T >
constexpr Point< T, 2 > Rectangle< T >::getBottomRight( ) const
{
    return { getRight( ), getBottom( ) };
}

template < Arithmetic T >
constexpr T Rectangle< T >::getLeft( ) const
{
    return mTopLeft.getX( );
}

template < Arithmetic T >
constexpr T Rectangle< T >::getRight( ) const
{
    const auto offsetX = mSize.getWidth( ) > 0 ? mSize.getWidth( ) - 1 : 0;
    return static_cast< T >( mTopLeft.getX( ) + offsetX );
}

template < Arithmetic T >
constexpr T Rectangle< T >::getTop( ) const
{
    return mTopLeft.getY( );
}

template < Arithmetic T >
constexpr T Rectangle< T >::getBottom( ) const
{
    const auto offsetY = mSize.getHeight( ) > 0 ? mSize.getHeight( ) - 1 : 0;
    return static_cast< T >( mTopLeft.getY( ) + offsetY );
}

template < Arithmetic T >
constexpr T Rectangle< T >::getWidth( ) const
{
    return mSize.getWidth( );
}

template < Arithmetic T >
constexpr T Rectangle< T >::getHeight( ) const
{
    return mSize.getHeight( );
}

template < Arithmetic T >
void Rectangle< T >::swap( Rectangle& other ) noexcept
{
    std::swap( this->mTopLeft, other.mTopLeft );
    std::swap( this->mSize, other.mSize );
}

} // namespace cvl::core