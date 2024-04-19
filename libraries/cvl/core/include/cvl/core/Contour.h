#pragma once

// STD includes
#include <vector>

// CVL includes
#include <cvl/core/ContourTraits.h>
#include <cvl/core/Point.h>
#include <cvl/core/macros.h>

namespace cvl::core
{
/**
 * @brief The Contour class
 *
 * A contour describes an object that contains ordered points like a polygon.
 * The data type could be double or int32_t. The contour can contain further
 * information about the edged like direction and response.
 *
 */
template < typename ValueType,
           template < typename > typename... ContourFeature >
class Contour
    : public ContourFeature< Contour< ValueType, ContourFeature... > >...
{
public:
    using value_type = ValueType;
    using reference_type = ValueType&;
    using pointer_type = ValueType*;
    using const_reference_type = const ValueType&;

    CVL_NO_DEFAULT_INIT( Contour );

    Contour( const std::vector< Point< ValueType, 2 > >& contour );

    Contour( const std::vector< Point< ValueType, 2 > >& contour,
             const std::vector< double >& response,
             const std::vector< double >& direction );

    const std::vector< Point< ValueType, 2 > >& getContourPoints( ) const;

    [[nodiscard]] const std::vector< double >& getDirections( ) const;

    [[nodiscard]] const std::vector< double >& getResponses( ) const;

private:
    std::vector< Point< ValueType, 2 > > mContour;
    std::vector< double > mResponse;
    std::vector< double > mDirection;
};

template < typename ValueType,
           template < typename > typename... ContourFeature >
Contour< ValueType, ContourFeature... >::Contour(
    const std::vector< Point< ValueType, 2 > >& contour )
    : mContour( contour )
{
    EXPECT_MSG( contour.size( ) != 0, "Invalid input contour!" );
}

template < typename ValueType,
           template < typename > typename... ContourFeature >
Contour< ValueType, ContourFeature... >::Contour(
    const std::vector< Point< ValueType, 2 > >& contour,
    const std::vector< double >& response,
    const std::vector< double >& direction )
    : mContour( contour )
    , mResponse( response )
    , mDirection( direction )
{
    EXPECT_MSG( contour.size( ) != 0, "Invalid input contour!" );
    EXPECT_MSG( contour.size( ) == response.size( ),
                "Response size does not match Contour size" );
    EXPECT_MSG( contour.size( ) == direction.size( ),
                "Direction size does not match Contour size" );
}

template < typename ValueType,
           template < typename > typename... ContourFeature >
const std::vector< Point< ValueType, 2 > >&
Contour< ValueType, ContourFeature... >::getContourPoints( ) const
{
    return mContour;
}

template < typename ValueType,
           template < typename > typename... ContourFeature >
const std::vector< double >&
Contour< ValueType, ContourFeature... >::getDirections( ) const
{
    return mDirection;
}

template < typename ValueType,
           template < typename > typename... ContourFeature >
const std::vector< double >&
Contour< ValueType, ContourFeature... >::getResponses( ) const
{
    return mResponse;
}

} // namespace cvl::core