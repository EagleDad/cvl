#pragma once

// STD includes
#include <cstddef>
#include <cstdint>
#include <memory>

// CVL includes
#include <cvl/core/AlignedAllocator.h>
#include <cvl/core/Alignment.h>
#include <cvl/core/Handle.h>
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
 * The data is managed by a shared handle and is deleted if no other instance
 * has access to the buffer anymore
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
    using memory_handle = std::shared_ptr< Handle >;

    /**
     * Default constructor
     */
    Image( ) noexcept = default;

    /**
     * Value constructor
     *
     * @brief The constructor creates an empty image and uses the allocator
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
     * @brief The constructor creates an image with the defined dimensions an
     * initialized with the passed value.
     *
     * @param width         The width of the image
     * @param height        The height of the image
     * @param value         The value to be used to initialize the buffer.
     * @param allocator     The allocator object to be used
     */
    Image( int32_t width, int32_t height, PixelType value,
           const Allocator& allocator = Allocator( ) );

    /**
     * Value construct
     *
     * @brief The constructor creates an image with the defined dimensions
     *
     * @param size              The size of the image
     * @param value             The value to be used to initialize the buffer.
     * @param allocator         The allocator object to be used
     */
    Image( const SizeI& size, PixelType value,
           const Allocator& allocator = Allocator( ) );

    /**
     * Value construct
     *
     * @brief The constructor creates an image with the given dimensions and
     * already allocated. The data for the image will not be allocated and not
     * be managed. If the allocation instance disappears accessing the buffer
     * will lead to undefined behaviour.
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
     * @brief The copy constructor creates a shallow copy of the image. No
     * memory is copied. If a deep copy is required call call clone.
     *
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
     * @brief The constructor creates an image using a ROI from an other image
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
    ~Image( ) override = default;

    /**
     * Assignment operator
     *
     * @brief The assignment operator creates a shallow of the image. Nor memory
     * is copied. If a deep copy is required call clone.
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
    constexpr Image operator( )( const Rectangle< int32_t >& roi ) const;

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
    [[nodiscard]] constexpr allocator_type getAllocator( ) const;

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
    /**
     * Function that swaps class members.
     */
    void swap( Image& other ) noexcept;

    /**
     * Function that allocates the required image buffer.
     */
    constexpr void allocate( );

    /**
     * Function that return the number of pixels.
     *
     * @return The number of pixels
     */
    [[nodiscard]] constexpr size_t numberElements( ) const;

    /**
     * Function that calculates the offset from the beginning of the buffer to
     * the defined position.
     *
     * @param [in]  row       The row to access
     * @param [in]  column    The column to access
     * @param [in]  channel   The channel to access
     *
     * @return The offset
     */
    [[nodiscard]] constexpr size_t positionOffset( int32_t row, int32_t column,
                                                   int32_t channel ) const;

    int32_t mStride { };
    SizeI mSize { };
    pointer_type mData { nullptr };
    allocator_type mAllocator { };
    memory_handle mMemoryHandle { nullptr };
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
    int32_t width, int32_t height, PixelType value,
    const Allocator& allocator /*= Allocator( )*/ )
    : mStride( alignTo( width, static_cast< int32_t >( width_alignment ) ) )
    , mSize( width, height )
    , mAllocator( allocator )
{
    allocate( );

    std::fill_n( mData, numberElements( ), value );
}

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
Image< PixelType, Channels, Allocator >::Image(
    const SizeI& size, PixelType value,
    const Allocator& allocator /*= Allocator( )*/ )
    : Image( size.getWidth( ), size.getHeight( ), value, allocator )
{
}

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
Image< PixelType, Channels, Allocator >::Image(
    int32_t width, int32_t height, void* data, int32_t stride,
    const Allocator& allocator /*= Allocator( )*/ )
    : mStride( stride / static_cast< int32_t >( sizeof( PixelType ) ) )
    , mSize( width, height )
    , mData( static_cast< pointer_type >( data ) )
    , mAllocator( allocator )
{
}

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
Image< PixelType, Channels, Allocator >::Image( const Image& other )
    : mStride( other.mStride )
    , mSize( other.mSize )
    , mData( other.mData )
    , mAllocator( other.mAllocator )
    , mMemoryHandle( other.mMemoryHandle )
{
}

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
Image< PixelType, Channels, Allocator >::Image( Image&& other ) noexcept
    : mStride( other.mStride )
    , mSize( std::move( other.mSize ) )
    , mData( std::move( other.mData ) )
    , mAllocator( other.mAllocator )
    , mMemoryHandle( std::move( other.mMemoryHandle ) )
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
    , mMemoryHandle( other.mMemoryHandle )
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
        this->mMemoryHandle = std::move( other.mMemoryHandle );

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
        }

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
constexpr Image< PixelType, Channels, Allocator >
Image< PixelType, Channels, Allocator >::operator( )(
    const Rectangle< int32_t >& roi ) const
{
    return Image( *this, roi );
}

//
// Methods
//

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
    Image< PixelType, Channels, Allocator > image(
        this->getSize( ), false, mAllocator );

    this->copyTo( image );

    return image;
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
    std::swap( this->mMemoryHandle, other.mMemoryHandle );

    Allocator tmp = other.mAllocator;
    other.mAllocator = this->mAllocator;
    this->mAllocator = tmp;
}

template < Arithmetic PixelType, int32_t Channels, typename Allocator >
constexpr void Image< PixelType, Channels, Allocator >::allocate( )
{
    // Note: The allocator is pixel type dependent. We need to pass a stride in
    // PixelType, not in bytes.
    const auto elements = numberElements( );
    mData = allocator_traits::allocate( mAllocator, elements );

    // Get a copy of the allocator.
    // The object is destroyed, if the calls gets destructed and so the lambda
    // will not work
    auto allocator = mAllocator;
    auto data = mData;

    // Use mutable for lambda to not have the captured values const.
    // Tha allocator cannot be const for the call
    // https://en.cppreference.com/w/cpp/language/lambda
    mMemoryHandle = std::make_shared< Handle >(
        [ elements, allocator, data ]( ) mutable
        { allocator_traits::deallocate( allocator, data, elements ); } );
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

} // namespace cvl::core