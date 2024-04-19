// CVL includes
#include <cvl/core/Image.h>
#include <cvl/core/macros.h>

// STD includes
#include <random>

// GTest includes
IGNORE_WARNINGS_GTEST_PUSH
#include <gtest/gtest.h>
// #include <gmock/gmock.h> // GTest Update required because of removed function
//  in C++20
IGNORE_WARNINGS_POP

//
// Typed tests
// https://google.github.io/googletest/advanced.html#typed-tests

using namespace cvl::core;
using testing::Eq;

template < typename T >
class TestCvlCoreImage : public testing::Test
{
public:
    using List = std::list< T >;
    static T shared_;
    T value_ { };

    TestCvlCoreImage( ) = default;

    ~TestCvlCoreImage( ) override = default;

    TestCvlCoreImage( const TestCvlCoreImage& other ) = delete;

    TestCvlCoreImage( const TestCvlCoreImage&& other ) = delete;

    TestCvlCoreImage& operator=( const TestCvlCoreImage& other ) = delete;

    TestCvlCoreImage& operator=( TestCvlCoreImage&& other ) = delete;
};

using Types =
    testing::Types< int8_t, uint8_t, int16_t, uint16_t, float, double >;

TYPED_TEST_SUITE(
    TestCvlCoreImage,
    Types ); // NOLINT(clang-diagnostic-gnu-zero-variadic-macro-arguments)

TYPED_TEST( TestCvlCoreImage, DefaultConstructTyped )
{
    Image< TypeParam, 1 > image;

    ASSERT_EQ( image.getWidth( ), 0 );

    ASSERT_EQ( image.getStride( ), 0 );

    ASSERT_EQ( image.getHeight( ), 0 );

    ASSERT_EQ( image.getData( ), nullptr );
}

TYPED_TEST( TestCvlCoreImage, ValueConstructWidthHeightTyped )
{
    Image< TypeParam, 1 > image( 512, 256 );

    ASSERT_EQ( image.getWidth( ), 512 );

    ASSERT_EQ( image.getStride( ), 512 );

    ASSERT_EQ( image.getStrideInBytes( ), 512 * sizeof( TypeParam ) );

    ASSERT_EQ( image.getHeight( ), 256 );

    ASSERT_NE( image.getData( ), nullptr );
}

TYPED_TEST( TestCvlCoreImage, ValueConstructWidthHeightSmallTyped )
{
    Image< TypeParam, 1 > image( 8, 8, true );

    const auto lineSize = 8 * sizeof( TypeParam );
    int32_t stride;

    if constexpr ( lineSize % width_alignment == 0 )
    {
        stride = lineSize / sizeof( TypeParam );
    }
    else
    {
        stride =
            static_cast< int32_t >(
                lineSize / static_cast< double >( width_alignment ) + 1.0 ) *
            width_alignment / sizeof( TypeParam );
    }

    ASSERT_EQ( image.getWidth( ), 8 );

    ASSERT_EQ( image.getStride( ), stride );

    ASSERT_EQ( image.getStrideInBytes( ), stride * sizeof( TypeParam ) );

    ASSERT_EQ( image.getHeight( ), 8 );

    ASSERT_NE( image.getData( ), nullptr );

    for ( int32_t y = 0; y < image.getHeight( ); y++ )
    {
        for ( int32_t x = 0; x < image.getWidth( ); x++ )
        {
            ASSERT_EQ( image.at( y, x ), 0 );
        }
    }

    for ( int32_t y = 0; y < image.getHeight( ); y++ )
    {
        for ( int32_t x = 0; x < image.getWidth( ); x++ )
        {
            image.at( y, x ) = TypeParam { 2 };
        }
    }
}

TYPED_TEST( TestCvlCoreImage, ValueConstructWidthHeightNotAlignedTyped )
{
    Image< TypeParam, 1 > image( 500, 200 );

    ASSERT_EQ( image.getWidth( ), 500 );

    const auto lineSize = 500 * sizeof( TypeParam );
    int32_t stride;

    if constexpr ( lineSize % width_alignment == 0 )
    {
        stride = lineSize / sizeof( TypeParam );
    }
    else
    {
        stride =
            static_cast< int32_t >(
                lineSize / static_cast< double >( width_alignment ) + 1.0 ) *
            width_alignment / sizeof( TypeParam );
    }

    ASSERT_EQ( image.getStride( ), stride );

    ASSERT_EQ( image.getStrideInBytes( ), stride * sizeof( TypeParam ) );

    ASSERT_EQ( image.getHeight( ), 200 );

    ASSERT_NE( image.getData( ), nullptr );
}

TYPED_TEST( TestCvlCoreImage, ValueConstructWidthHeightStrideDataTyped )
{
    constexpr int32_t width = 256;
    constexpr int32_t stride = width * sizeof( TypeParam );
    constexpr int32_t height = 512;

    constexpr size_t bufferSize =
        static_cast< size_t >( stride ) * static_cast< size_t >( height );

    auto data = std::make_shared< TypeParam[] >( bufferSize );

    std::memset( data.get( ), 0x00, bufferSize * sizeof( TypeParam ) );

    for ( int32_t y = 0; y < height; y++ )
    {
        *( data.get( ) + y * width ) = TypeParam { 10 };
    }

    Image< TypeParam, 1 > image(
        width, height, static_cast< void* >( data.get( ) ), stride );

    ASSERT_EQ( image.getWidth( ), width );

    ASSERT_EQ( image.getStride( ), width );

    ASSERT_EQ( image.getStrideInBytes( ), stride );

    ASSERT_EQ( image.getHeight( ), height );

    ASSERT_NE( image.getData( ), nullptr );

    ASSERT_EQ( std::memcmp( image.getData( ), data.get( ), bufferSize ), 0 );
}

TYPED_TEST( TestCvlCoreImage, CopyConstructTyped )
{
    constexpr int32_t width = 256;
    constexpr int32_t stride = width * sizeof( TypeParam );
    constexpr int32_t height = 512;

    constexpr size_t bufferSize =
        static_cast< size_t >( stride ) * static_cast< size_t >( height );

    auto data = std::make_shared< TypeParam[] >( bufferSize );

    std::memset( data.get( ), 0x00, bufferSize * sizeof( TypeParam ) );

    for ( int32_t y = 0; y < height; y++ )
    {
        *( data.get( ) + y * width ) = TypeParam { 10 };
    }

    Image< TypeParam, 1 > image(
        width, height, static_cast< void* >( data.get( ) ), stride );

    const Image< TypeParam, 1 > imageCopy( image );

    ASSERT_EQ( image.getWidth( ), width );

    ASSERT_EQ( image.getStride( ), width );

    ASSERT_EQ( image.getStrideInBytes( ), stride );

    ASSERT_EQ( image.getHeight( ), height );

    ASSERT_EQ( image.getWidth( ), imageCopy.getWidth( ) );

    ASSERT_EQ( image.getStride( ), imageCopy.getStride( ) );

    ASSERT_EQ( image.getHeight( ), imageCopy.getHeight( ) );

    ASSERT_EQ(
        std::memcmp( image.getData( ), imageCopy.getData( ), bufferSize ), 0 );
}

TYPED_TEST( TestCvlCoreImage, MoveConstructTyped )
{
    constexpr int32_t width = 256;
    constexpr int32_t stride = width * sizeof( TypeParam );
    constexpr int32_t height = 512;

    constexpr size_t bufferSize =
        static_cast< size_t >( stride ) * static_cast< size_t >( height );

    auto data = std::make_shared< TypeParam[] >( bufferSize );

    std::memset( data.get( ), 0x00, bufferSize * sizeof( TypeParam ) );

    for ( int32_t y = 0; y < height; y++ )
    {
        *( data.get( ) + y * width ) = TypeParam { 10 };
    }

    Image< TypeParam, 1 > image(
        width, height, static_cast< void* >( data.get( ) ), stride );

    Image< TypeParam, 1 > imageToMove( image );

    Image< TypeParam, 1 > imageMoved( std::move( imageToMove ) );

    ASSERT_EQ( image.getWidth( ), width );

    ASSERT_EQ( image.getStride( ), width );

    ASSERT_EQ( image.getStrideInBytes( ), stride );

    ASSERT_EQ( image.getHeight( ), height );

    ASSERT_EQ( image.getWidth( ), imageMoved.getWidth( ) );

    ASSERT_EQ( image.getStride( ), imageMoved.getStride( ) );

    ASSERT_EQ( image.getHeight( ), imageMoved.getHeight( ) );

    ASSERT_EQ(
        std::memcmp( image.getData( ), imageMoved.getData( ), bufferSize ), 0 );
}

TYPED_TEST( TestCvlCoreImage, AssignmentOperatorTyped )
{
    constexpr int32_t width = 256;
    constexpr int32_t stride = width * sizeof( TypeParam );
    constexpr int32_t height = 512;

    constexpr size_t bufferSize =
        static_cast< size_t >( stride ) * static_cast< size_t >( height );

    auto data = std::make_shared< TypeParam[] >( bufferSize );

    std::memset( data.get( ), 0x00, bufferSize * sizeof( TypeParam ) );

    for ( int32_t y = 0; y < height; y++ )
    {
        *( data.get( ) + y * width ) = TypeParam { 10 };
    }

    Image< TypeParam, 1 > image(
        width, height, static_cast< void* >( data.get( ) ), stride );

    Image< TypeParam, 1 > imageAssigned { };

    imageAssigned = image;

    ASSERT_EQ( image.getWidth( ), width );

    ASSERT_EQ( image.getStride( ), width );

    ASSERT_EQ( image.getStrideInBytes( ), stride );

    ASSERT_EQ( image.getHeight( ), height );

    ASSERT_EQ( image.getWidth( ), imageAssigned.getWidth( ) );

    ASSERT_EQ( image.getStride( ), imageAssigned.getStride( ) );

    ASSERT_EQ( image.getHeight( ), imageAssigned.getHeight( ) );

    ASSERT_EQ(
        std::memcmp( image.getData( ), imageAssigned.getData( ), bufferSize ),
        0 );
}

TYPED_TEST( TestCvlCoreImage, MoveOperatorTyped )
{
    constexpr int32_t width = 256;
    constexpr int32_t stride = width * sizeof( TypeParam );
    constexpr int32_t height = 512;

    constexpr size_t bufferSize =
        static_cast< size_t >( stride ) * static_cast< size_t >( height );

    auto data = std::make_shared< TypeParam[] >( bufferSize );

    std::memset( data.get( ), 0x00, bufferSize * sizeof( TypeParam ) );

    for ( int32_t y = 0; y < height; y++ )
    {
        *( data.get( ) + y * width ) = TypeParam { 10 };
    }

    Image< TypeParam, 1 > image(
        width, height, static_cast< void* >( data.get( ) ), stride );

    Image< TypeParam, 1 > imageToMove( image );

    Image< TypeParam, 1 > imageMoved;

    imageMoved = std::move( imageToMove );

    ASSERT_EQ( image.getWidth( ), width );

    ASSERT_EQ( image.getStride( ), width );

    ASSERT_EQ( image.getStrideInBytes( ), stride );

    ASSERT_EQ( image.getHeight( ), height );

    ASSERT_EQ( image.getWidth( ), imageMoved.getWidth( ) );

    ASSERT_EQ( image.getStride( ), imageMoved.getStride( ) );

    ASSERT_EQ( image.getHeight( ), imageMoved.getHeight( ) );

    ASSERT_EQ(
        std::memcmp( image.getData( ), imageMoved.getData( ), bufferSize ), 0 );
}

TYPED_TEST( TestCvlCoreImage, EqualOperatorTrueTyped )
{
    constexpr int32_t width = 256;
    constexpr int32_t stride = width * sizeof( TypeParam );
    constexpr int32_t height = 512;

    constexpr size_t bufferSize =
        static_cast< size_t >( stride ) * static_cast< size_t >( height );

    auto data = std::make_shared< TypeParam[] >( bufferSize );

    std::memset( data.get( ), 0x00, bufferSize * sizeof( TypeParam ) );

    for ( int32_t y = 0; y < height; y++ )
    {
        *( data.get( ) + y * width ) = TypeParam { 10 };
    }

    Image< TypeParam, 1 > image(
        width, height, static_cast< void* >( data.get( ) ), stride );

    const Image< TypeParam, 1 > imageCopy( image );

    ASSERT_EQ( image, imageCopy );
}

TYPED_TEST( TestCvlCoreImage, EqualOperatorFalseStrideTyped )
{
    constexpr int32_t width = 256;
    constexpr int32_t stride = width * sizeof( TypeParam );
    constexpr int32_t height = 512;

    constexpr size_t bufferSize =
        static_cast< size_t >( stride ) * static_cast< size_t >( height );

    auto data = std::make_shared< TypeParam[] >( bufferSize );

    std::memset( data.get( ), 0x00, bufferSize * sizeof( TypeParam ) );

    for ( int32_t y = 0; y < height; y++ )
    {
        *( data.get( ) + y * width ) = TypeParam { 10 };
    }

    Image< TypeParam, 1 > image(
        width, height, static_cast< void* >( data.get( ) ), stride );

    constexpr int32_t width2 = 300;

    const Image< TypeParam, 1 > image2( width2, height );

    ASSERT_NE( image, image2 );
}

TYPED_TEST( TestCvlCoreImage, EqualOperatorFalseWidthTyped )
{
    constexpr int32_t width = 256;
    constexpr int32_t stride = width * sizeof( TypeParam );
    constexpr int32_t height = 512;

    constexpr size_t bufferSize =
        static_cast< size_t >( stride ) * static_cast< size_t >( height );

    auto data = std::make_shared< TypeParam[] >( bufferSize );

    std::memset( data.get( ), 0x00, bufferSize * sizeof( TypeParam ) );

    for ( int32_t y = 0; y < height; y++ )
    {
        *( data.get( ) + y * width ) = TypeParam { 10 };
    }

    Image< TypeParam, 1 > image(
        width, height, static_cast< void* >( data.get( ) ), stride );

    constexpr int32_t width2 = 300;

    const Image< TypeParam, 1 > image2( width2, height );

    ASSERT_NE( image, image2 );
}

TYPED_TEST( TestCvlCoreImage, EqualOperatorFalseHeightTyped )
{
    constexpr int32_t width = 256;
    constexpr int32_t stride = width * sizeof( TypeParam );
    constexpr int32_t height = 512;

    constexpr size_t bufferSize =
        static_cast< size_t >( stride ) * static_cast< size_t >( height );

    auto data = std::make_shared< TypeParam[] >( bufferSize );

    std::memset( data.get( ), 0x00, bufferSize * sizeof( TypeParam ) );

    for ( int32_t y = 0; y < height; y++ )
    {
        *( data.get( ) + y * width ) = TypeParam { 10 };
    }

    Image< TypeParam, 1 > image(
        width, height, static_cast< void* >( data.get( ) ), stride );

    constexpr int32_t height2 = 300;

    const Image< TypeParam, 1 > image2( width, height2 );

    ASSERT_NE( image, image2 );
}

TYPED_TEST( TestCvlCoreImage, EqualOperatorFalseBufferTyped )
{
    constexpr int32_t width = 256;
    constexpr int32_t stride = width * sizeof( TypeParam );
    constexpr int32_t height = 512;

    constexpr size_t bufferSize =
        static_cast< size_t >( stride ) * static_cast< size_t >( height );

    auto data = std::make_shared< TypeParam[] >( bufferSize );

    std::memset( data.get( ), 0x00, bufferSize * sizeof( TypeParam ) );

    for ( int32_t y = 0; y < height; y++ )
    {
        *( data.get( ) + y * width ) = TypeParam { 10 };
    }

    Image< TypeParam, 1 > image(
        width, height, static_cast< void* >( data.get( ) ), stride );

    const Image< TypeParam, 1 > image2( width, height );

    ASSERT_NE( image, image2 );
}

TYPED_TEST( TestCvlCoreImage, AccessRowPointerTyped )
{
    constexpr int32_t width = 256;
    constexpr int32_t stride = width * sizeof( TypeParam );
    constexpr int32_t height = 512;

    constexpr size_t bufferSize =
        static_cast< size_t >( stride ) * static_cast< size_t >( height );

    auto data = std::make_shared< TypeParam[] >( bufferSize );

    std::memset( data.get( ), 0x00, bufferSize * sizeof( TypeParam ) );

    for ( int32_t y = 0; y < height; y++ )
    {
        *( data.get( ) + y * width ) = TypeParam { 10 };
    }

    Image< TypeParam, 1 > image(
        width, height, static_cast< void* >( data.get( ) ), stride );

    for ( int32_t y = 0; y < height; y++ )
    {
        ASSERT_EQ( *image.getRowPointer( y ), TypeParam { 10 } );
    }
}

TYPED_TEST( TestCvlCoreImage, AccessAtRefTyped )
{
    constexpr int32_t width = 256;
    constexpr int32_t stride = width * sizeof( TypeParam );
    constexpr int32_t height = 512;

    constexpr size_t bufferSize =
        static_cast< size_t >( stride ) * static_cast< size_t >( height );

    auto data = std::make_shared< TypeParam[] >( bufferSize );

    std::memset( data.get( ), 0x00, bufferSize * sizeof( TypeParam ) );

    for ( int32_t y = 0; y < height; y++ )
    {
        *( data.get( ) + y * width ) = TypeParam { 10 };
    }

    Image< TypeParam, 1 > image(
        width, height, static_cast< void* >( data.get( ) ), stride );

    for ( int32_t y = 0; y < height; y++ )
    {
        ASSERT_EQ( image.at( y, 0 ), TypeParam { 10 } );
    }
}

TYPED_TEST( TestCvlCoreImage, AccessAtConstRefTyped )
{
    constexpr int32_t width = 256;
    constexpr int32_t stride = width * sizeof( TypeParam );
    constexpr int32_t height = 512;

    constexpr size_t bufferSize =
        static_cast< size_t >( stride ) * static_cast< size_t >( height );

    auto data = std::make_shared< TypeParam[] >( bufferSize );

    std::memset( data.get( ), 0x00, bufferSize * sizeof( TypeParam ) );

    for ( int32_t y = 0; y < height; y++ )
    {
        *( data.get( ) + y * width ) = TypeParam { 10 };
    }

    const Image< TypeParam, 1 > image(
        width, height, static_cast< void* >( data.get( ) ), stride );

    for ( int32_t y = 0; y < height; y++ )
    {
        ASSERT_EQ( image.at( y, 0 ), TypeParam { 10 } );
    }
}

TYPED_TEST( TestCvlCoreImage, AccessAt3ChannelTyped )
{
    constexpr int32_t width = 16;
    constexpr int32_t height = 16;

    Image< TypeParam, 3 > image( width, height, true );

    constexpr auto typeStride = width;

    {
        auto data = image.getData( );
        data[ 0 ] = TypeParam { 100 };

        data += typeStride * height;
        data[ 0 ] = TypeParam { 100 };

        data += typeStride * height;
        data[ 0 ] = TypeParam { 100 };
    }

    {
        auto data = image.getData( );
        ++data;

        data[ 0 ] = TypeParam { 101 };

        data += typeStride * height;
        data[ 0 ] = TypeParam { 101 };

        data += typeStride * height;
        data[ 0 ] = TypeParam { 101 };
    }

    {
        auto data = image.getData( );
        data += 2;

        data[ 0 ] = TypeParam { 102 };

        data += typeStride * height;
        data[ 0 ] = TypeParam { 102 };

        data += typeStride * height;
        data[ 0 ] = TypeParam { 102 };
    }

    EXPECT_EQ( image.at( 0, 0, 0 ), TypeParam { 100 } );
    EXPECT_EQ( image.at( 0, 1, 0 ), TypeParam { 101 } );
    EXPECT_EQ( image.at( 0, 2, 0 ), TypeParam { 102 } );
    EXPECT_EQ( image.at( 0, 3, 0 ), TypeParam { 0 } );

    EXPECT_EQ( image.at( 0, 0, 1 ), TypeParam { 100 } );
    EXPECT_EQ( image.at( 0, 1, 1 ), TypeParam { 101 } );
    EXPECT_EQ( image.at( 0, 2, 1 ), TypeParam { 102 } );
    EXPECT_EQ( image.at( 0, 3, 1 ), TypeParam { 0 } );

    EXPECT_EQ( image.at( 0, 0, 2 ), TypeParam { 100 } );
    EXPECT_EQ( image.at( 0, 1, 2 ), TypeParam { 101 } );
    EXPECT_EQ( image.at( 0, 2, 2 ), TypeParam { 102 } );
    EXPECT_EQ( image.at( 0, 3, 2 ), TypeParam { 0 } );
}

TYPED_TEST( TestCvlCoreImage, RoiOperator )
{
    auto imageFull = Image< TypeParam, 1 >( 12, 12, true );

    const auto roi = Rectangle< int32_t >( Point2i { 2, 2 }, { 4, 4 } );

    auto imageRoi = imageFull( roi );

    // We expect changes on the ROI also happening on the original image

    imageFull.at( 2, 2 ) = TypeParam { 100 };

    EXPECT_EQ( imageFull.at( 2, 2 ), imageRoi.at( 0, 0 ) );
}

TEST( TestCvlCoreImage, CopyImageRoi )
{
    Image8UC1 imageOrg( 256, 256, true );

    for ( int32_t y = 64; y < 192; y++ )
    {
        for ( int32_t x = 64; x < 192; x++ )
        {
            imageOrg.at( y, x ) = uint8_t { 128 };
        }
    }

    const Rectangle< int32_t > roi( Point2i { 64, 64 }, { 128, 128 } );

    const Image8UC1 imageRoi = imageOrg( roi );

    EXPECT_EQ( imageRoi.getWidth( ), 128 );
    EXPECT_EQ( imageRoi.getHeight( ), 128 );
    EXPECT_EQ( imageRoi.getStride( ), 256 );

    for ( int32_t y = 0; y < imageRoi.getHeight( ); y++ )
    {
        const auto roiRowPtr = imageRoi.getRowPointer( y );
        for ( int32_t x = 0; x < imageRoi.getWidth( ); x++ )
        {
            EXPECT_EQ( imageRoi.at( y, x ), 128 );
            EXPECT_EQ( roiRowPtr[ x ], 128 );
        }
    }

    const Image8UC1 imageRoiCopy( imageRoi );

    EXPECT_EQ( imageRoiCopy.getWidth( ), 128 );
    EXPECT_EQ( imageRoiCopy.getHeight( ), 128 );
    EXPECT_EQ( imageRoiCopy.getStride( ), 128 );

    for ( int32_t y = 0; y < imageRoiCopy.getHeight( ); y++ )
    {
        const auto roiRowPtr = imageRoiCopy.getRowPointer( y );
        for ( int32_t x = 0; x < imageRoiCopy.getWidth( ); x++ )
        {
            EXPECT_EQ( imageRoiCopy.at( y, x ), 128 );
            EXPECT_EQ( roiRowPtr[ x ], 128 );
        }
    }
}

TYPED_TEST( TestCvlCoreImage, CopyTo )
{
}

TYPED_TEST( TestCvlCoreImage, Clone )
{
    Image< TypeParam, 1 > image( 256, 125 );

    image.at( 0, 0 ) = TypeParam { 5 };

    const auto imageClone = image.clone( );

    EXPECT_EQ( *imageClone.getData( ), 5 );
}