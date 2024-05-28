#pragma once

// CVL includes
#include <cvl/core/CallOnce.h>
#include <cvl/core/CrtpBase.h>

// STD includes

namespace cvl::processing
{

template < typename Derived >
class Area : public core::CrtpBase< Derived, Area >
{
public:
    Area( ) noexcept = default;
    Area( const Area& other ) = default;
    Area( Area&& other ) noexcept = default;
    Area& operator=( const Area& other ) noexcept = default;
    Area& operator=( Area&& other ) noexcept = default;
    ~Area( ) = default;

    Area( double area ) noexcept;

    [[nodiscard]] double getArea( );

private:
    void calculate( );

private:
    double mArea { };
    bool mCalculated { false };
};

template < typename Derived >
Area< Derived >::Area( double area ) noexcept
    : mArea( area )
{
}

template < typename Derived >
double Area< Derived >::getArea( )
{
    core::call_once( mCalculated, [ this ] { calculate( ); } );
    return mArea;
}

template < typename Derived >
void Area< Derived >::calculate( )
{
    auto& labelImage = this->underlying( ).getLabelImage( );
    std::ignore = labelImage;
    // mArea;
}

} // namespace cvl::processing