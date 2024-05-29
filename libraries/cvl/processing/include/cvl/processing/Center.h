#pragma once

// CVL includes
#include <cvl/core/CallOnce.h>
#include <cvl/core/CrtpBase.h>
#include <cvl/core/Point.h>

// STD includes

namespace cvl::processing
{

template < typename Derived >
class Center : public core::CrtpBase< Derived, Center >
{
public:
    Center( ) noexcept = default;
    Center( const Center& other ) = default;
    Center( Center&& other ) noexcept = default;
    Center& operator=( const Center& other ) noexcept = default;
    Center& operator=( Center&& other ) noexcept = default;
    ~Center( ) = default;

    //Center( const core::Point< float, 2 >& center ) noexcept;

    [[nodiscard]] core::Point< float, 2 > getCenter( );

private:
    void calculate( );

private:
    core::Point< float, 2 > mCenter { };
    bool mCalculated { false };
};

//template < typename Derived >
//Center< Derived >::Center( const core::Point< float, 2 >& center ) noexcept
//    : mCenter( center )
//{
//}

template < typename Derived >
core::Point< float, 2 > Center< Derived >::getCenter( )
{
    core::call_once( mCalculated, [ this ] { calculate( ); } );
    return mCenter;
}

template < typename Derived >
void Center< Derived >::calculate( )
{
    auto& labelImage = this->underlying( ).getLabelImage( );
    std::ignore = labelImage;
}

} // namespace cvl::processing