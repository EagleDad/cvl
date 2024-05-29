#pragma warning( push )
#pragma warning( disable : 4868 )

// CVL includes
#include <Core.h>
#include <Processing.h>

// STD includes

int32_t main( [[maybe_unused]] int32_t argc, [[maybe_unused]] char** argv )
{
    cvl::core::Image< uint8_t, 1 > image( 16, 16 );

   /* cvl::core::Region< uint8_t,
                       cvl::core::AlignedAllocator< uint8_t >,
                       cvl::processing::Area >
        region( image, 1, 5.0 );

    auto area = region.getArea( );

    std::cout << area << "\n";*/

    return 0;
}

#pragma warning( pop )