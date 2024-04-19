set( EXECUTABLE_NAME "benchmark_cvl_core" )


add_benchmark_executable(
    TARGET
        ${EXECUTABLE_NAME}

    HEADERS

    SOURCES
        src/benchmark_cvl_core.cpp
       
    DEPENDENCIES
        CVL::Core

)

set_compiler_warning_flags( 
    STRICT
    TARGET ${EXECUTABLE_NAME}
)
