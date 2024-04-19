#pragma once

// STD includes

// CVL includes
#include <cvl/core/NormTraits.h>
#include <cvl/core/Point.h>
#include <cvl/core/Vector.h>

namespace cvl::core
{
/**
 * @brief Template implementation for a generic n Dimensional line
 *
 * Main/Normal form:        g(x) := y = mx + b
 * Coordinates form         g(x) := ax + by = c, a = nx and b = ny
 * Point direction form     g(x) := o + t * r, o = offset, r = direction
 * Hesse form               g(x) := n0 * ( x - o ), n0 = normalized normal,
 *                               o = offset, x = point on the line
 *
 * n is perpendicular to r
 *
 * n * ( x - o ) = n x - n o -> n o = c
 *
 */
template < int32_t Dimension >
class Line
{
public:
    /**
     * Default constructor
     *
     */
    Line( ) = default;

    /**
     * Value constructor from two points
     *
     * @param [in]  point1        The first point used for the location vector
     * @param [in]  point2        The second point P2P1 = direction vector
     *
     */
    template < typename U, typename V >
    Line( const Point< U, Dimension >& point1,
          const Point< V, Dimension >& point2 );

    /**
     * Value constructor from two points
     *
     * @param [in]  locationVector     The location vector of the line
     * @param [in]  directionVector    The direction vector of the line
     *
     */
    Line( const Vector< Dimension >& locationVector,
          const Vector< Dimension >& directionVector );

    /**
     * Copy constructor
     *
     * @param [in]  other     The vector to copy from
     */
    Line( const Line& other );

    /**
     * Move constructor
     *
     * @param [in]  other     The vector to move
     */
    Line( Line&& other ) noexcept;

    /**
     * Destructor
     *
     */
    ~Line( ) = default;

    /**
     * Assignment operator
     *
     * @param [in]  other     The vector to assign from
     */
    constexpr Line& operator=( const Line& other ) noexcept;

    /**
     * Move operator
     *
     * @param [in]  other  The vector to move from
     */
    constexpr Line& operator=( Line&& other ) noexcept;

    /**
     * Equal operator
     *
     * @param [in]  other  The vector to compare with
     *
     * @return true if equal, else false
     */
    constexpr bool operator==( const Line& other ) const noexcept;

    /**
     * Not equal operator
     *
     * @param [in]  other  The vector to compare with
     *
     * @return true if not equal, else false
     */
    constexpr bool operator!=( const Line& other ) const noexcept;

    /**
     * Function that returns the location vector of the line.
     *
     * @return The location vector
     */
    [[nodiscard]] Vector< Dimension > getLocation( ) const;

    /**
     * Function that returns the direction vector of the line.
     *
     * @return The direction vector
     */
    [[nodiscard]] Vector< Dimension > getDirection( ) const;

    /**
     * Calculates the slope m of the line for a line in main form
     *
     * y := mx + b
     *
     * @returns The slope of the line
     */
    template < int32_t N_Dim = Dimension,
               typename = std::enable_if_t< N_Dim == 2 > >
    [[nodiscard]] double getSlope( ) const;

    /**
     * Calculates the axis offset b of the line for a line in main form
     *
     * y := mx + b
     *
     * @returns The axis offset of the line
     */
    template < int32_t N_Dim = Dimension,
               typename = std::enable_if_t< N_Dim == 2 > >
    [[nodiscard]] double getAxisOffset( ) const;

    /**
     * Function that returns the normal vector of the line.
     *
     * @return The normal vector
     */
    template < int32_t N_Dim = Dimension,
               typename = std::enable_if_t< N_Dim == 2 > >
    [[nodiscard]] Vector< Dimension > getNormal( ) const;

    /**
     * Function that returns the parameter a of a line in coordinate form.
     *
     * @return The parameter a of g(x) := ax + by + c = 0
     */
    template < int32_t N_Dim = Dimension,
               typename = std::enable_if_t< N_Dim == 2 > >
    [[nodiscard]] double getParameterA( ) const;

    /**
     * Function that returns the parameter b of a line in coordinate form.
     *
     * @return The parameter b of g(x) := ax + by + c = 0
     */
    template < int32_t N_Dim = Dimension,
               typename = std::enable_if_t< N_Dim == 2 > >
    [[nodiscard]] double getParameterB( ) const;

    /**
     * Function that returns the parameter c of a line in coordinate form.
     *
     * @return The parameter c of g(x) := ax + by + c = 0
     */
    template < int32_t N_Dim = Dimension,
               typename = std::enable_if_t< N_Dim == 2 > >
    [[nodiscard]] double getParameterC( ) const;

private:
    void swap( Line& other ) noexcept;

private:
    Vector< Dimension > mLocationVector;
    Vector< Dimension > mDirectionVector;
};

template < int32_t Dimension >
template < typename U, typename V >
Line< Dimension >::Line( const Point< U, Dimension >& point1,
                         const Point< V, Dimension >& point2 )
    : mLocationVector( point1 )
    , mDirectionVector( Vector< 2 >( point1, point2 ) )
{
}

template < int32_t Dimension >
Line< Dimension >::Line( const Vector< Dimension >& locationVector,
                         const Vector< Dimension >& directionVector )
    : mLocationVector( locationVector )
    , mDirectionVector( directionVector )
{
}

template < int32_t Dimension >
Line< Dimension >::Line( const Line& other )
    : mLocationVector( other.mLocationVector )
    , mDirectionVector( other.mDirectionVector )
{
}

template < int32_t Dimension >
Line< Dimension >::Line( Line&& other ) noexcept
    : mLocationVector( std::move( other.mLocationVector ) )
    , mDirectionVector( std::move( other.mDirectionVector ) )
{
}

template < int32_t Dimension >
constexpr Line< Dimension >&
Line< Dimension >::operator=( const Line& other ) noexcept
{
    if ( this != &other )
    {
        Line tmp( other );
        tmp.swap( *this );
    }

    return *this;
}

template < int32_t Dimension >
constexpr Line< Dimension >&
Line< Dimension >::operator=( Line&& other ) noexcept
{
    if ( this != &other )
    {
        // Transfer ownership
        this->mDirectionVector = std::move( other.mDirectionVector );
        this->mLocationVector = std::move( other.mLocationVector );
    }

    return *this;
}

template < int32_t Dimension >
constexpr bool Line< Dimension >::operator==( const Line& other ) const noexcept
{
    return this->mDirectionVector == other.mDirectionVector &&
           this->mLocationVector == other.mLocationVector;
}

template < int32_t Dimension >
constexpr bool Line< Dimension >::operator!=( const Line& other ) const noexcept
{
    return ! ( *this == other );
}

template < int32_t Dimension >
Vector< Dimension > Line< Dimension >::getLocation( ) const
{
    return mLocationVector;
}

template < int32_t Dimension >
Vector< Dimension > Line< Dimension >::getDirection( ) const
{
    return mDirectionVector;
}

template < int32_t Dimension >
void Line< Dimension >::swap( Line& other ) noexcept
{
    std::swap( this->mDirectionVector, other.mDirectionVector );
    std::swap( this->mLocationVector, other.mLocationVector );
}

template < int32_t Dimension >
template < int32_t N_Dim, typename >
double Line< Dimension >::getSlope( ) const
{
    // The parameter m (slope) can be derived from the direction vector
    return mDirectionVector.at( 1 ) / mDirectionVector.at( 0 );
}

template < int32_t Dimension >
template < int32_t N_Dim, typename >
double Line< Dimension >::getAxisOffset( ) const
{
    // The offset b can be derived by knowing the the offset vector is a point
    // on the line b = y - mx
    return mLocationVector.at( 1 ) - getSlope( ) * mLocationVector.at( 0 );
}

template < int32_t Dimension >
template < int32_t N_Dim, typename >
Vector< Dimension > Line< Dimension >::getNormal( ) const
{
    Vector< Dimension > normal;

    normal.at( 0 ) = mDirectionVector.at( 1 );
    normal.at( 1 ) = mDirectionVector.at( 0 ) * -1.0;

    return normal;
}

template < int32_t Dimension >
template < int32_t N_Dim, typename >
double Line< Dimension >::getParameterA( ) const
{
    // Line in normal form
    // g(x) : = n * ( x - o ) -> n * x - n * o
    // Parameter a = nx
    return getNormal( )[ 0 ];
}

template < int32_t Dimension >
template < int32_t N_Dim, typename >
double Line< Dimension >::getParameterB( ) const
{
    // Line in normal form
    // g(x) : = n * ( x - o ) -> n * x - n * o
    // Parameter b = ny
    return getNormal( )[ 1 ];
}

template < int32_t Dimension >
template < int32_t N_Dim, typename >
double Line< Dimension >::getParameterC( ) const
{
    // To be able to calculate parameter c we can use the normal vector and the
    // location vector
    // Parameter a = nx and parameter b = ny
    // g(x) := ax + by + c = 0 Hartley Zissermsn
    // g(x) := ax + by = c
    // We just use the location vector for calculation c
    // -c = ax + by Hartley Zissermsn
    const auto normal = getNormal( );

    return normal[ 0 ] * mLocationVector[ 0 ] +
           normal[ 1 ] * mLocationVector[ 1 ];

    // This will also work
    // Line in normal form
    // g(x) : = n * ( x - o ) -> n * x - n * o
    // Parameter c = n * o
}

using Line2d = Line< 2 >;
using Line3d = Line< 3 >;

} // namespace cvl::core