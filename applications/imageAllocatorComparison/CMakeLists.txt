set( EXECUTABLE_NAME "imageAllocatorComparison" )

find_package( TBB QUIET )

add_executable( ${EXECUTABLE_NAME}
    main.cpp
)

set_compiler_warning_flags( 
    STRICT
    TARGET ${EXECUTABLE_NAME}
)


if( ENABLE_SOLUTION_FOLDERS )
    set_target_properties( ${EXECUTABLE_NAME} PROPERTIES FOLDER "applications" )
endif()

target_include_directories( ${EXECUTABLE_NAME}
    INTERFACE
    
    PUBLIC
)

target_link_libraries( ${EXECUTABLE_NAME}
    PUBLIC
        CVL::Core

    PRIVATE

    INTERFACE
)

if( ${TBB_FOUND} )

    message(STATUS "TBB ${TBB_VERSION} found")

    target_link_libraries( ${EXECUTABLE_NAME}
        PUBLIC

        PRIVATE
            TBB::tbb
            TBB::tbbmalloc
            TBB::tbbmalloc_proxy

        INTERFACE
    )

    target_compile_definitions( ${EXECUTABLE_NAME}
        PUBLIC BUILD_WITH_TBB
    )

endif( ${TBB_FOUND} )