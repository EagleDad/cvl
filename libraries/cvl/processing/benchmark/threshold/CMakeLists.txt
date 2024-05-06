set( EXECUTABLE_NAME "benchmark_cvl_threshold" )

find_package( TBB REQUIRED )


add_benchmark_executable(
    TARGET
        ${EXECUTABLE_NAME}

    HEADERS

    SOURCES
        src/benchmark_cvl_threshold.cpp
       
    DEPENDENCIES
        CVL::Core
)

set_compiler_warning_flags( 
    STRICT
    TARGET ${EXECUTABLE_NAME}
)
