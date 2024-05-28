#include <cvl/core/macros.h>

// GTest includes
IGNORE_WARNINGS_GTEST_PUSH
#include <gtest/gtest.h>
// #include <gmock/gmock.h> // GTest Update required because of removed function
//  in C++20
IGNORE_WARNINGS_POP

// STD includes
#include <random>
#include <list>

// CVL includes
#include <cvl/core/Size.h>

//
// Typed tests
// https://google.github.io/googletest/advanced.html#typed-tests

using namespace cvl::core;
using testing::Eq;

template < typename T >
class TestCvlCoreSize : public testing::Test
{
public:
    using List = std::list< T >;
    static T shared_;
    T value_ { };

    TestCvlCoreSize( ) = default;

    ~TestCvlCoreSize( ) override = default;

    TestCvlCoreSize( const TestCvlCoreSize& other ) = delete;

    TestCvlCoreSize( const TestCvlCoreSize&& other ) = delete;

    TestCvlCoreSize& operator=( const TestCvlCoreSize& other ) = delete;

    TestCvlCoreSize& operator=( TestCvlCoreSize&& other ) = delete;

    constexpr T getRandomValue( ) const
    {
        // Will be used to obtain a seed for the random number engine
        std::random_device rd;
        // Standard mersenne_twister_engine seeded with rd()
        std::mt19937 gen( rd( ) );

        if constexpr ( std::is_floating_point< T >( ) )
        {
            std::uniform_real_distribution<> distribution( 1., 1000. );
            return static_cast< T >( distribution( gen ) );
        }
        else
        {
            std::uniform_int_distribution<> distribution( 1, 1000 );
            return static_cast< T >( distribution( gen ) );
        }
    }
};

using Types =
    testing::Types< int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t,
                    int64_t, uint64_t, float, double, size_t >;

TYPED_TEST_SUITE(
    TestCvlCoreSize,
    Types ); // NOLINT(clang-diagnostic-gnu-zero-variadic-macro-arguments)

TYPED_TEST( TestCvlCoreSize, DefaultConstructTyped )
{
    Size< TypeParam > size;

    ASSERT_EQ( size.getHeight( ), 0 );
    ASSERT_EQ( size.getWidth( ), 0 );
}

TYPED_TEST( TestCvlCoreSize, ValueConstructTyped )
{
    const auto w = this->getRandomValue( );
    const auto h = this->getRandomValue( );

    Size< TypeParam > size( w, h );

    ASSERT_EQ( size.getWidth( ), w );
    ASSERT_EQ( size.getHeight( ), h );
}

TYPED_TEST( TestCvlCoreSize, CopyConstructTyped )
{
    const auto w = this->getRandomValue( );
    const auto h = this->getRandomValue( );

    Size< TypeParam > size1( w, h );
    const Size< TypeParam > size2( size1 );

    ASSERT_EQ( size1.getWidth( ), w );
    ASSERT_EQ( size1.getHeight( ), h );

    ASSERT_EQ( size1.getWidth( ), size2.getWidth( ) );
    ASSERT_EQ( size1.getHeight( ), size2.getHeight( ) );
}

TYPED_TEST( TestCvlCoreSize, MoveConstructTyped )
{
    const auto w = this->getRandomValue( );
    const auto h = this->getRandomValue( );

    Size< TypeParam > size1( w, h );
    Size< TypeParam > size2( size1 );

    Size< TypeParam > sizeMoved( std::move( size2 ) );

    ASSERT_EQ( size1.getWidth( ), w );
    ASSERT_EQ( size1.getHeight( ), h );

    ASSERT_EQ( size1.getWidth( ), sizeMoved.getWidth( ) );
    ASSERT_EQ( size1.getHeight( ), sizeMoved.getHeight( ) );
}

TYPED_TEST( TestCvlCoreSize, AssignmentOperatorTyped )
{
    const auto w = this->getRandomValue( );
    const auto h = this->getRandomValue( );

    const Size< TypeParam > size1( w, h );
    Size< TypeParam > size2;

    size2 = size1;

    ASSERT_EQ( size1.getWidth( ), w );
    ASSERT_EQ( size1.getHeight( ), h );

    ASSERT_EQ( size1.getWidth( ), size2.getWidth( ) );
    ASSERT_EQ( size1.getHeight( ), size2.getHeight( ) );
}

TYPED_TEST( TestCvlCoreSize, MoveOperatorTyped )
{
    const auto w = this->getRandomValue( );
    const auto h = this->getRandomValue( );

    const Size< TypeParam > size1( w, h );
    Size< TypeParam > sizeToMove( size1 );
    Size< TypeParam > size2;

    size2 = std::move( sizeToMove );

    ASSERT_EQ( size1.getWidth( ), w );
    ASSERT_EQ( size1.getHeight( ), h );

    ASSERT_EQ( size1.getWidth( ), size2.getWidth( ) );
    ASSERT_EQ( size1.getHeight( ), size2.getHeight( ) );
}

TYPED_TEST( TestCvlCoreSize, EqualOperatorTyped )
{
    const auto w = this->getRandomValue( );
    const auto h = this->getRandomValue( );

    Size< TypeParam > size1( w, h );
    Size< TypeParam > size2( w, h );

    ASSERT_EQ( size1, size2 );
}

TYPED_TEST( TestCvlCoreSize, NotEqualOperatorTyped )
{
    const auto w1 = this->getRandomValue( );
    const auto h1 = this->getRandomValue( );

    const auto w2 = this->getRandomValue( );
    const auto h2 = this->getRandomValue( );

    Size< TypeParam > size1( w1, h1 );
    Size< TypeParam > size2( w2, h2 );

    ASSERT_NE( size1, size2 );
}

TYPED_TEST( TestCvlCoreSize, AddOperatorNewObjectTyped )
{
    if constexpr ( ! std::is_same< TypeParam, uint8_t >( ) &&
                   ! std::is_same< TypeParam, int8_t >( ) )
    {
        const auto w1 = this->getRandomValue( );
        const auto h1 = this->getRandomValue( );

        const auto w2 = this->getRandomValue( );
        const auto h2 = this->getRandomValue( );

        Size< TypeParam > size1( w1, h1 );
        Size< TypeParam > size2( w2, h2 );

        const Size< TypeParam > sizeAdd = size1 + size2;

        ASSERT_EQ( sizeAdd.getWidth( ), static_cast< TypeParam >( w1 + w2 ) );
        ASSERT_EQ( sizeAdd.getHeight( ), static_cast< TypeParam >( h1 + h2 ) );
    }
    else
    {
        // uint8_t and int8_t will be interpreted as characters.
        ASSERT_EQ( true, true );
    }
}

TEST( TestCvlCoreSize, AddOperatorNewObject )
{
    constexpr int32_t w1 = 300;
    constexpr int32_t h1 = 600;

    constexpr double w2 = 50.75;
    constexpr double h2 = 100.25;

    const Size< int32_t > size1( w1, h1 );
    const Size< double > size2( w2, h2 );

    const auto sizeAdd = size1 + size2;

    ASSERT_NEAR( sizeAdd.getWidth( ), static_cast< double >( w1 ) + w2, 0.01 );
    ASSERT_NEAR( sizeAdd.getHeight( ), static_cast< double >( h1 ) + h2, 0.01 );
}

TYPED_TEST( TestCvlCoreSize, AddAssigneOperatorTyped )
{
    if constexpr ( ! std::is_same< TypeParam, uint8_t >( ) &&
                   ! std::is_same< TypeParam, int8_t >( ) )
    {
        const auto w1 = this->getRandomValue( );
        const auto h1 = this->getRandomValue( );

        const auto w2 = this->getRandomValue( );
        const auto h2 = this->getRandomValue( );

        Size< TypeParam > size1( w1, h1 );
        Size< TypeParam > size2( w2, h2 );

        size1 += size2;

        ASSERT_EQ( size1.getWidth( ), static_cast< TypeParam >( w1 + w2 ) );
        ASSERT_EQ( size1.getHeight( ), static_cast< TypeParam >( h1 + h2 ) );
    }
    else
    {
        // uint8_t and int8_t will be interpreted as characters.
        ASSERT_EQ( true, true );
    }
}

TYPED_TEST( TestCvlCoreSize, SubtractOperatorNewObjectTyped )
{
    if constexpr ( ! std::is_same< TypeParam, uint8_t >( ) &&
                   ! std::is_same< TypeParam, int8_t >( ) )
    {
        const auto w1 = this->getRandomValue( );
        const auto h1 = this->getRandomValue( );

        const auto w2 = this->getRandomValue( );
        const auto h2 = this->getRandomValue( );

        Size< TypeParam > size1( w1, h1 );
        Size< TypeParam > size2( w2, h2 );

        const Size< TypeParam > sizeSub = size1 - size2;

        ASSERT_EQ( sizeSub.getWidth( ), static_cast< TypeParam >( w1 - w2 ) );
        ASSERT_EQ( sizeSub.getHeight( ), static_cast< TypeParam >( h1 - h2 ) );
    }
    else
    {
        // uint8_t and int8_t will be interpreted as characters.
        ASSERT_EQ( true, true );
    }
}

TEST( TestCvlCoreSize, SubtractOperatorNewObject )
{
    constexpr int32_t w1 = 300;
    constexpr int32_t h1 = 600;

    constexpr double w2 = 50.75;
    constexpr double h2 = 100.25;

    const Size< int32_t > size1( w1, h1 );
    const Size< double > size2( w2, h2 );

    const auto sizeAdd = size1 - size2;

    ASSERT_NEAR( sizeAdd.getWidth( ), static_cast< double >( w1 ) - w2, 0.01 );
    ASSERT_NEAR( sizeAdd.getHeight( ), static_cast< double >( h1 ) - h2, 0.01 );
}

TYPED_TEST( TestCvlCoreSize, SubtractAssigneOperatorTyped )
{
    if constexpr ( ! std::is_same< TypeParam, uint8_t >( ) &&
                   ! std::is_same< TypeParam, int8_t >( ) )
    {
        const auto w1 = this->getRandomValue( );
        const auto h1 = this->getRandomValue( );

        const auto w2 = this->getRandomValue( );
        const auto h2 = this->getRandomValue( );

        Size< TypeParam > size1( w1, h1 );
        Size< TypeParam > size2( w2, h2 );

        size1 -= size2;

        ASSERT_EQ( size1.getWidth( ), static_cast< TypeParam >( w1 - w2 ) );
        ASSERT_EQ( size1.getHeight( ), static_cast< TypeParam >( h1 - h2 ) );
    }
    else
    {
        // uint8_t and int8_t will be interpreted as characters.
        ASSERT_EQ( true, true );
    }
}

TYPED_TEST( TestCvlCoreSize, ScaleOperatorNewObjectTyped )
{
    if constexpr ( ! std::is_same< TypeParam, uint8_t >( ) &&
                   ! std::is_same< TypeParam, int8_t >( ) )
    {
        const auto w = this->getRandomValue( );
        const auto h = this->getRandomValue( );

        const auto s = this->getRandomValue( );

        Size< TypeParam > size1( w, h );

        const Size< TypeParam > sizeScaled = size1 * s;

        ASSERT_EQ( sizeScaled.getWidth( ), static_cast< TypeParam >( w * s ) );
        ASSERT_EQ( sizeScaled.getHeight( ), static_cast< TypeParam >( h * s ) );
    }
    else
    {
        // uint8_t and int8_t will be interpreted as characters.
        ASSERT_EQ( true, true );
    }
}

TEST( TestCvlCoreSize, ScaleOperatorNewObject )
{
    constexpr int32_t w1 = 300;
    constexpr int32_t h1 = 600;

    constexpr double s = 0.25;

    const Size< int32_t > size( w1, h1 );

    const auto sizeAdd = size * s;

    ASSERT_NEAR( sizeAdd.getWidth( ), static_cast< double >( w1 ) * s, 0.01 );
    ASSERT_NEAR( sizeAdd.getHeight( ), static_cast< double >( h1 ) * s, 0.01 );
}

TYPED_TEST( TestCvlCoreSize, OutputOperatorTyped )
{
    if constexpr ( ! std::is_same< TypeParam, uint8_t >( ) &&
                   ! std::is_same< TypeParam, int8_t >( ) )
    {
        const auto w = this->getRandomValue( );
        const auto h = this->getRandomValue( );

        std::stringstream ss;
        ss << Size< TypeParam >( { w, h } );

        std::stringstream cmp;
        cmp << "(w: " << w << ", h: " << h << ")";
        ASSERT_EQ( ss.str( ), cmp.str( ) );
    }

    else
    {
        // uint8_t and int8_t will be interpreted as characters.
        ASSERT_EQ( true, true );
    }
}
