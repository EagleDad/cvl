#pragma once

// CVL includes
#include <cvl/core/Image.h>
#include <cvl/core/Point.h>
#include <cvl/core/Rectangle.h>
#include <cvl/core/RegionTraits.h>
#include <cvl/core/Types.h>

// STD includes
#include <memory>
#include <mutex>

namespace cvl::core
{

/**
 * @brief The Region class
 *
 * A region describes an object that contains a binary image.
 * The data type could be uint8_t or int32_t. The binary foreground
 * value is described by the label number. This is because the the region
 * could hold a reference image from a connected component analysis.
 * Having such image as reference it could be that within a Bounding Box of a
 * region two gray values are existing.
 *
 */
template < Arithmetic PixelType, typename Allocator,
           template < typename > typename... RegionFeature >
class Region : public RegionFeature<
                   Region< PixelType, Allocator, RegionFeature... > >...
{
public:
    using value_type = PixelType;
    using reference_type = PixelType&;
    using pointer_type = PixelType*;
    using const_reference_type = const PixelType&;
    using allocator_type = Allocator;
    using allocator_traits = std::allocator_traits< allocator_type >;

    using trait_type = RegionTypeTrait< PixelType, Allocator >;

    Region( ) = delete;

    Region( const Region& other ) = delete;

    Region( Region&& other ) noexcept = delete;

    constexpr Region& operator=( const Region& other ) noexcept = delete;

    constexpr Region& operator=( Region&& other ) noexcept = delete;

    virtual ~Region( ) = default;

    explicit Region(
        std::shared_ptr< Image< PixelType, 1, Allocator > > labelImage,
        int32_t labelNumber );

    explicit Region(
        std::shared_ptr< Image< PixelType, 1, Allocator > > labelImage,
        int32_t labelNumber, Rectangle< int32_t >&& boundingRect,
        Point< double, 2 >&& center, int32_t area );

    [[nodiscard]] constexpr int32_t getLabelNumber( ) const;

    [[nodiscard]] constexpr const std::shared_ptr<
        Image< PixelType, 1, Allocator > >&
    getLabelImage( ) const;

    [[nodiscard]] constexpr Rectangle< int32_t > getBoundingRect( );

    [[nodiscard]] constexpr Point< double, 2 > getCenter( );

    [[nodiscard]] constexpr int32_t getArea( );

private:
    void calculateBasicFeatures( );

private:
    int32_t mLabelNumber { };
    Rectangle< int32_t > mBoundingRect { };
    Point< double, 2 > mCenter { };
    int32_t mArea { };
    std::shared_ptr< Image< PixelType, 1, Allocator > > mLabelImage { nullptr };

    std::once_flag mFlagBaseFeatures;
};

template < Arithmetic PixelType, typename Allocator,
           template < typename > typename... RegionFeature >
Region< PixelType, Allocator, RegionFeature... >::Region(
    std::shared_ptr< Image< PixelType, 1, Allocator > > labelImage,
    int32_t labelNumber )
    : mLabelNumber( labelNumber )
    , mLabelImage( std::move( labelImage ) )
{
    EXPECT_MSG( mLabelImage != nullptr, "Invalid input image" );
}

template < Arithmetic PixelType, typename Allocator,
           template < typename > typename... RegionFeature >
Region< PixelType, Allocator, RegionFeature... >::Region(
    std::shared_ptr< Image< PixelType, 1, Allocator > > labelImage,
    int32_t labelNumber, Rectangle< int32_t >&& boundingRect,
    Point< double, 2 >&& center, int32_t area )
    : mLabelNumber( labelNumber )
    , mLabelImage( std::move( labelImage ) )
{
    EXPECT_MSG( mLabelImage != nullptr, "Invalid input image" );

    std::call_once( mFlagBaseFeatures,
                    [ this, &boundingRect, &center, &area ]
                    {
                        mBoundingRect = std::move( boundingRect );
                        mCenter = std::move( center );
                        mArea = area;
                    } );
}

template < Arithmetic PixelType, typename Allocator,
           template < typename > class... RegionFeature >
constexpr int32_t
Region< PixelType, Allocator, RegionFeature... >::getLabelNumber( ) const
{
    return mLabelNumber;
}

template < Arithmetic PixelType, typename Allocator,
           template < typename > class... RegionFeature >
constexpr const std::shared_ptr< Image< PixelType, 1, Allocator > >&
Region< PixelType, Allocator, RegionFeature... >::getLabelImage( ) const
{
    return mLabelImage;
}

template < Arithmetic PixelType, typename Allocator,
           template < typename > class... RegionFeature >
constexpr Rectangle< int32_t >
Region< PixelType, Allocator, RegionFeature... >::getBoundingRect( )
{
    std::call_once( mFlagBaseFeatures,
                    [ this ] { calculateBasicFeatures( ); } );
    return mBoundingRect;
}

template < Arithmetic PixelType, typename Allocator,
           template < typename > class... RegionFeature >
constexpr Point< double, 2 >
Region< PixelType, Allocator, RegionFeature... >::getCenter( )
{
    std::call_once( mFlagBaseFeatures,
                    [ this ] { calculateBasicFeatures( ); } );
    return mCenter;
}

template < Arithmetic PixelType, typename Allocator,
           template < typename > class... RegionFeature >
constexpr int32_t Region< PixelType, Allocator, RegionFeature... >::getArea( )
{
    std::call_once( mFlagBaseFeatures,
                    [ this ] { calculateBasicFeatures( ); } );
    return mArea;
}

template < Arithmetic PixelType, typename Allocator,
           template < typename > class... RegionFeature >
void Region< PixelType, Allocator, RegionFeature... >::calculateBasicFeatures( )
{
    int32_t moment00 { };
    int32_t moment01 { };
    int32_t moment10 { };
    int32_t rowMin { std::numeric_limits< int32_t >::max( ) };
    int32_t columnMin { std::numeric_limits< int32_t >::max( ) };
    int32_t rowMax { };
    int32_t columnMax { };

    auto calculate = [ &moment00,
                       &moment01,
                       &moment10,
                       &rowMin,
                       &columnMin,
                       &rowMax,
                       &columnMax ]( const int32_t& y, const int32_t& x )
    {
        moment00++;
        moment10 += x;
        moment01 += y;

        if ( y < rowMin )
        {
            rowMin = y;
        }

        if ( x < columnMin )
        {
            columnMin = x;
        }

        if ( y > rowMax )
        {
            rowMax = y;
        }

        if ( x > columnMax )
        {
            columnMax = x;
        }
    };

    const auto width = mLabelImage->getWidth( );
    const auto height = mLabelImage->getHeight( );

    constexpr auto byteSize = sizeof( PixelType );
    constexpr auto stepSize =
        static_cast< int32_t >( sizeof( uint64_t ) / byteSize - 1 );

    for ( int32_t y = 0; y < height; y++ )
    {
        const auto rowPtrLbl = mLabelImage->getRowPointer( y );

        for ( int32_t x = 0; x < width; x++ )
        {
            // NOTE: For connected component labeling we do not expect all
            // pixels to be a foreground pixel. Casting the row pointer to a
            // 64 bit pointer and checking for 0 can improve performance for
            // images with less information.

            // Check 8 pixels at the same time
            if ( x + stepSize < width &&
                 ! *reinterpret_cast< const uint64_t* >( rowPtrLbl + x ) )
            {
                x += stepSize;
                continue;
            }

            if ( rowPtrLbl[ x ] == mLabelNumber )
            {
                calculate( y, x );
            }
        }
    }

    mBoundingRect = { core::Point< int32_t, 2 >( columnMin, rowMin ),
                      Size( columnMax - columnMin + 1, rowMax - rowMin + 1 ) };

    mArea = moment00;

    if ( moment00 > 0 )
    {
        mCenter = Point< double, 2 >( static_cast< double >( moment10 ) /
                                          static_cast< double >( moment00 ),
                                      static_cast< double >( moment01 ) /
                                          static_cast< double >( moment00 ) );
    }
}

using Region8U = Region< uint8_t, AlignedAllocator< uint8_t > >;
using Region16U = Region< uint16_t, AlignedAllocator< uint16_t > >;
using Region32S = Region< int32_t, AlignedAllocator< int32_t > >;

} // namespace cvl::core