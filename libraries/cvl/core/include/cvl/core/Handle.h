#pragma once

// CVL includes
#include <cvl/core/macros.h>

// STD includes
#include <functional>

namespace cvl::core
{
/**
 * @brief Template implementation for a handle class
 *
 * https://stackoverflow.com/questions/39516416/using-weak-ptr-to-implement-the-observer-pattern
 */
class CVL_CORE_EXPORT Handle
{
public:
    /*
     *   Default constructor
     */
    Handle( ) = default;

    /*
     *   Value constructor to create a handle
     *   @param [in] onDestroy   The cleanup function
     */
    Handle( std::function< void( ) > onDestroy );

    /*
     *   Delete copy constructor
     */
    Handle( const Handle& ) = delete;

    /*
     *   Move constructor
     *   @param [in] other   The handle to take the function from
     */
    Handle( Handle&& other ) noexcept;

    /*
     *   Destructor
     *   Clear the handle by calling the cleanup function
     */
    ~Handle( );

    /*
     *   Delete copy operator
     */
    Handle& operator=( const Handle& rhs ) = delete;

    /*
     *   Move operator
     *   @param [in] rhs   The handle to take the function from
     *   return            Reference to this
     */
    Handle& operator=( Handle&& rhs ) noexcept;

    void clear( );

private:
    std::function< void( ) > mOnDestroy;
};

DECLARE_SMARTPTR( Handle );

} // namespace cvl::core