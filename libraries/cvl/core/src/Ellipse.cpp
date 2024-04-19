// Own includes
#include <cvl/core/Ellipse.h>

namespace cvl::core
{
Ellipse::Ellipse( const Point2d& center, const SizeD& size, double phi )
    : mCenter( center )
    , mSize( size )
    , mPhi( phi )
{
}

Point2d Ellipse::getCenter( ) const
{
    return mCenter;
}

SizeD Ellipse::getSize( ) const
{
    return mSize;
}

double Ellipse::getAngle( ) const
{
    return mPhi;
}

} // namespace cvl::core
