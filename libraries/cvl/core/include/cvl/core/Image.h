#pragma once

// STD includes
#include <cstddef>
#include <cstdint>
#include <memory>

// CVL includes
#include <cvl/core/AlignedAllocator.h>
#include <cvl/core/Alignment.h>
#include <cvl/core/ImageTraits.h>
#include <cvl/core/Rectangle.h>
#include <cvl/core/Size.h>
#include <cvl/core/Types.h>
#include <cvl/core/macros.h>

namespace cvl::core
{

constexpr std::size_t width_alignment = 1;

class IImage
{
    CVL_INTERFACE( IImage );
};

/**
 * @brief This class describes a generic image class.
 * The data is managed by a cv::Mat, or passed by pointer
 * from an external resource
 */
template < Arithmetic PixelType, int32_t Channels,
           typename Allocator = AlignedAllocator< PixelType > >
class Image : public IImage
{
public:
    using value_type = PixelType;
    using reference_type = PixelType&;
    using pointer_type = PixelType*;
    using const_reference_type = const PixelType&;
    using allocator_type = Allocator;
    using allocator_traits = std::allocator_traits< allocator_type >;

    /**
     * Default constructor
     */
    Image( ) noexcept = default;

    /**
     * Value constructor
     *
     * @brief The constructor creates an empty image and uses th allocator
     * passed for memory allocation.
     *
     * @param allocator     The allocator to be used for memory allocation.
     */
    explicit Image( const Allocator& allocator ) noexcept;

    /**
     * Value construct
     *
     * @brief The constructor creates an image with the defined dimensions
     *
     * @param width             The width of the image
     * @param height            The height of the image
     * @param zeroInitialize    Flag that signals if the buffer should be
     *                          initialized
     *  @param allocator        The allocator object to be used
     */
    Image( int32_t width, int32_t height, bool zeroInitialize = false,
           const Allocator& allocator = Allocator( ) );

    /**
     * Value construct
     *
     * @brief The constructor creates an image with the defined dimensions
     *
     * @param size              The size of the image
     * @param zeroInitialize    Flag that signals if the buffer should be
     *                          initialized
     * @param allocator         The allocator object to be used
     */
    Image( const SizeI& size, bool zeroInitialize = false,
           const Allocator& allocator = Allocator( ) );

    /**
     * Value construct
     *
     * @brief The constructor creates an image with the given dimensions.
     * The data for the image will be allocated and managed. The buffer will be
     * copied to the internal memory.
     *
     * @param width         The width of the image
     * @param height        The height of the image
     * @param data          The pointer to the data
     * @param stride        The step size of each row in bytes
     * @param allocator     The allocator object to be used
     */
    Image( int32_t width, int32_t height, void* data, int32_t stride,
           const Allocator& allocator = Allocator( ) );

    /**
     * Copy constructor
     *
     * @brief The copy constructor creates a deep copy to have a clear owner
     * ship to the memory. If a shallow copy is required call the value
     * constructor that takes the buffer as input.
     *
     * @param other The image to copy from
     */
    Image( const Image& other );

    /**
     * Move constructor
     *
     * @param [in]  other     The image to move
     */
    Image( Image&& other ) noexcept;

    /**
     * Value construct
     *
     * @brief The constructor creates a cv::Mat using a ROI from an other image
     *
     * @param other         The other image
     * @param roi           The roi to use
     * @param allocator     The allocator object to be used
     * @param allocator     The allocator object to be used
     */
    Image( const Image& other, const Rectangle< int32_t >& roi,
           const Allocator& allocator = Allocator( ) );

    /**
     * Destructor
     */
    ~Image( ) override;

    /**
     * Assignment operator
     *
     * @brief The assignment operator creates a deep copy to have a clear owner
     * ship to the memory. If a shallow copy is required call the value
     * constructor that takes the buffer as input.
     *
     * @param [in]  other     The image to assign from
     */
    constexpr Image& operator=( const Image& other ) noexcept;

    /**
     * Move operator
     *
     * @param [in]  other  The image to move from
     */
    constexpr Image& operator=( Image&& other ) noexcept;

    /**
     * Equal operator
     *
     * @param [in]  other  The point to compare
     */
    constexpr bool operator==( const Image& other ) const noexcept;

    /**
     * Not equal operator
     *
     * @param [in]  other  The point to compare with
     */
    constexpr bool operator!=( const Image& other ) const noexcept;

    /**
     * ROI operator
     *
     * @param [in]  roi  The ROI to crop
     */
    Image operator( )( const Rectangle< int32_t >& roi ) const;

    /**
     * Accessor width
     *
     * @returns The width of the image
     */
    [[nodiscard]] constexpr int32_t getWidth( ) const;

    /**
     * Accessor stride
     *
     * @returns The normalized stride of the image. Stride divided by tge
     * element size.
     */
    [[nodiscard]] constexpr int32_t getStride( ) const;

    /**
     * Accessor stride in bytes
     *
     * @returns The stride of the image in bytes.
     */
    [[nodiscard]] constexpr int32_t getStrideInBytes( ) const;

    /**
     * Accessor height
     *
     * @returns The height of the image
     */
    [[nodiscard]] constexpr int32_t getHeight( ) const;

    /**
     * Accessor size
     *
     * @returns The size of the image
     */
    [[nodiscard]] constexpr SizeI getSize( ) const;

    /**
     * Accessor image data
     *
     * @returns A shared pointer to the image data
     */
    [[nodiscard]] constexpr pointer_type getData( ) const;

    /**
     * Accessor for row pointer
     *
     * @param [in]  row       The row to access
     * @param [in]  channel   The channel to access
     *
     * @returns the pointer to the row of the defined channel
     */
    [[nodiscard]] constexpr pointer_type
    getRowPointer( int32_t row, int32_t channel = 0 ) const;

    /**
     * Accessor for pixel
     *
     * @param [in]  row       The row to access
     * @param [in]  column    The column to access
     * @param [in]  channel   The channel to access
     *
     * @returns the reference to a pixel
     */
    [[nodiscard]] constexpr reference_type at( int32_t row, int32_t column,
                                               int32_t channel = 0 );

    /**
     * Accessor for pixel
     *
     * @param [in]  row       The row to access
     * @param [in]  column    The column to access
     * @param [in]  channel   The channel to access
     *
     * @returns the reference to a pixel
     */
    [[nodiscard]] constexpr const_reference_type
    at( int32_t row, int32_t column, int32_t channel = 0 ) const;

    /**
     * Returns the used allocator
     *
     * @brief The function returns the used allocator.
     *
     * @returns the allocator.
     */
    constexpr allocator_type getAllocator( ) const;

    /**
     *
     * @brief Copies the content of an image to an other image.
     *
     * @param other    The image to be copied
     */
    void copyTo( Image& other ) const;

    /**
     * Function that returns a deep copy of an image.
     *
     * @return The clones image.
     */
    [[nodiscard]] Image clone( ) const;

private:
    void swap( Image& other ) noexcept;

    constexpr void allocate( );
    constexpr void deallocate( );
    constexpr size_t numberElements( ) const;

    [[nodiscard]] constexpr size_t positionOffset( int32_t row, int32_t column,
                                                   int32_t channel ) const;

    int32_t mStride { };
    SizeI mSize;
    pointer_type mData { nullptr };
    allocator_type mAllocator { };
    bool mReleaseMemory { true };
};

//
// Public Methods
//

//
// Construction
//

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
Image< PixelType, Channels, Allocator >::Image(
    const Allocator& allocator ) noexcept
    : mAllocator( allocator )
{
}

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
Image< PixelType, Channels, Allocator >::Image(
    int32_t width, int32_t height, bool zeroInitialize /* = false*/,
    const Allocator& allocator /*= Allocator( )*/ )
    : mStride( alignTo( width, static_cast< int32_t >( width_alignment ) ) )
    , mSize( width, height )
    , mAllocator( allocator )
{
    allocate( );

    if ( zeroInitialize )
    {
        std::memset( mData, 0x00, numberElements( ) * sizeof( PixelType ) );
    }
}

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
Image< PixelType, Channels, Allocator >::Image(
    const SizeI& size, bool zeroInitialize /* = false*/,
    const Allocator& allocator /*= Allocator( )*/ )
    : Image( size.getWidth( ), size.getHeight( ), zeroInitialize, allocator )
{
}

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
Image< PixelType, Channels, Allocator >::Image(
    int32_t width, int32_t height, void* data, int32_t stride,
    const Allocator& allocator /*= Allocator( )*/ )
    : mStride( stride / sizeof( PixelType ) )
    , mSize( width, height )
    , mData( static_cast< pointer_type >( data ) )
    , mAllocator( allocator )
    , mReleaseMemory( false )
{
}

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
Image< PixelType, Channels, Allocator >::Image( const Image& other )
    //    : mAllocator( other.mAllocator )
    : mStride( alignTo( other.getWidth( ),
                        static_cast< int32_t >( width_alignment ) ) )
    , mSize( other.getWidth( ), other.getHeight( ) )
    , mAllocator( other.mAllocator )
{
    // other.copyTo( *this );

    allocate( );

    other.copyTo( *this );

    // std::memcpy(
    //     mData, other.mData, other.numberElements( ) * sizeof( PixelType ) );
}

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
Image< PixelType, Channels, Allocator >::Image( Image&& other ) noexcept
    : mStride( other.mStride )
    , mSize( std::move( other.mSize ) )
    , mData( std::move( other.mData ) )
    , mAllocator( other.mAllocator )
    , mReleaseMemory( other.mReleaseMemory )
{
    // Invalidate the date of the moved object.
    other.mData = nullptr;
}

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
Image< PixelType, Channels, Allocator >::Image(
    const Image& other, const Rectangle< int32_t >& roi,
    const Allocator& allocator /*= Allocator( )*/ )
    : mStride( other.mStride )
    , mSize( roi.getSize( ) )
    , mAllocator( allocator )
    , mReleaseMemory( false )
{
    // Memory is organized in a monotonic buffer
    // In case of a multichannel image the data is organized as follows:
    // R G B R G B ...
    // R G B R G B ...
    // ...

    // Since the pointer is a typed pointer we need to have the offset in normal
    // offset, not in bytes. Otherwise, we move too far.
    const auto typeStride = mStride;
    const auto left = roi.getLeft( );
    const auto top = roi.getTop( );

    const auto offset = typeStride * top + left;

    mData = other.mData + offset;
}

//
// Destruction
//

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
Image< PixelType, Channels, Allocator >::~Image( )
{
    deallocate( );
}

//
// Operators
//

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
constexpr Image< PixelType, Channels, Allocator >&
Image< PixelType, Channels, Allocator >::operator=(
    const Image& other ) noexcept
{
    if ( this != &other )
    {
        Image tmp( other );
        tmp.swap( *this );
    }

    return *this;
}

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
constexpr Image< PixelType, Channels, Allocator >&
Image< PixelType, Channels, Allocator >::operator=( Image&& other ) noexcept
{
    if ( this != &other )
    {
        // Transfer ownership
        this->mStride = other.mStride;
        this->mSize = other.mSize;
        this->mData = std::move( other.mData );
        // this->mAllocator = other.mAllocator;

        // Invalidate the date of the moved object.
        other.mData = nullptr;
        this->mReleaseMemory = false;

        // std::pmr::allocator cannot be moved. How to handle this here??

        if constexpr ( std::is_move_assignable_v< Allocator > )
        {
            this->mAllocator = std::move( other.mAllocator );
        }
        else if constexpr ( std::is_assignable_v< Allocator, Allocator > )
        {
            this->mAllocator = other.mAllocator;
        }
        else
        {
            /* std::exchange( this->mAllocator, other.mAllocator );
             using _Ptrval = typename Allocator::value_type*;*/
        }
    }

    return *this;
}

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
constexpr bool Image< PixelType, Channels, Allocator >::operator==(
    const Image& other ) const noexcept
{
    if ( this->mStride != other.mStride )
    {
        return false;
    }

    if ( this->mSize != other.mSize )
    {
        return false;
    }

    if ( this->mData != nullptr && other.mData != nullptr )
    {
        if ( std::memcmp( this->mData,
                          other.mData,
                          numberElements( ) * sizeof( PixelType ) ) != 0 )
        {
            return false;
        }
    }

    return true;
}

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
constexpr bool Image< PixelType, Channels, Allocator >::operator!=(
    const Image& other ) const noexcept
{
    return ! operator==( other );
}

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
Image< PixelType, Channels, Allocator >
Image< PixelType, Channels, Allocator >::operator( )(
    const Rectangle< int32_t >& roi ) const
{
    return Image( *this, roi );
}

// Methods

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
constexpr int32_t Image< PixelType, Channels, Allocator >::getWidth( ) const
{
    return mSize.getWidth( );
}

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
constexpr int32_t Image< PixelType, Channels, Allocator >::getStride( ) const
{
    return mStride;
}

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
constexpr int32_t
Image< PixelType, Channels, Allocator >::getStrideInBytes( ) const
{
    return mStride * static_cast< int32_t >( sizeof( PixelType ) );
}

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
constexpr int32_t Image< PixelType, Channels, Allocator >::getHeight( ) const
{
    return mSize.getHeight( );
}

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
constexpr SizeI Image< PixelType, Channels, Allocator >::getSize( ) const
{
    return mSize;
}

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
constexpr typename Image< PixelType, Channels, Allocator >::pointer_type
Image< PixelType, Channels, Allocator >::getData( ) const
{
    return mData;
}

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
constexpr typename Image< PixelType, Channels, Allocator >::pointer_type
Image< PixelType, Channels, Allocator >::getRowPointer(
    int32_t row, int32_t channel /*= 0*/ ) const
{
    return mData + positionOffset( row, 0, channel );
}

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
constexpr typename Image< PixelType, Channels, Allocator >::reference_type
Image< PixelType, Channels, Allocator >::at( int32_t row, int32_t column,
                                             int32_t channel /* = 0*/ )
{
    return mData[ static_cast< ptrdiff_t >(
        positionOffset( row, column, channel ) ) ];
}

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
constexpr typename Image< PixelType, Channels, Allocator >::const_reference_type
Image< PixelType, Channels, Allocator >::at( int32_t row, int32_t column,
                                             int32_t channel /* = 0*/ ) const
{
    return mData[ static_cast< ptrdiff_t >(
        positionOffset( row, column, channel ) ) ];
}

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
constexpr typename Image< PixelType, Channels, Allocator >::allocator_type
Image< PixelType, Channels, Allocator >::getAllocator( ) const
{
    return mAllocator;
}

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
void Image< PixelType, Channels, Allocator >::copyTo( Image& other ) const
{
    if ( other.mSize != mSize )
    {
        other = Image( mSize, false, mAllocator );
    }

    if ( other.mStride != mStride )
    {
        for ( int32_t c = 0; c < Channels; c++ )
        {
            for ( int32_t y = 0; y < mSize.getHeight( ); y++ )
            {
                const auto dataSize =
                    static_cast< size_t >( other.getStrideInBytes( ) );

                const auto srcPtr = this->getRowPointer( y, c );
                const auto dstPtr = other.getRowPointer( y, c );

                std::memcpy( dstPtr, srcPtr, dataSize );
            }
        }
    }
    else
    {
        std::memcpy(
            other.mData, mData, other.numberElements( ) * sizeof( PixelType ) );
    }
}

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
Image< PixelType, Channels, Allocator >
Image< PixelType, Channels, Allocator >::clone( ) const
{
    return Image( *this );
}

//
// Private methods
//

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
void Image< PixelType, Channels, Allocator >::swap( Image& other ) noexcept
{
    std::swap( this->mStride, other.mStride );
    std::swap( this->mSize, other.mSize );
    std::swap( this->mData, other.mData );
    std::swap( this->mReleaseMemory, other.mReleaseMemory );

    Allocator tmp = other.mAllocator;
    other.mAllocator = this->mAllocator;
    this->mAllocator = tmp;
}

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
constexpr void Image< PixelType, Channels, Allocator >::allocate( )
{
    // Note: The allocator is pixel type dependent. We need to pass a stride in
    // PixelType, not in bytes.
    mData = allocator_traits::allocate( mAllocator, numberElements( ) );
}

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
constexpr void Image< PixelType, Channels, Allocator >::deallocate( )
{
    if ( mReleaseMemory && mData != nullptr )
    {
        allocator_traits::deallocate( mAllocator, mData, numberElements( ) );
        mData = { nullptr };
    }
}

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
constexpr size_t
Image< PixelType, Channels, Allocator >::numberElements( ) const
{
    return static_cast< size_t >( getStride( ) ) * getHeight( ) * Channels;
}

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
constexpr size_t Image< PixelType, Channels, Allocator >::positionOffset(
    int32_t row, int32_t column, int32_t channel ) const
{
    // Store multi channel image in planes
    // c1 c1 c1
    // c1 c1 c1
    // c2 c2 c2
    // c2 c2 c2
    // c3 c3 c3
    // c3 c3 c3
    const auto channelOffset = getStride( ) * getHeight( ) * channel;

    return channelOffset + getStride( ) * static_cast< size_t >( row ) +
           static_cast< size_t >( column );
}

//
// Type definitions
//

using Image8UC1 = Image< uint8_t, 1 >;
using Image16SC1 = Image< int16_t, 1 >;
using Image16UC1 = Image< uint16_t, 1 >;
using Image32SC1 = Image< int32_t, 1 >;

} // namespace cvl::core