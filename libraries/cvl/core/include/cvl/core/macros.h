#pragma once

// CVL includes
#include <cvl/core/Error.h>

// STD includes
#include <sstream>
#include <memory>

#if defined( WIN32 )

#define IGNORE_WARNINGS_GTEST_PUSH                                             \
    __pragma( warning( push ) ) __pragma( warning( disable : 4389 ) )          \
        __pragma( warning( disable : 4626 ) )                                  \
            __pragma( warning( disable : 5026 ) )                              \
                __pragma( warning( disable : 4625 ) )                          \
                    __pragma( warning( disable : 5027 ) )                      \
                        __pragma( warning( disable : 5219 ) )                  \
                            __pragma( warning( disable : 4388 ) )              \
                                __pragma( warning( disable : 4365 ) )          \
                                    __pragma( warning( disable : 5204 ) )      \
                                        __pragma( warning( disable : 4355 ) )

#define IGNORE_WARNINGS_STD_PUSH                                               \
    __pragma( warning( push ) ) __pragma( warning( disable : 4355 ) )          \
        __pragma( warning( disable : 5204 ) )                                  \
            __pragma( warning( disable : 5220 ) )                              \
                __pragma( warning( disable : 4625 )                            \
                              __pragma( warning( disable : 4626 ) ) )          \
                    __pragma( warning( disable : 5026 ) )                      \
                        __pragma( warning( disable : 5027 ) )

#define IGNORE_WARNINGS_POP __pragma( warning( pop ) )

#else

#define IGNORE_WARNINGS_GTEST_PUSH
#define IGNORE_WARNINGS_STD_PUSH 
#define IGNORE_WARNINGS_POP 

#endif

#define ANONYMOUS_VARIABLE( str ) VARIABLE_MERGE( str, __LINE__ )
#define VARIABLE_MERGE( a, b )    DO_MERGE( a, b )
#define DO_MERGE( a, b )          a##b

/**
 * @brief Mark class a interface
 *
 * Make class inheritable and remove all default initialization for constructors
 * and assignment operators.
 */
#define CVL_NO_DEFAULT_INIT( className )                                       \
public:                                                                        \
    className( ) = delete;                                                     \
                                                                               \
    virtual ~className( ) = default;                                           \
                                                                               \
    className( const className& other ) = delete;                              \
                                                                               \
    className( className&& other ) = delete;                                   \
                                                                               \
    className& operator=( const className& other ) = delete;                   \
                                                                               \
    className& operator=( className&& other ) = delete

/**
 * @brief Mark class a interface
 *
 * Make class inheritable and remove all initialization expect default
 * construction.
 */
#define CVL_DEFAULT_ONLY( className )                                          \
public:                                                                        \
    className( ) = default;                                                    \
                                                                               \
    ~className( ) = default;                                                   \
                                                                               \
    className( const className& other ) = delete;                              \
                                                                               \
    className( className&& other ) = delete;                                   \
                                                                               \
    className& operator=( const className& other ) = delete;                   \
                                                                               \
    className& operator=( className&& other ) = delete

/**
 * @brief Mark class a interface
 *
 * Make class inheritable and remove initialization using default
 * constructors and assignment operators.
 */
#define CVL_INTERFACE( className )                                             \
protected:                                                                     \
    className( ) = default;                                                    \
                                                                               \
public:                                                                        \
    virtual ~className( ) = default;                                           \
                                                                               \
    className( const className& other ) = delete;                              \
                                                                               \
    className( className&& other ) = delete;                                   \
                                                                               \
    className& operator=( const className& other ) = delete;                   \
                                                                               \
    className& operator=( className&& other ) = delete

/**
 * @brief Declare class as interface
 *
 * Mark class as abstract, so that it cannot be instantiated. The resulting
 * virtual table must be defined using CVL_INTERFACE_IMPL().
 */
#define CVL_INTERFACE_DECL( className )                                        \
protected:                                                                     \
    className( );                                                              \
                                                                               \
public:                                                                        \
    virtual ~className( ) = 0;                                                 \
                                                                               \
    className( const className& other ) = delete;                              \
                                                                               \
    className( className&& other ) = delete;                                   \
                                                                               \
    className& operator=( const className& other ) = delete;                   \
                                                                               \
    className& operator=( className&& other ) = delete

/**
 * @brief Declare class as interface overriding an implemented interface
 *
 * Mark class as abstract, so that it cannot be instantiated. The resulting
 * virtual table must be defined using CVL_INTERFACE_IMPL().
 */
#define CVL_INTERFACE_DECL_OVERRIDE( className )                               \
protected:                                                                     \
    className( );                                                              \
                                                                               \
public:                                                                        \
    virtual ~className( ) override = 0;                                        \
                                                                               \
    className( const className& other ) = delete;                              \
                                                                               \
    className( className&& other ) = delete;                                   \
                                                                               \
    className& operator=( const className& other ) = delete;                   \
                                                                               \
    className& operator=( className&& other ) = delete

/**
 * @brief Define a classes virtual table
 *
 * Must be used together with CVL_INTERFACE_DECL() or INTERFACE_DECL_OVERRIDE()
 */
#define CVL_INTERFACE_IMPL( className )                                        \
    className::className( ) = default;                                         \
    className::~className( ) = default;

/**
 *
 * @brief Define class to be not copyable.
 */
#define CVT_DISABLE_COPY( className )                                          \
    className( const className& other ) = delete;                              \
                                                                               \
    className& operator=( const className& other ) = delete

/**
 *
 * @brief Define class to be not movable.
 */
#define CVT_DISABLE_MOVE( className )                                          \
    className( className&& other ) = delete;                                   \
                                                                               \
    className& operator=( className&& other ) = delete

/**
 *
 * @brief Define class to be not assignable.
 */
#define CVT_DISABLE_ASSIGNMENT( className )                                    \
                                                                               \
    className& operator=( const className& other ) = delete;                   \
                                                                               \
    className& operator=( className&& other ) = delete

/**
 * @brief Declare smart pointers to a type
 * @note @<memory@> must be included
 */
#define DECLARE_SMARTPTR( className )                                          \
    class className;                                                           \
    using className##UniquePtr = std::unique_ptr< className >;                 \
    using className##SharedPtr = std::shared_ptr< className >;                 \
    using className##WeakPtr = std::weak_ptr< className >;

#define EXPECT_MSG( expr, msg )                                                \
    do                                                                         \
    {                                                                          \
        if ( ! ( expr ) )                                                      \
        {                                                                      \
            std::stringstream ANONYMOUS_VARIABLE( ss );                        \
            ANONYMOUS_VARIABLE( ss ) << #expr << ": " << msg;                  \
            throw cvl::core::Error(                                            \
                __FILE__, __LINE__, ANONYMOUS_VARIABLE( ss ).str( ) );         \
        }                                                                      \
    } while ( 0 )

#define THROW_MSG( msg )                                                       \
    do                                                                         \
    {                                                                          \
        std::stringstream ANONYMOUS_VARIABLE( ss );                            \
        ANONYMOUS_VARIABLE( ss ) << msg;                                       \
        throw cvl::core::Error(                                                \
            __FILE__, __LINE__, ANONYMOUS_VARIABLE( ss ).str( ) );             \
    } while ( 0 )
