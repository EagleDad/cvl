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
    Image( ) = default;

    /**
     * Value construct
     *
     * @brief The constructor creates an image with a memory resource to use for
     * allocation.
     *
     * @param resource  The memory resource to use.
     */
    Image( std::pmr::memory_resource* resource );

    /**
     * Value construct
     *
     * @brief The constructor creates an image with the defined dimensions
     *
     * @param width             The width of the image
     * @param height            The height of the image
     * @param zeroInitialize    Flag that signals if the buffer should be
     * initialized
     */
    Image( int32_t width, int32_t height, bool zeroInitialize = false );

    /**
     * Value construct
     *
     * @brief The constructor creates an image with the defined dimensions
     *
     * @param size              The size of the image
     * @param zeroInitialize    Flag that signals if the buffer should be
     * initialized
     */
    Image( const SizeI& size, bool zeroInitialize = false );

    /**
     * Value construct
     *
     * @brief The constructor creates an image with the given dimensions.
     * The data for the image will be allocated and managed. The buffer will be
     * copied to the internal memory.
     *
     * @param width   The width of the image
     * @param height  The height of the image
     * @param data    The pointer to the data
     * @param stride  The step size of each row
     */
    Image( int32_t width, int32_t height, void* data, int32_t stride );

    /**
     * Copy constructor
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
     * @param other    The other image
     * @param roi      The roi to use
     */
    Image( const Image& other, const Rectangle< int32_t >& roi );

    /**
     * Destructor
     */
    ~Image( ) override;

    /**
     * Assignment operator
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
     * Accessor stride in byted
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
     * Allocation function
     *
     * @brief The function allocates the image buffer, if memory is not
     * sufficient. The data for the image will be allocated and managed. If set,
     * the buffer is zero initialized, else it contains random values.
     *
     * @param width             The width of the image
     * @param height            The height of the image
     * @param zeroInitialize    The pointer to the data
     */
    constexpr void allocate( int32_t width, int32_t height,
                             bool zeroInitialize = false );

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

    constexpr void deallocate( );

    [[nodiscard]] constexpr size_t positionOffset( int32_t row, int32_t column,
                                                   int32_t channel ) const;

    int32_t mStride { };
    SizeI mSize;
    pointer_type mData { nullptr };
    allocator_type mAllocator { };
    bool mReleaseMemory { true };
};

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
Image< PixelType, Channels, Allocator >::Image(
    std::pmr::memory_resource* resource )
    : mAllocator( resource )
{
}

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
Image< PixelType, Channels, Allocator >::Image(
    int32_t width, int32_t height, bool zeroInitialize /* = false*/ )
    : mStride( alignTo( width * static_cast< int32_t >( sizeof( PixelType ) ),
                        static_cast< int32_t >( width_alignment ) ) )
    , mSize( width, height )
{
    mData = allocator_traits::allocate( mAllocator,
                                        static_cast< size_t >( getStride( ) ) *
                                            mSize.getHeight( ) * Channels );

    if ( zeroInitialize )
    {
        for ( size_t i = 0; i < static_cast< size_t >( getStride( ) ) *
                                    mSize.getHeight( ) * Channels;
              i++ )
        {
            allocator_traits::construct( mAllocator, mData + i, PixelType( ) );
        }
    }
}

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
Image< PixelType, Channels, Allocator >::Image(
    const SizeI& size, bool zeroInitialize /* = false*/ )
    : Image( size.getWidth( ), size.getHeight( ), zeroInitialize )
{
}

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
Image< PixelType, Channels, Allocator >::Image( int32_t width, int32_t height,
                                                void* data, int32_t stride )
    : mStride( stride )
    , mSize( width, height )
    , mData( static_cast< pointer_type >( data ) )
    , mReleaseMemory( false )
{
}

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
Image< PixelType, Channels, Allocator >::Image( const Image& other )

{
    other.copyTo( *this );
    /*mData = allocator_traits::allocate( mAllocator,
                                        static_cast< size_t >( getStride( ) ) *
                                            mSize.getHeight( ) * Channels );

    std::memcpy( mData,
                 other.mData,
                 static_cast< size_t >( mStride ) * mSize.getHeight( ) *
                     Channels );*/
}

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
Image< PixelType, Channels, Allocator >::Image( Image&& other ) noexcept
    : mStride( other.mStride )
    , mSize( std::move( other.mSize ) )
    , mData( std::move( other.mData ) )
    , mReleaseMemory( other.mReleaseMemory )
{
    // Invalidate the date of the moved object.
    other.mData = nullptr;
}

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
Image< PixelType, Channels, Allocator >::Image(
    const Image& other, const Rectangle< int32_t >& roi )
    : mStride( other.mStride )
    , mSize( roi.getSize( ) )
    , mReleaseMemory( false )
{
    // Memory is organized in a monotonic buffer
    // In case of a multichannel image the data is organized as follows:
    // R G B R G B ...
    // R G B R G B ...
    // ...

    // Since the pointer is a typed pointer we need to have the offset in normal
    // offset, not in bytes. Otherwise, we move too far.
    const auto typeStride = mStride / sizeof( PixelType );
    const auto left = roi.getLeft( );
    const auto top = roi.getTop( );

    const auto offset = typeStride * top + left;

    mData = other.mData + offset;
}

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
Image< PixelType, Channels, Allocator >::~Image( )
{
    if ( mData != nullptr && mReleaseMemory )
    {
        if constexpr ( isPolymorphicAllocator< Allocator > )
        {
            mAllocator.deallocate_bytes( static_cast< void* >( mData ),
                                         static_cast< size_t >( getStride( ) ) *
                                             mSize.getHeight( ) * Channels,
                                         64 );
            /* mAllocator.resource( )->deallocate(
                 static_cast< void* >( mData ),
                 static_cast< size_t >( getStride( ) ) * mSize.getHeight( ),
                 64 );*/
        }
        else
        {
            allocator_traits::deallocate(
                mAllocator,
                mData,
                static_cast< size_t >( getStride( ) ) * mSize.getHeight( ) *
                    Channels );
        }
    }

    mData = { nullptr };
}

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

        // Invalidate the date of the moved object.
        other.mData = nullptr;
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
        const auto bufferSize =
            static_cast< size_t >( mStride ) *
            static_cast< size_t >( mSize.getHeight( ) * Channels );

        if ( std::memcmp( this->mData, other.mData, bufferSize ) != 0 )
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

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
constexpr int32_t Image< PixelType, Channels, Allocator >::getWidth( ) const
{
    return mSize.getWidth( );
}

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
constexpr int32_t Image< PixelType, Channels, Allocator >::getStride( ) const
{
    return mStride / static_cast< int32_t >( sizeof( PixelType ) );
}

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
constexpr int32_t
Image< PixelType, Channels, Allocator >::getStrideInBytes( ) const
{
    return mStride;
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
void Image< PixelType, Channels, Allocator >::swap( Image& other ) noexcept
{
    std::swap( this->mStride, other.mStride );
    std::swap( this->mSize, other.mSize );
    std::swap( this->mData, other.mData );
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
    const auto typeStride = mStride / sizeof( PixelType );

    const auto channelOffset = typeStride * mSize.getHeight( ) * channel;

    return channelOffset + typeStride * static_cast< size_t >( row ) +
           static_cast< size_t >( column );
}

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
constexpr void Image< PixelType, Channels, Allocator >::allocate(
    int32_t width, int32_t height, bool zeroInitialize )
{
    if ( mSize == Size( width, height ) )
    {
        return;
    }

    deallocate( );

    mStride = alignTo( width * static_cast< int32_t >( sizeof( PixelType ) ),
                       static_cast< int32_t >( width_alignment ) );
    mSize = { width, height };

    if constexpr ( isPolymorphicAllocator< Allocator > )
    {
        /*const auto memoryResource = mAllocator.resource( );
        std::ignore = memoryResource;
        memoryResource->allocate(
            static_cast< size_t >( getStride( ) ) * mSize.getHeight( ), 64 );*/

        /* mData = static_cast< PixelType* >( mAllocator.resource( )->allocate(
             static_cast< size_t >( getStride( ) ) * mSize.getHeight( ), 64 )
           );*/

        mData = static_cast< PixelType* >(
            mAllocator.allocate_bytes( static_cast< size_t >( getStride( ) ) *
                                           mSize.getHeight( ) * Channels,
                                       64 ) );
    }
    else
    {
        mData =
            allocator_traits::allocate( mAllocator,
                                        static_cast< size_t >( getStride( ) ) *
                                            mSize.getHeight( ) * Channels );
    }

    if ( zeroInitialize )
    {
        for ( size_t i = 0; i < static_cast< size_t >( getStride( ) ) *
                                    mSize.getHeight( ) * Channels;
              i++ )
        {
            allocator_traits::construct( mAllocator, mData + i, PixelType( ) );
        }
    }
}

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
constexpr void Image< PixelType, Channels, Allocator >::deallocate( )
{
    if ( mData != nullptr && mReleaseMemory )
    {
        if constexpr ( isPolymorphicAllocator< Allocator > )
        {
            mAllocator.deallocate_bytes( static_cast< void* >( mData ),
                                         static_cast< size_t >( getStride( ) ) *
                                             mSize.getHeight( ) * Channels,
                                         64 );
        }
        else
        {
            allocator_traits::deallocate(
                mAllocator,
                mData,
                static_cast< size_t >( getStride( ) ) * mSize.getHeight( ) *
                    Channels );
        }
    }

    mData = { nullptr };
}

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
constexpr typename Image< PixelType, Channels, Allocator >::allocator_type
Image< PixelType, Channels, Allocator >::getAllocator( ) const
{
    return mAllocator;
}

template < Arithmetic PixelTypeOut, Arithmetic PixelTypeIn, int32_t Channels,
           typename Allocator >
Image< PixelTypeOut, Channels,
       typename std::allocator_traits< Allocator >::template rebind_alloc<
           PixelTypeOut > >
getCalculationImage(
    [[maybe_unused]] const Image< PixelTypeIn, Channels, Allocator >& imageIn,
    bool zeroInitialize = false )
{
    using allocator_traits = std::allocator_traits< Allocator >;
    using OutAllocator =
        typename allocator_traits::template rebind_alloc< PixelTypeOut >;

    if constexpr ( isPolymorphicAllocator< Allocator > )
    {
        auto image = core::Image< PixelTypeOut, Channels, OutAllocator >(
            imageIn.getAllocator( ).resource( ) );
        image.allocate(
            imageIn.getWidth( ), imageIn.getHeight( ), zeroInitialize );
        return image;
    }
    else
    {
        return core::Image< PixelTypeOut, Channels, OutAllocator >(
            imageIn.getWidth( ), imageIn.getHeight( ), zeroInitialize );
    }
}

template < Arithmetic PixelTypeOut, Arithmetic PixelTypeIn, int32_t Channels,
           typename Allocator >
Image< PixelTypeOut, Channels,
       typename std::allocator_traits< Allocator >::template rebind_alloc<
           PixelTypeOut > >
getCalculationImage(
    [[maybe_unused]] const Image< PixelTypeIn, Channels, Allocator >& imageIn,
    const cvl::core::SizeI& size, bool zeroInitialize = false )
{
    using allocator_traits = std::allocator_traits< Allocator >;
    using OutAllocator =
        typename allocator_traits::template rebind_alloc< PixelTypeOut >;

    if constexpr ( isPolymorphicAllocator< Allocator > )
    {
        auto image = core::Image< PixelTypeOut, Channels, OutAllocator >(
            imageIn.getAllocator( ).resource( ) );
        image.allocate( size.getWidth( ), size.getHeight( ), zeroInitialize );
        return image;
    }
    else
    {
        return core::Image< PixelTypeOut, Channels, OutAllocator >(
            size, zeroInitialize );
    }
}

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
void Image< PixelType, Channels, Allocator >::copyTo( Image& other ) const
{
    if ( other.mSize != mSize )
    {
        other = Image( mSize, false );
    }

    if ( other.mStride != mStride )
    {
        for ( int32_t y = 0; y < mSize.getHeight( ); y++ )
        {
            const auto dataSize = mSize.getWidth( ) * sizeof( PixelType );

            const auto srcPtr = this->getRowPointer( y );
            const auto dstPtr = other.getRowPointer( y );

            std::memcpy( dstPtr, srcPtr, dataSize );
        }
    }
    else
    {
        std::memcpy( other.mData,
                     mData,
                     static_cast< size_t >( mStride ) *
                         static_cast< size_t >( mSize.getHeight( ) ) );
    }
}

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
Image< PixelType, Channels, Allocator >
Image< PixelType, Channels, Allocator >::clone( ) const
{
    return Image( *this );
}

using Image8UC1 = Image< uint8_t, 1 >;
using Image16SC1 = Image< int16_t, 1 >;
using Image16UC1 = Image< uint16_t, 1 >;
using Image32SC1 = Image< int32_t, 1 >;

} // namespace cvl::core