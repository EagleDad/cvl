#pragma once

// CVL includes
#include <cvl/core/CallOnce.h>
#include <cvl/core/CrtpBase.h>
#include <cvl/core/Rectangle.h>

// STD includes

namespace cvl::processing
{

template < typename Derived >
class BoundingBox : public core::CrtpBase< Derived, BoundingBox >
{
public:
    BoundingBox( ) noexcept = default;
    BoundingBox( const BoundingBox& other ) = default;
    BoundingBox( BoundingBox&& other ) noexcept = default;
    BoundingBox& operator=( const BoundingBox& other ) noexcept = default;
    BoundingBox& operator=( BoundingBox&& other ) noexcept = default;
    ~BoundingBox( ) = default;

    //BoundingBox( const core::Rectangle< float >& boundingBox ) noexcept;

    [[nodiscard]] core::Rectangle< float > getBoundingBox( );

private:
    void calculate( );

private:
    core::Rectangle< float > mBoundingBox { };
    bool mCalculated { false };
};

//template < typename Derived >
//BoundingBox< Derived >::BoundingBox(
//    const core::Rectangle< float >& boundingBox ) noexcept
//    : mBoundingBox( boundingBox )
//{
//}

template < typename Derived >
core::Rectangle< float > BoundingBox< Derived >::getBoundingBox( )
{
    core::call_once( mCalculated, [ this ] { calculate( ); } );
    return mBoundingBox;
}

template < typename Derived >
void BoundingBox< Derived >::calculate( )
{
    auto& labelImage = this->underlying( ).getLabelImage( );
    std::ignore = labelImage;
    // mArea;
}

} // namespace cvl::processing