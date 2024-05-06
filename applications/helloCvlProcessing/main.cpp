#pragma warning( push )
#pragma warning( disable : 4868 )

// CVL includes
#include <Core.h>
#include <Processing.h>

// STD includes

template < typename Derived >
class Area : public cvl::core::CrtpBase< Derived, Area >
{
public:
    Area( )
    {
        auto val = 5.0;
        mArea = val;
    }
    Area( const Area& other ) = default;
    Area( Area&& other ) = default;
    Area& operator=( const Area& other ) = default;
    Area& operator=( Area&& other ) = default;

    Area( double area )
        : mArea( area )
    {
    }

    double getArea( ) const { return mArea; }

private:
    double mArea { };
};

int32_t main( [[maybe_unused]] int32_t argc, [[maybe_unused]] char** argv )
{
    cvl::core::Image< uint8_t, 1 > image( 16, 16 );

    cvl::core::Region< uint8_t, cvl::core::AlignedAllocator< uint8_t >, Area >
        region( image, 1, 5.0 );

    auto area = region.getArea( );

    std::cout << area << "\n";

    return 0;
}

#pragma warning( pop )