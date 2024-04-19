#pragma once

// STD includes

// CVL includes
#include <cvl/core/AccessTraits.h>
#include <cvl/core/DimensionTraits.h>
#include <cvl/core/Types.h>
#include <cvl/core/macros.h>

namespace traits
{

template < typename Object1, typename Object2, int D >
struct Manhattan
{
    static double apply( Object1 const& a, Object2 const& b )
    {
        const double d = static_cast< double >( get< D - 1 >( a ) ) -
                         static_cast< double >( get< D - 1 >( b ) );
        return std::abs( d ) +
               Manhattan< Object1, Object2, D - 1 >::apply( a, b );
    }
};

template < typename Object1, typename Object2 >
struct Manhattan< Object1, Object2, 0 >
{
    static double apply( Object1 const&, Object2 const& ) { return 0; }
};

template < typename Object1, typename Object2, int D >
struct Euclidean
{
    static double apply( Object1 const& a, Object2 const& b )
    {
        const double d = static_cast< double >( get< D - 1 >( a ) ) -
                         static_cast< double >( get< D - 1 >( b ) );
        return d * d + Euclidean< Object1, Object2, D - 1 >::apply( a, b );
    }
};

template < typename Object1, typename Object2 >
struct Euclidean< Object1, Object2, 0 >
{
    static double apply( Object1 const&, Object2 const& ) { return 0; }
};

template < typename Object1, typename Object2, int D >
struct Maximum
{
    static double apply( Object1 const& a, Object2 const& b )
    {
        const double d = static_cast< double >( get< D - 1 >( a ) ) -
                         static_cast< double >( get< D - 1 >( b ) );
        return std::max( std::abs( d ),
                         Maximum< Object1, Object2, D - 1 >::apply( a, b ) );
    }
};

template < typename Object1, typename Object2 >
struct Maximum< Object1, Object2, 0 >
{
    static double apply( Object1 const&, Object2 const& ) { return 0; }
};

template < cvl::core::Norm NormType, typename Object1, typename Object2 >
struct Norm
{
    static double norm( [[maybe_unused]] Object1 const& a,
                        [[maybe_unused]] Object2 const& b )
    {
        THROW_MSG( "Unsupported type for norm calculation" );
    }
};

template < typename Object1, typename Object2 >
struct Norm< cvl::core::Norm::Euclidean, Object1, Object2 >
{
    static double norm( Object1 const& a, Object2 const& b )
    {
        return sqrt(
            Euclidean< Object1, Object2, dimension< Object1 >::value >::apply(
                a, b ) );
    }
};

template < typename Object1, typename Object2 >
struct Norm< cvl::core::Norm::Manhattan, Object1, Object2 >
{
    static double norm( Object1 const& a, Object2 const& b )
    {
        return Manhattan< Object1, Object2, dimension< Object1 >::value >::
            apply( a, b );
    }
};

template < typename Object1, typename Object2 >
struct Norm< cvl::core::Norm::Maximum, Object1, Object2 >
{
    static double norm( Object1 const& a, Object2 const& b )
    {
        return Maximum< Object1, Object2, dimension< Object1 >::value >::apply(
            a, b );
    }
};

} // namespace traits

template < cvl::core::Norm NormType, typename Object1, typename Object2 >
double distance( Object1 const& a, Object2 const& b )
{
    return traits::Norm< NormType, Object1, Object2 >::norm( a, b );
}

template < cvl::core::Norm NormType, typename Object >
double norm( Object const& a )
{
    constexpr Object b { };
    return traits::Norm< NormType, Object, Object >::norm( a, b );
}

template < cvl::core::Norm NormType, typename Object >
Object normalize( Object const& a )
{
    return a * ( 1.0 / norm< NormType >( a ) );
}