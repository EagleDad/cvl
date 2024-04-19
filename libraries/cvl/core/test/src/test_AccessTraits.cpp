#include <cvl/core/macros.h>

// GTest includes
IGNORE_WARNINGS_GTEST_PUSH
#include <gtest/gtest.h>
// #include <gmock/gmock.h> // GTest Update required because of removed function
//  in C++20
IGNORE_WARNINGS_POP

// STD includes
#include <random>

// CVL includes
#include <cvl/core/AccessTraits.h>

//
// Typed tests
// https://google.github.io/googletest/advanced.html#typed-tests

using namespace cvl::core;
using testing::Eq;

template < typename T >
class TestCvlCoreAccessTraits : public testing::Test
{
public:
    using List = std::list< T >;
    static T shared_;
    T value_ { };

    TestCvlCoreAccessTraits( ) = default;

    ~TestCvlCoreAccessTraits( ) override = default;

    TestCvlCoreAccessTraits( const TestCvlCoreAccessTraits& other ) = delete;

    TestCvlCoreAccessTraits( const TestCvlCoreAccessTraits&& other ) = delete;

    TestCvlCoreAccessTraits&
    operator=( const TestCvlCoreAccessTraits& other ) = delete;

    TestCvlCoreAccessTraits&
    operator=( TestCvlCoreAccessTraits&& other ) = delete;

    constexpr std::vector< T > getRandomValues( const size_t dimension )
    {
        // Will be used to obtain a seed for the random number engine
        std::random_device rd;
        // Standard mersenne_twister_engine seeded with rd()
        std::mt19937 gen( rd( ) );

        std::vector< T > data;

        if constexpr ( std::is_floating_point< T >( ) )
        {
            std::uniform_real_distribution<> distribution( 1., 1000. );

            data.reserve( dimension );
            for ( size_t i = 0; i < dimension; i++ )
            {
                data.push_back( static_cast< T >( distribution( gen ) ) );
            }
        }
        else
        {
            std::uniform_int_distribution<> distribution( 1, 1000 );

            data.reserve( dimension );
            for ( size_t i = 0; i < dimension; i++ )
            {
                data.push_back( static_cast< T >( distribution( gen ) ) );
            }
        }

        return data;
    }
};

using Types =
    testing::Types< int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t,
                    int64_t, uint64_t, float, double, size_t >;

TYPED_TEST_SUITE(
    TestCvlCoreAccessTraits,
    Types ); // NOLINT(clang-diagnostic-gnu-zero-variadic-macro-arguments)

TYPED_TEST( TestCvlCoreAccessTraits, DefaultConstructPoint1DTyped )
{
    Point< TypeParam, 1 > point;

    ASSERT_EQ( get< 0 >( point ), 0 );
}

TYPED_TEST( TestCvlCoreAccessTraits, RandomPointNDTyped )
{
    constexpr int32_t dimension = 10;

    auto data = this->getRandomValues( dimension );

    Point< TypeParam, dimension > point( data[ 0 ],
                                         data[ 1 ],
                                         data[ 2 ],
                                         data[ 3 ],
                                         data[ 4 ],
                                         data[ 5 ],
                                         data[ 6 ],
                                         data[ 7 ],
                                         data[ 8 ],
                                         data[ 9 ] );

    ASSERT_EQ( get< 0 >( point ), data[ 0 ] );
    ASSERT_EQ( get< 1 >( point ), data[ 1 ] );
    ASSERT_EQ( get< 2 >( point ), data[ 2 ] );
    ASSERT_EQ( get< 3 >( point ), data[ 3 ] );
    ASSERT_EQ( get< 4 >( point ), data[ 4 ] );
    ASSERT_EQ( get< 5 >( point ), data[ 5 ] );
    ASSERT_EQ( get< 6 >( point ), data[ 6 ] );
    ASSERT_EQ( get< 7 >( point ), data[ 7 ] );
    ASSERT_EQ( get< 8 >( point ), data[ 8 ] );
    ASSERT_EQ( get< 9 >( point ), data[ 9 ] );
}

TEST( TestCvlCoreAccessTraits, DefaultConstructVector1D )
{
    constexpr Vector< 1 > vector;

    ASSERT_EQ( get< 0 >( vector ), 0.0 );
}

TYPED_TEST( TestCvlCoreAccessTraits, RandomVectorNDTyped )
{
    constexpr int32_t dimension = 10;

    auto data = this->getRandomValues( dimension );

    const Vector< dimension > vector( static_cast< double >( data[ 0 ] ),
                                      static_cast< double >( data[ 1 ] ),
                                      static_cast< double >( data[ 2 ] ),
                                      static_cast< double >( data[ 3 ] ),
                                      static_cast< double >( data[ 4 ] ),
                                      static_cast< double >( data[ 5 ] ),
                                      static_cast< double >( data[ 6 ] ),
                                      static_cast< double >( data[ 7 ] ),
                                      static_cast< double >( data[ 8 ] ),
                                      static_cast< double >( data[ 9 ] ) );

    ASSERT_EQ( get< 0 >( vector ), data[ 0 ] );
    ASSERT_EQ( get< 1 >( vector ), data[ 1 ] );
    ASSERT_EQ( get< 2 >( vector ), data[ 2 ] );
    ASSERT_EQ( get< 3 >( vector ), data[ 3 ] );
    ASSERT_EQ( get< 4 >( vector ), data[ 4 ] );
    ASSERT_EQ( get< 5 >( vector ), data[ 5 ] );
    ASSERT_EQ( get< 6 >( vector ), data[ 6 ] );
    ASSERT_EQ( get< 7 >( vector ), data[ 7 ] );
    ASSERT_EQ( get< 8 >( vector ), data[ 8 ] );
    ASSERT_EQ( get< 9 >( vector ), data[ 9 ] );
}
