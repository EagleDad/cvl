#pragma once

// STD includes
#include <algorithm>
#include <array>
#include <numeric>

// CVL includes
#include <cvl/core/Point.h>

namespace cvl::core
{
/**
 * @brief Template implementation for a generic n Dimensional vector
 *
 */
template < int32_t Dimension >
class Vector
{
public:
    using value_type = double;
    using reference_type = value_type&;
    using const_reference_type = const value_type&;

    using iterator = typename std::array< value_type, Dimension >::iterator;
    using const_iterator =
        typename std::array< value_type, Dimension >::const_iterator;

    /**
     * Default constructor
     *
     */
    Vector( ) = default;

    /**
     * Value constructor
     *
     */
    template < typename... TArgs >
    explicit Vector( TArgs... args );

    /**
     * Create a vector from a std array
     *
     * @param [in]  elements  The array to create from
     */
    explicit Vector( const std::array< double, Dimension >& elements );

    /**
     * Value constructor from one point, like location vector
     *
     * @param [in]  point  The point
     */
    template < typename U >
    Vector( const Point< U, Dimension >& point );

    /**
     * Value constructor from tow points
     *
     * V := P2 - P1
     *
     * @param [in]  point1  The first point
     * @param [in]  point2  The second point
     */
    template < typename U, typename V >
    Vector( const Point< U, Dimension >& point1,
            const Point< V, Dimension >& point2 );

    /**
     * Copy constructor
     *
     * @param [in]  other     The vector to copy from
     */
    Vector( const Vector& other );

    /**
     * Move constructor
     *
     * @param [in]  other     The vector to move
     */
    Vector( Vector&& other ) noexcept;

    /**
     * Destructor
     *
     */
    ~Vector( ) = default;

    /**
     * Assignment operator
     *
     * @param [in]  other     The vector to assign from
     */
    constexpr Vector& operator=( const Vector& other ) noexcept;

    /**
     * Move operator
     *
     * @param [in]  other  The vector to move from
     */
    constexpr Vector& operator=( Vector&& other ) noexcept;

    /**
     * Equal operator
     *
     * @param [in]  other  The vector to compare with
     *
     * @return true if equal, else false
     */
    constexpr bool operator==( const Vector& other ) const noexcept;

    /**
     * Not equal operator
     *
     * @param [in]  other  The vector to compare with
     *
     * @return true if not equal, else false
     */
    constexpr bool operator!=( const Vector& other ) const noexcept;

    /**
     * Add operator
     *
     * @param [in]  other  The vector to add
     */
    Vector operator+( const Vector& other ) const noexcept;

    /**
     * Add operator
     *
     * @param [in]  other  The vector to add
     */
    Vector& operator+=( const Vector& other ) noexcept;

    /**
     * Subtract operator
     *
     * @param [in]  other  The vector to subtract
     */
    Vector operator-( const Vector& other ) const noexcept;

    /**
     * Subtract operator
     *
     * @param [in]  other  The vector to subtract
     */
    Vector& operator-=( const Vector& other ) noexcept;

    /**
     * Scale operator
     *
     * @param [in]  scale  The scale factor for the vector
     */
    template < Arithmetic U >
    Vector operator*( U scale ) const noexcept;

    /**
     * scale operator
     *
     * @param [in]  scale  The scale factor for the vector
     */
    template < Arithmetic U >
    Vector& operator*=( U scale ) noexcept;

    /**
     * Dot product operator
     *
     * @param [in]  other  The vector to build the dot product with
     */
    double operator*( const Vector& other ) const noexcept;

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
     * Accessor for elements
     *
     * @returns the element array
     */
    constexpr std::array< double, Dimension >& elements( );

    /**
     * Accessor for elements
     *
     * @returns the element array
     */
    [[nodiscard]] constexpr std::array< double, Dimension > elements( ) const;

    /**
     * Iterator to the begin
     *
     * @returns The iterator to the begin ot the element array
     */
    constexpr iterator begin( ) noexcept;

    /**
     * Iterator to the begin
     *
     * @returns The const iterator to the begin ot the element array
     */
    [[nodiscard]] constexpr const_iterator begin( ) const noexcept;

    /**
     * Iterator to the end
     *
     * @returns The iterator to the end ot the element array
     */
    constexpr iterator end( ) noexcept;

    /**
     * Iterator to the end
     *
     * @returns The const iterator to the end ot the element array
     */
    [[nodiscard]] constexpr const_iterator end( ) const noexcept;

private:
    void swap( Vector& other ) noexcept;

private:
    std::array< double, Dimension > mElements { };
};

template < int32_t Dimension >
template < typename... TArgs >
Vector< Dimension >::Vector( TArgs... args )
    : mElements( { { args... } } )
{
}

template < int32_t Dimension >
Vector< Dimension >::Vector( const std::array< double, Dimension >& elements )
    : mElements( elements )
{
}

template < int32_t Dimension >
template < typename U >
Vector< Dimension >::Vector( const Point< U, Dimension >& point )
{
    for ( size_t i = 0; i < Dimension; i++ )
    {
        mElements[ i ] = static_cast< double >( point.coordinates( )[ i ] );
    }
}

template < int32_t Dimension >
template < typename U, typename V >
Vector< Dimension >::Vector( const Point< U, Dimension >& point1,
                             const Point< V, Dimension >& point2 )
{
    for ( size_t i = 0; i < Dimension; i++ )
    {
        mElements[ i ] = static_cast< double >( point2.coordinates( )[ i ] -
                                                point1.coordinates( )[ i ] );
    }
}

template < int32_t Dimension >
Vector< Dimension >::Vector( const Vector& other )
    : mElements( other.mElements )
{
}

template < int32_t Dimension >
Vector< Dimension >::Vector( Vector&& other ) noexcept
    : mElements( std::move( other.mElements ) )
{
}

template < int32_t Dimension >
constexpr Vector< Dimension >&
Vector< Dimension >::operator=( const Vector& other ) noexcept
{
    if ( this != &other )
    {
        Vector tmp( other );
        tmp.swap( *this );
    }

    return *this;
}

template < int32_t Dimension >
constexpr Vector< Dimension >&
Vector< Dimension >::operator=( Vector&& other ) noexcept
{
    if ( this != &other )
    {
        // Transfer ownership
        this->mElements = std::move( other.mElements );
    }

    return *this;
}

template < int32_t Dimension >
constexpr bool
Vector< Dimension >::operator==( const Vector& other ) const noexcept
{
    return this->mElements == other.mElements;
}

template < int32_t Dimension >
constexpr bool
Vector< Dimension >::operator!=( const Vector& other ) const noexcept
{
    return ! ( *this == other );
}

template < int32_t Dimension >
Vector< Dimension >
Vector< Dimension >::operator+( const Vector& other ) const noexcept
{
    Vector ret;

    std::transform( this->begin( ),
                    this->end( ),
                    other.begin( ),
                    ret.begin( ),
                    std::plus( ) );

    return ret;
}

template < int32_t Dimension >
Vector< Dimension >&
Vector< Dimension >::operator+=( const Vector& other ) noexcept
{
    std::transform( this->begin( ),
                    this->end( ),
                    other.begin( ),
                    this->begin( ),
                    std::plus( ) );

    return *this;
}

template < int32_t Dimension >
Vector< Dimension >
Vector< Dimension >::operator-( const Vector& other ) const noexcept
{
    Vector ret;

    std::transform( this->begin( ),
                    this->end( ),
                    other.begin( ),
                    ret.begin( ),
                    std::minus( ) );

    return ret;
}

template < int32_t Dimension >
Vector< Dimension >&
Vector< Dimension >::operator-=( const Vector& other ) noexcept
{
    std::transform( this->begin( ),
                    this->end( ),
                    other.begin( ),
                    this->begin( ),
                    std::minus( ) );

    return *this;
}

template < int32_t Dimension >
template < Arithmetic U >
Vector< Dimension > Vector< Dimension >::operator*( U scale ) const noexcept
{
    Vector ret;

    std::transform( this->begin( ),
                    this->end( ),
                    ret.begin( ),
                    [ &scale ]( double element )
                    { return element *= static_cast< double >( scale ); } );

    return ret;
}

template < int32_t Dimension >
template < Arithmetic U >
Vector< Dimension >& Vector< Dimension >::operator*=( U scale ) noexcept
{
    std::transform( this->begin( ),
                    this->end( ),
                    this->begin( ),
                    [ &scale ]( double element )
                    { return element *= static_cast< double >( scale ); } );

    return *this;
}

template < int32_t Dimension >
double Vector< Dimension >::operator*( const Vector& other ) const noexcept
{
    return std::inner_product(
        this->begin( ), this->end( ), other.begin( ), 0.0 );
}

template < int32_t Dimension >
constexpr typename Vector< Dimension >::reference_type
Vector< Dimension >::at( size_t index )
{
    return mElements.at( index );
}

template < int32_t Dimension >
[[nodiscard]] constexpr typename Vector< Dimension >::const_reference_type
Vector< Dimension >::at( size_t index ) const
{
    return mElements.at( index );
}

template < int32_t Dimension >
constexpr typename Vector< Dimension >::reference_type
Vector< Dimension >::operator[]( const size_t index )
{
    return mElements[ index ];
}

template < int32_t Dimension >
constexpr typename Vector< Dimension >::const_reference_type
Vector< Dimension >::operator[]( const size_t index ) const
{
    return mElements[ index ];
}

template < int32_t Dimension >
constexpr std::array< double, Dimension >& Vector< Dimension >::elements( )
{
    return mElements;
}

template < int32_t Dimension >
constexpr std::array< double, Dimension > Vector< Dimension >::elements( ) const
{
    return mElements;
}

template < int32_t Dimension >
constexpr typename Vector< Dimension >::iterator
Vector< Dimension >::begin( ) noexcept
{
    return mElements.begin( );
}

template < int32_t Dimension >
constexpr typename Vector< Dimension >::const_iterator
Vector< Dimension >::begin( ) const noexcept
{
    return mElements.cbegin( );
}

template < int32_t Dimension >
constexpr typename Vector< Dimension >::iterator
Vector< Dimension >::end( ) noexcept
{
    return mElements.end( );
}

template < int32_t Dimension >
constexpr typename Vector< Dimension >::const_iterator
Vector< Dimension >::end( ) const noexcept
{
    return mElements.cend( );
}

template < int32_t Dimension >
void Vector< Dimension >::swap( Vector& other ) noexcept
{
    std::swap( this->mElements, other.mElements );
}

} // namespace cvl::core