set( EXECUTABLE_NAME "benchmark_cvl_image" )

find_package( TBB REQUIRED )


add_benchmark_executable(
    TARGET
        ${EXECUTABLE_NAME}

    HEADERS

    SOURCES
        src/benchmark_cvl_image.cpp
       
    DEPENDENCIES
        CVL::Core
        TBB::tbb
        TBB::tbbmalloc
        TBB::tbbmalloc_proxy

)

set_compiler_warning_flags( 
    STRICT
    TARGET ${EXECUTABLE_NAME}
)
