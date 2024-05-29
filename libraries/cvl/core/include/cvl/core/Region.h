#pragma once

// CVL includes
#include <cvl/core/Image.h>
#include <cvl/core/RegionTraits.h>
#include <cvl/core/Types.h>

// STD includes

namespace cvl::core
{

class IRegion
{
    CVL_INTERFACE( IRegion );
};

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
template < Arithmetic PixelType,
           typename Allocator = AlignedAllocator< PixelType >,
           template < typename > typename... RegionFeature >
class Region final : public IRegion,
                     public RegionFeature<
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

    /**
     * Default constructor
     */
    Region( ) = default;

    /**
     * Value constructor
     *
     * @brief The constructor creates a region with a label image and a label
     * number.
     *
     * @param labelImage    The label image.
     * @param labelNumber   The label number.
     */
    explicit Region( Image< PixelType, 1, Allocator > labelImage,
                     int32_t labelNumber );

    /**
     * Value constructor
     *
     * @brief The constructor creates a region with a label image and a label
     * number.
     *
     * @param labelImage    The label image.
     * @param labelNumber   The label number.
     * @param args          The arguments for the CRTP mixin classes. Order
     *                      matters based on definition.
     */
    /*explicit Region( Image< PixelType, 1, Allocator > labelImage,
                     int32_t labelNumber, RegionFeature< Region >... args );*/

    /**
     * Copy constructor
     *
     * @brief The copy constructor creates a shallow copy of the region and the
     * underlying image. No memory is copied.
     *
     * @param other The region to copy from
     */
    Region( const Region& other );

    /**
     * Move constructor
     *
     * @param [in]  other     The region to move
     */
    Region( Region&& other ) noexcept;

    /**
     * Assignment operator
     *
     * @brief The assignment operator creates a shallow of the region. Nor
     * memory is copied of the underlying image.
     *
     * @param [in]  other     The region to assign from
     */
    constexpr Region& operator=( const Region& other ) noexcept;

    /**
     * Move operator
     *
     * @param [in]  other  The region to move from
     */
    constexpr Region& operator=( Region&& other ) noexcept;

    /**
     * Destructor
     */
    ~Region( ) override = default;

    /**
     * Accessor label image
     *
     * @returns The The label image
     */
    [[nodiscard]] constexpr const Image< PixelType, 1, Allocator >&
    getLabelImage( ) const;

    /**
     * Accessor label number
     *
     * @returns The The label number
     */
    [[nodiscard]] constexpr int32_t getLabelNumber( ) const;

private:
    /**
     * Function that swaps class members.
     */
    void swap( Region& other ) noexcept;

private:
    int32_t mLabelNumber { };
    Image< PixelType, 1, Allocator > mLabelImage;
};

//
// Public Methods
//

//
// Construction
//

template < Arithmetic PixelType, typename Allocator,
           template < typename > typename... RegionFeature >
Region< PixelType, Allocator, RegionFeature... >::Region(
    Image< PixelType, 1, Allocator > labelImage, int32_t labelNumber )
    : mLabelNumber( labelNumber )
    , mLabelImage( std::move( labelImage ) )
{
}

// template < Arithmetic PixelType, typename Allocator,
//            template < typename > typename... RegionFeature >
// Region< PixelType, Allocator, RegionFeature... >::Region(
//     Image< PixelType, 1, Allocator > labelImage, int32_t labelNumber,
//     RegionFeature< Region >... args )
//     : RegionFeature< Region >( args )...
//     , mLabelNumber( labelNumber )
//     , mLabelImage( std::move( labelImage ) )
//{
// }

template < Arithmetic PixelType, typename Allocator,
           template < typename > typename... RegionFeature >
Region< PixelType, Allocator, RegionFeature... >::Region( const Region& other )
    : mLabelNumber( other.mLabelNumber )
    , mLabelImage( other.mLabelImage )
{
}

template < Arithmetic PixelType, typename Allocator,
           template < typename > typename... RegionFeature >
Region< PixelType, Allocator, RegionFeature... >::Region(
    Region&& other ) noexcept
    : mLabelNumber( other.mLabelNumber )
    , mLabelImage( std::move( other.mLabelImage ) )
{
}

//
// Destruction
//

//
// Operators
//

template < Arithmetic PixelType, typename Allocator,
           template < typename > class... RegionFeature >
constexpr Region< PixelType, Allocator, RegionFeature... >&
Region< PixelType, Allocator, RegionFeature... >::operator=(
    const Region& other ) noexcept
{
    if ( this != &other )
    {
        Region tmp( other );
        tmp.swap( *this );
    }

    return *this;
}

template < Arithmetic PixelType, typename Allocator,
           template < typename > class... RegionFeature >
constexpr Region< PixelType, Allocator, RegionFeature... >&
Region< PixelType, Allocator, RegionFeature... >::operator=(
    Region&& other ) noexcept
{
    if ( this != &other )
    {
        this->mLabelNumber = other.mLabelNumber;
        this->mLabelImage = std::move( other.mLabelImage );
    }

    return *this;
}

//
// Methods
//

template < Arithmetic PixelType, typename Allocator,
           template < typename > class... RegionFeature >
constexpr int32_t
Region< PixelType, Allocator, RegionFeature... >::getLabelNumber( ) const
{
    return mLabelNumber;
}

template < Arithmetic PixelType, typename Allocator,
           template < typename > class... RegionFeature >
constexpr const Image< PixelType, 1, Allocator >&
Region< PixelType, Allocator, RegionFeature... >::getLabelImage( ) const
{
    return mLabelImage;
}

//
// Private methods
//
template < Arithmetic PixelType, typename Allocator,
           template < typename > class... RegionFeature >
void Region< PixelType, Allocator, RegionFeature... >::swap(
    Region& other ) noexcept
{
    std::swap( this->mLabelImage, other.mLabelImage );
    std::swap( this->mLabelNumber, other.mLabelNumber );
}

} // namespace cvl::core