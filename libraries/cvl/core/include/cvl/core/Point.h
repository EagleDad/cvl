#pragma once

// STD includes
#include <algorithm>
#include <array>
#include <ostream>

// CVL includes
#include <cvl/core/Types.h>

namespace cvl::core
{
/**
 * @brief Template implementation for a generic n Dimensional point
 *
 */
template < Arithmetic T, int32_t Dimension >
class Point
{
public:
    using value_type = T;
    using reference_type = T&;
    using const_reference_type = const T&;

    using iterator = typename std::array< T, Dimension >::iterator;
    using const_iterator = typename std::array< T, Dimension >::const_iterator;

    /**
     * Default constructor
     *
     */
    Point( ) = default;

    /**
     * Value constructor
     *
     */
    template < typename... TArgs >
    explicit Point( TArgs... args );

    /**
     * Create a point from a std array
     *
     * @param [in]  coordinates  The array to create from
     */
    explicit Point( const std::array< T, Dimension >& coordinates );

    /**
     * Copy constructor
     *
     * @param [in]  other     The point to copy from
     */
    Point( const Point& other );

    /**
     * Move constructor
     *
     * @param [in]  other     The point to move
     */
    Point( Point&& other ) noexcept;

    /**
     * Destructor
     *
     */
    ~Point( ) = default;

    /**
     * Assignment operator
     *
     * @param [in]  other     The point to assign from
     */
    constexpr Point& operator=( const Point& other ) noexcept;

    /**
     * Move operator
     *
     * @param [in]  other  The point to move from
     */
    constexpr Point& operator=( Point&& other ) noexcept;

    /**
     * Equal operator
     *
     * @param [in]  other  The point to compare
     */
    constexpr bool operator==( const Point& other ) const noexcept;

    /**
     * Not equal operator
     *
     * @param [in]  other  The point to compare with
     */
    constexpr bool operator!=( const Point& other ) const noexcept;

    /**
     * Add operator
     *
     * @param [in]  other  The point to add
     */
    template < typename U >
    constexpr Point< std::common_type_t< T, U >, Dimension >
    operator+( const Point< U, Dimension >& other ) const noexcept;

    /**
     * Add operator
     *
     * @param [in]  other  The point to add
     */
    constexpr Point& operator+=( const Point& other ) noexcept;

    /**
     * Subtract operator
     *
     * @param [in]  other  The point to subtract
     */
    template < typename U >
    constexpr Point< std::common_type_t< T, U >, Dimension >
    operator-( const Point< U, Dimension >& other ) const noexcept;

    /**
     * Subtract operator
     *
     * @param [in]  other  The point to subtract
     */
    constexpr Point& operator-=( const Point& other ) noexcept;

    /**
     * Scale operator
     *
     * @param [in]  scale  The scale factor
     */
    template < typename U >
    constexpr Point< std::common_type_t< T, U >, Dimension >
    operator*( U scale ) const noexcept;

    /**
     * Scale operator
     *
     * @param [in]  scale  The scale factor
     */
    template < typename U >
    constexpr Point& operator*=( U scale ) noexcept;

    /**
     * Accessor X
     *
     * @returns The x value
     */
    template < int32_t NDim = Dimension,
               typename = std::enable_if_t< NDim >= 1 > >
    [[nodiscard]] constexpr T getX( ) const;

    /**
     * Accessor Y
     *
     * @returns The y value
     */
    template < int32_t NDim = Dimension,
               typename = std::enable_if_t< NDim >= 2 > >
    [[nodiscard]] constexpr T getY( ) const;

    /**
     * Accessor Z
     *
     * @returns The z value
     */
    template < int32_t NDim = Dimension,
               typename = std::enable_if_t< NDim >= 3 > >
    [[nodiscard]] constexpr T getZ( ) const;

    /**
     * Accessor at index
     *
     * @param [in]  index  The index to access
     *
     * @returns The value at index
     */
    constexpr reference_type at( size_t index );

    /**
     * Accessor at index
     *
     * @param [in]  index  The index to access
     *
     * @returns The value at index
     */
    [[nodiscard]] constexpr const_reference_type at( size_t index ) const;

    /**
     * Accessor at index
     *
     * @param [in]  index  The index to access
     *
     * @returns The value at index
     */
    constexpr reference_type operator[]( size_t index );

    /**
     * Accessor at index
     *
     * @param [in]  index  The index to access
     *
     * @returns The value at index
     */
    constexpr const_reference_type operator[]( size_t index ) const;

    /**
     * Accessor for coordinates
     *
     * @returns the coordinate array
     */
    constexpr std::array< T, Dimension >& coordinates( );

    /**
     * Accessor for coordinates
     *
     * @returns the coordinate array
     */
    [[nodiscard]] constexpr std::array< T, Dimension > coordinates( ) const;

    /**
     * Iterator to the begin
     *
     * @returns The iterator to the begin of the coordinate array
     */
    constexpr iterator begin( ) noexcept;

    /**
     * Iterator to the begin
     *
     * @returns The const iterator to the begin of the coordinate array
     */
    [[nodiscard]] constexpr const_iterator begin( ) const noexcept;

    /**
     * Iterator to the end
     *
     * @returns The iterator to the end of the coordinate array
     */
    constexpr iterator end( ) noexcept;

    /**
     * Iterator to the end
     *
     * @returns The const iterator to the end of the coordinate array
     */
    [[nodiscard]] constexpr const_iterator end( ) const noexcept;

private:
    void swap( Point& other ) noexcept;

private:
    std::array< T, Dimension > mCoordinates { };
};

template < Arithmetic T, int32_t Dimension >
template < typename... TArgs >
Point< T, Dimension >::Point( TArgs... args )
    : mCoordinates( { { args... } } )
{
}

template < Arithmetic T, int32_t Dimension >
Point< T, Dimension >::Point( const std::array< T, Dimension >& coordinates )
    : mCoordinates( coordinates )
{
}

template < Arithmetic T, int32_t Dimension >
Point< T, Dimension >::Point( const Point& other )
    : mCoordinates( other.mCoordinates )
{
}

template < Arithmetic T, int32_t Dimension >
Point< T, Dimension >::Point( Point&& other ) noexcept
    : mCoordinates( std::move( other.mCoordinates ) )
{
}

template < Arithmetic T, int32_t Dimension >
constexpr Point< T, Dimension >&
Point< T, Dimension >::operator=( const Point& other ) noexcept
{
    if ( this != &other )
    {
        Point tmp( other );
        tmp.swap( *this );
    }

    return *this;
}

template < Arithmetic T, int32_t Dimension >
constexpr Point< T, Dimension >&
Point< T, Dimension >::operator=( Point&& other ) noexcept
{
    if ( this != &other )
    {
        // Transfer ownership
        this->mCoordinates = std::move( other.mCoordinates );
    }

    return *this;
}

template < Arithmetic T, int32_t Dimension >
constexpr bool
Point< T, Dimension >::operator==( const Point& other ) const noexcept
{
    // TODO: Use a compare function since floating point compare might not work
    // in small cases
    return this->mCoordinates == other.mCoordinates;
}

template < Arithmetic T, int32_t Dimension >
constexpr bool
Point< T, Dimension >::operator!=( const Point& other ) const noexcept
{
    return ! operator==( other );
}

template < Arithmetic T, int32_t Dimension >
template < typename U >
constexpr Point< std::common_type_t< T, U >, Dimension >
Point< T, Dimension >::operator+(
    const Point< U, Dimension >& other ) const noexcept
{
    using common_type = std::common_type_t< T, U >;

    std::array< common_type, Dimension > newCoordinates { };

    std::transform( mCoordinates.begin( ),
                    mCoordinates.end( ),
                    other.begin( ),
                    newCoordinates.begin( ),
                    std::plus< std::common_type_t< T, U > >( ) );

    return Point< common_type, Dimension >( newCoordinates );
}

template < Arithmetic T, int32_t Dimension >
constexpr Point< T, Dimension >&
Point< T, Dimension >::operator+=( const Point& other ) noexcept
{
    std::transform( mCoordinates.begin( ),
                    mCoordinates.end( ),
                    other.begin( ),
                    mCoordinates.begin( ),
                    std::plus< T >( ) );

    return *this;
}

template < Arithmetic T, int32_t Dimension >
template < typename U >
constexpr Point< std::common_type_t< T, U >, Dimension >
Point< T, Dimension >::operator-(
    const Point< U, Dimension >& other ) const noexcept
{
    using common_type = std::common_type_t< T, U >;

    std::array< common_type, Dimension > newCoordinates { };

    std::transform( mCoordinates.begin( ),
                    mCoordinates.end( ),
                    other.begin( ),
                    newCoordinates.begin( ),
                    std::minus< std::common_type_t< T, U > >( ) );

    return Point< common_type, Dimension >( newCoordinates );
}

template < Arithmetic T, int32_t Dimension >
constexpr Point< T, Dimension >&
Point< T, Dimension >::operator-=( const Point& other ) noexcept
{
    std::transform( mCoordinates.begin( ),
                    mCoordinates.end( ),
                    other.begin( ),
                    mCoordinates.begin( ),
                    std::minus< T >( ) );

    return *this;
}

template < Arithmetic T, int32_t Dimension >
template < typename U >
constexpr Point< std::common_type_t< T, U >, Dimension >
Point< T, Dimension >::operator*( U scale ) const noexcept
{
    using common_type = std::common_type_t< T, U >;

    std::array< common_type, Dimension > newCoordinates { };

    std::transform( mCoordinates.cbegin( ),
                    mCoordinates.cend( ),
                    newCoordinates.begin( ),
                    [ & ]( const T coordinate ) {
                        return static_cast< std::common_type_t< T, U > >(
                            coordinate * scale );
                    } );

    return Point< common_type, Dimension >( newCoordinates );
}

template < Arithmetic T, int32_t Dimension >
template < typename U >
constexpr Point< T, Dimension >&
Point< T, Dimension >::operator*=( U scale ) noexcept
{
    std::transform( mCoordinates.cbegin( ),
                    mCoordinates.cend( ),
                    mCoordinates.begin( ),
                    [ & ]( const T coordinate )
                    { return static_cast< T >( coordinate * scale ); } );

    return *this;
}

template < Arithmetic T, int32_t Dimension >
void Point< T, Dimension >::swap( Point& other ) noexcept
{
    std::swap( this->mCoordinates, other.mCoordinates );
}

template < Arithmetic T, int32_t Dimension >
template < int32_t N_DIM, typename >
constexpr T Point< T, Dimension >::getX( ) const
{
    return mCoordinates.at( 0 );
}

template < Arithmetic T, int32_t Dimension >
template < int32_t N_DIM, typename >
constexpr T Point< T, Dimension >::getY( ) const
{
    return mCoordinates.at( 1 );
}

template < Arithmetic T, int32_t Dimension >
template < int32_t N_DIM, typename >
constexpr T Point< T, Dimension >::getZ( ) const
{
    return mCoordinates.at( 2 );
}

template < Arithmetic T, int32_t Dimension >
constexpr typename Point< T, Dimension >::reference_type
Point< T, Dimension >::at( size_t index )
{
    return mCoordinates.at( index );
}

template < Arithmetic T, int32_t Dimension >
[[nodiscard]] constexpr typename Point< T, Dimension >::const_reference_type
Point< T, Dimension >::at( size_t index ) const
{
    return mCoordinates.at( index );
}

template < Arithmetic T, int32_t Dimension >
constexpr typename Point< T, Dimension >::reference_type
Point< T, Dimension >::operator[]( const size_t index )
{
    return mCoordinates[ index ];
}

template < Arithmetic T, int32_t Dimension >
constexpr typename Point< T, Dimension >::const_reference_type
Point< T, Dimension >::operator[]( const size_t index ) const
{
    return mCoordinates[ index ];
}

template < Arithmetic T, int32_t Dimension >
constexpr std::array< T, Dimension >& Point< T, Dimension >::coordinates( )
{
    return mCoordinates;
}

template < Arithmetic T, int32_t Dimension >
constexpr std::array< T, Dimension > Point< T, Dimension >::coordinates( ) const
{
    return mCoordinates;
}

template < Arithmetic T, int32_t Dimension >
constexpr typename Point< T, Dimension >::iterator
Point< T, Dimension >::begin( ) noexcept
{
    return mCoordinates.begin( );
}

template < Arithmetic T, int32_t Dimension >
constexpr typename Point< T, Dimension >::const_iterator
Point< T, Dimension >::begin( ) const noexcept
{
    return mCoordinates.cbegin( );
}

template < Arithmetic T, int32_t Dimension >
constexpr typename Point< T, Dimension >::iterator
Point< T, Dimension >::end( ) noexcept
{
    return mCoordinates.end( );
}

template < Arithmetic T, int32_t Dimension >
constexpr typename Point< T, Dimension >::const_iterator
Point< T, Dimension >::end( ) const noexcept
{
    return mCoordinates.cend( );
}

template < Arithmetic T, int32_t Dimension >
std::ostream& operator<<( std::ostream& os, const Point< T, Dimension >& point )
{
    if constexpr ( Dimension == 2 )
    {
        return os << "Point (x: " << point.getX( ) << ", y: " << point.getY( )
                  << ")";
    }
    else if constexpr ( Dimension == 3 )
    {
        return os << "Point (x: " << point.getX( ) << ", y: " << point.getY( )
                  << ", z: " << point.getZ( ) << ")";
    }
    else
    {
        os << "Point (";
        for ( auto iter = point.begin( ); iter != point.end( ); ++iter )
        {
            os << *iter;

            if ( iter != point.end( ) - 1 )
            {
                os << ", ";
            }
        }
        os << ")";
        return os;
    }
}

using Point2d = Point< double, 2 >;
using Point2f = Point< float, 2 >;
using Point2i = Point< int32_t, 2 >;

using Point3d = Point< double, 3 >;
using Point3f = Point< float, 3 >;
using Point3i = Point< int32_t, 3 >;

} // namespace cvl::core
