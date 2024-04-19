#pragma once

namespace cvl::core
{

/**
 * @brief A CRTP helper class
 *
 * This class is a useful helper with CRTP ans static polymorphism to
 * avoid static_casts in each derived class
 *
 * this->underlying() instead of
 * auto derived = static_cast< Derived& >( *this );
 *
 * // https://www.fluentcpp.com/2017/05/19/crtp-helper/
 *
 */
template < typename Derived, template < typename > class CrtpType >
struct CrtpBase
{
    /*using allocator_traits_ =
        std::allocator_traits< typename Derived::allocator_type >;*/

    Derived& underlying( ) { return static_cast< Derived& >( *this ); }

    Derived const& underlying( ) const
    {
        return static_cast< Derived const& >( *this );
    }

private:
    CrtpBase( ) = default;
    friend CrtpType< Derived >;
};

} // namespace cvl::core