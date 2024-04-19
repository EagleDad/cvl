include(FetchContent)
include(ExternalProject)

project ( SuperBuild NONE )

set( DEPENDENCIES )
set( EXTRA_CMAKE_ARGS )

list( APPEND
    EXTRA_CMAKE_ARGS
        -DUSE_SUPERBUILD=OFF
)

if( BUILD_TESTING )
    include( GoogleTestSupport )
endif( BUILD_TESTING )

if( BUILD_BENCHMARK )
    include( BenchmarkSupport )
endif( BUILD_BENCHMARK )


# FIXME add to default target "all"?
ExternalProject_Add (superBuild
    DEPENDS
        ${DEPENDENCIES}
        
    SOURCE_DIR
        ${PROJECT_SOURCE_DIR}
        
    CMAKE_ARGS
        ${EXTRA_CMAKE_ARGS}
        
    INSTALL_COMMAND
        ""
        
    BINARY_DIR
        ${CMAKE_CURRENT_BINARY_DIR}
)