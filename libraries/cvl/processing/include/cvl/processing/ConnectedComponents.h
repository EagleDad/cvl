#pragma once

// CVL includes
#include <cvl/core/Image.h>
#include <cvl/core/Region.h>
#include <cvl/processing/Area.h>
#include <cvl/processing/BoundingBox.h>
#include <cvl/processing/Center.h>

// STD includes

template < typename Derived >
constexpr bool hasGetArea = requires( Derived derived ) { derived.getArea( ); };

namespace cvl::processing
{
template < typename Allocator >
using allocator_traits = std::allocator_traits< Allocator >;

namespace detail
{

struct Blob
{
    int32_t top { };
    int32_t bottom { };
    int32_t left { };
    int32_t right { };
    int32_t area { };
    int32_t label { };
    int32_t sumX { };
    int32_t sumY { };

    Blob( int32_t x, int32_t y, int32_t objectId )
        : top( y )
        , bottom( y )
        , left( x )
        , right( x )
        , area( 1 )
        , label( objectId )
        , sumX( x )
        , sumY( y )
    {
    }

    void add( int32_t x, int32_t y )
    {
        area += 1;
        sumX += x;
        sumY += y;

        if ( x < left )
        {
            left = x;
        }

        if ( x > right )
        {
            right = x;
        }

        if ( y < top )
        {
            top = y;
        }

        if ( y > bottom )
        {
            bottom = y;
        }
    }

    void merge( const Blob& other )
    {
        area += other.area;
        sumX += other.sumX;
        sumY += other.sumY;

        if ( other.left < left )
        {
            left = other.left;
        }

        if ( other.right > right )
        {
            right = other.right;
        }

        if ( other.top < top )
        {
            top = other.top;
        }

        if ( other.bottom > bottom )
        {
            bottom = other.bottom;
        }
    }

    [[nodiscard]] core::Rectangle< int32_t > getBoundingRect( ) const
    {
        return { core::Point< int32_t, 2 >( left, top ),
                 core::Size( right - left + 1, bottom - top + 1 ) };
    }

    [[nodiscard]] core::Point< double, 2 > getCenter( ) const
    {
        return {
            core::Point< double, 2 >( sumX / static_cast< double >( area ),
                                      sumY / static_cast< double >( area ) ) };
    }

    [[nodiscard]] int32_t getArea( ) const { return area; }
};

/*
 * Function that check the 8 connected neighbourhood for labeling
 *
 * |x|x|x|
 * |x|c|0|
 * |0|0|0|
 *
 * @param [in]       x               The x position of the current pixel
 * @param [in]       y               The y position of the current pixel
 * @param [in]       refTable        The reference table containing already
 * applied label numbers.
 * @param [in out]   neighbourhood   The vector receiving the neighbours.
 */
template < typename PixelType, typename Allocator >
void checkNeighbourhood( const int32_t x, const int32_t y,
                         const core::Image< PixelType, 1, Allocator >& refTable,
                         std::vector< PixelType >& neighbourhood )
{
    // TODO: Make template for 4 or 8 connectivity. Make neighborhood as an enum
    // class
    const auto top = y - 1 >= 0 ? y - 1 : y;
    const auto left = x - 1 >= 0 ? x - 1 : x;
    const auto right = x + 1 < refTable.getWidth( ) ? x + 1 : x;

    const auto yPtr = refTable.getRowPointer( y );
    const auto topPtr = refTable.getRowPointer( top );

    neighbourhood[ 0 ] = yPtr[ left ];
    neighbourhood[ 1 ] = topPtr[ left ];
    neighbourhood[ 2 ] = topPtr[ x ];
    neighbourhood[ 3 ] = topPtr[ right ];
}

/*
 * Function that returns the smallest element that is non-zero
 *
 * |x|x|x|
 * |x|c|0|
 * |0|0|0|
 *
 * @param [in]   neighbourhood   The vector receiving the neighbours.
 *
 * @return Returns the smallest non-zero element if exists, else zero.
 */
template < typename PixelType >
PixelType smallestNeighbour( const std::vector< PixelType >& neighbourhood,
                             const std::vector< PixelType >& activeLabels )
{
    PixelType smallest = std::numeric_limits< PixelType >::max( );

    for ( const auto& elem : neighbourhood )
    {
        if ( elem > 0 )
        {
            const auto label =
                activeLabels[ static_cast< size_t >( elem - 1 ) ];

            if ( label < smallest )
            {
                smallest = label;
            }
        }
    }

    return smallest == std::numeric_limits< PixelType >::max( )
               ? PixelType { 0 }
               : smallest;
}

/*
 * Function that creates a new object list and updates the reference table
 *
 *
 * @param [in]       x              The x position of the current pixel
 * @param [in]       y              The y position of the current pixel
 * @param [in]       objects        The table of already created objects
 * @param [in out]   referenceTable The reference table.
 */
template < typename PixelType >
void createElement( const int32_t x, const int32_t y,
                    std::vector< std::shared_ptr< Blob > >& objects,
                    std::vector< PixelType >& activeLabels,
                    const PixelType labelNumber )
{
    objects.push_back( std::make_shared< Blob >( x, y, labelNumber ) );

    activeLabels.push_back( labelNumber );
}

/*
 * Function that adds a point to an existing object. NOTE that 0 is not a valid
 * label number. 0 means background.
 * The object is added to object at index labelNumber - 1
 *
 *
 * @param [in]       x              The x position of the current pixel
 * @param [in]       y              The y position of the current pixel
 * @param [in]       objects        The table of already created objects
 *
 */
template < typename PixelType >
void addElement( const int32_t x, const int32_t y, const PixelType labelNumber,
                 const std::vector< std::shared_ptr< Blob > >& objects )
{
    objects[ static_cast< size_t >( labelNumber - 1 ) ]->add( x, y );
}

/*
 * Function that merges elements together, if elements in the neighborhood
 have a different label number.
 *
 * @param [in]    neighbourhood  The vector receiving the neighbours.
 * @param [in]    newLabelNumber The label number to merge to.
 * @param [in]    objects        The table of already created objects.
 * @param [in]    referenceTable The reference table.
 *
 */
template < typename PixelType >
void mergeElement( const std::vector< PixelType >& neighbourhood,
                   const PixelType newLabelNumber,
                   std::vector< std::shared_ptr< Blob > >& objects,
                   std::vector< PixelType >& activeLabels )
{
    for ( const auto& elem : neighbourhood )
    {
        if ( elem > 0 )
        {
            const auto old_l = static_cast< size_t >( elem - 1 );
            const auto new_l = static_cast< size_t >( newLabelNumber - 1 );
            // Get the real label number of the current object, it could already
            // be merged
            const auto realElemNum = activeLabels[ old_l ];

            if ( realElemNum != newLabelNumber )
            {
                if ( objects[ old_l ] == objects[ new_l ] )
                    continue;

                objects[ new_l ]->merge( *objects[ old_l ] );

                // All active labels needs to be changed to the merged one
                for ( size_t i = 0; i < activeLabels.size( ); i++ )
                {
                    if ( activeLabels[ i ] == realElemNum )
                    {
                        activeLabels[ i ] = newLabelNumber;
                        objects[ i ] = objects[ new_l ];
                    }
                }
            }
        }
    }
}

template < typename PixelType, typename Allocator,
           template < typename > typename... RegionFeature >
struct ConnectedComponentsDetector
{
    static std::vector< std::unique_ptr<
        core::Region< PixelType,
                      typename std::allocator_traits<
                          Allocator >::template rebind_alloc< PixelType >,
                      RegionFeature... > > >
    connection(
        [[maybe_unused]] const core::Region< uint8_t, Allocator >& regionIn )
    {
        THROW_MSG( "Unsupported PixelType" );
    }

    static std::vector< std::unique_ptr<
        core::Region< PixelType,
                      typename std::allocator_traits<
                          Allocator >::template rebind_alloc< PixelType >,
                      RegionFeature... > > >
    connection(
        const core::Image< uint8_t, 1, Allocator >& imageIn,
        core::Image< PixelType, 1,
                     typename std::allocator_traits< Allocator >::
                         template rebind_alloc< PixelType > >& labelImageOut )
    {
        using allocator_traits = std::allocator_traits< Allocator >;
        using OutAllocator =
            typename allocator_traits::template rebind_alloc< PixelType >;

        const auto width = imageIn.getWidth( );
        const auto height = imageIn.getHeight( );

        // Use same allocator for vectors as well
        std::vector< std::shared_ptr< Blob > > objects;
        std::vector< PixelType > activeLabels;

        PixelType labelNumber = 0;
        std::vector< PixelType > neighbourhood( 4 );

        // Check if the label image is already big enough
        if ( imageIn.getSize( ) != labelImageOut.getSize( ) )
        {
            labelImageOut = core::Image< PixelType, 1, OutAllocator >(
                width, height, true );
        }

        for ( int32_t y = 0; y < height; y++ )
        {
            const auto rowPtrSrc = imageIn.getRowPointer( y );
            const auto rowPtrLbl = labelImageOut.getRowPointer( y );

            for ( int32_t x = 0; x < width; x++ )
            {
                // NOTE: For connected component labeling we do not expect all
                // pixels to be a foreground pixel. Casting the row pointer to a
                // 64 bit pointer and checking for 0 can improve performance for
                // images with less information.

                // Check 8 pixels at the same time
                if ( x + 7 < width &&
                     ! *reinterpret_cast< const uint64_t* >( rowPtrSrc + x ) )
                {
                    x += 7;
                    continue;
                }

                if ( rowPtrSrc[ x ] != 0 )
                {
                    PixelType currentLabelNumber { };
                    checkNeighbourhood( x, y, labelImageOut, neighbourhood );

                    const auto minLabel =
                        smallestNeighbour( neighbourhood, activeLabels );

                    if ( minLabel == 0 )
                    {
                        currentLabelNumber = ++labelNumber;
                        rowPtrLbl[ x ] = currentLabelNumber;
                        createElement(
                            x, y, objects, activeLabels, currentLabelNumber );
                    }
                    else
                    {
                        currentLabelNumber = minLabel;
                        rowPtrLbl[ x ] = currentLabelNumber;
                        addElement( x, y, minLabel, objects );

                        mergeElement(
                            neighbourhood, minLabel, objects, activeLabels );
                    }
                }
            }
        }

        // Reassign label numbers in label image
        for ( int32_t y = 0; y < height; y++ )
        {
            const auto rowPtrSrc = imageIn.getRowPointer( y );
            const auto rowPtrLbl = labelImageOut.getRowPointer( y );

            for ( int32_t x = 0; x < width; x++ )
            {
                // NOTE: For connected component labeling we do not expect all
                // pixels to be a foreground pixel. Casting the row pointer to a
                // 64 bit pointer and checking for 0 can improve performance for
                // images with less information.

                // Check 8 pixels at the same time
                if ( x + 7 < width &&
                     ! *reinterpret_cast< const uint64_t* >( rowPtrSrc + x ) )
                {
                    x += 7;
                    continue;
                }

                if ( rowPtrSrc[ x ] != 0 )
                {
                    rowPtrLbl[ x ] = activeLabels[ static_cast< size_t >(
                        rowPtrLbl[ x ] - 1 ) ];
                }
            }
        }

        std::vector< std::unique_ptr<
            core::Region< PixelType, OutAllocator, RegionFeature... > > >
            regionsOut;

        // Create objects
        for ( size_t i = 0; i < activeLabels.size( ); i++ )
        {
            if ( activeLabels[ i ] != static_cast< int32_t >( i ) + 1 )
                continue;

            // TODO: Maybe store Blob Object in region as reference for the
            // bounding rect. Otherwise, some basic features needs to be
            // calculated twice.

            if constexpr ( hasGetArea< core::Region< PixelType,
                                                     OutAllocator,
                                                     RegionFeature... > > )
            {
                int test = 1;
            }

            regionsOut.push_back(
                std::make_unique<
                    core::Region< PixelType, OutAllocator, RegionFeature... > >(
                    labelImageOut, activeLabels[ i ] ) );
        }

        return regionsOut;
    }
};

} // namespace detail

/**
 * Function that performs connected component labeling on binary images
 *
 * @param [in]   imageIn        The input image
 * @param [in]   labelImageOut  The labeled output image
 *
 * NOTE: The labelImageOut acts as buffer for the labeling. If it is
 * pre-allocated with the same size as the input image, the function do not
 * allocate image buffer.
 *
 * @return Returns the resulting output regions
 */
template < typename PixelType, typename Allocator,
           template < typename > typename... RegionFeature >
std::vector<
    std::unique_ptr< core::Region< PixelType,
                                   typename std::allocator_traits< Allocator >::
                                       template rebind_alloc< PixelType >,
                                   RegionFeature... > > >
connectedComponents(
    const core::Image< uint8_t, 1, Allocator >& imageIn,
    core::Image< PixelType, 1,
                 typename std::allocator_traits< Allocator >::
                     template rebind_alloc< PixelType > >& labelImageOut )
{
    return detail::ConnectedComponentsDetector<
        PixelType,
        Allocator,
        RegionFeature... >::connection( imageIn, labelImageOut );
}

} // namespace cvl::processing