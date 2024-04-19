#pragma once

// CVL Includes
#include <cvl/core/Error.h>
#include <cvl/core/ILogger.h>
#include <cvl/core/Image.h>
#include <cvl/core/macros.h>

// STD includes
#include <memory_resource>

namespace cvl::core
{

class DebugMemoryResource : public std::pmr::memory_resource
{
public:
    DebugMemoryResource( std::string&& name,
                         std::pmr::memory_resource* upstream )
        : mName( std::move( name ) )
        , mUpstream( upstream )
    {
        EXPECT_MSG( upstream != nullptr, "Upstream resource is invalid" );
    }

    ~DebugMemoryResource( ) override {}

private:
    void* do_allocate( std::size_t bytes, std::size_t alignment ) override
    {
        LOG_INFO( "[" << mName << " (alloc)] Size: " << bytes
                      << " Alignment: " << alignment << " ..." );

        auto result = mUpstream->allocate( bytes, alignment );

        LOG_INFO( "[" << mName << "(alloc)] ... Address: " << result );

        return result;
    }

    void do_deallocate( void* p, std::size_t bytes,
                        std::size_t alignment ) override
    {
        LOG_INFO( "[" << mName << " (dealloc)] Address: << " << p
                      << " Dealloc Size: " << bytes
                      << " Alignment: " << alignment );

        mUpstream->deallocate( p, bytes, alignment );
    }

    bool do_is_equal(
        const std::pmr::memory_resource& other ) const noexcept override
    {
        return this == &other;
    }

    std::string mName;
    std::pmr::memory_resource* mUpstream;
};

} // namespace cvl::core