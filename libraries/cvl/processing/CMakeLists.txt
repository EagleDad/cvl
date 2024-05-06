set( LIBRARY_NAME "CVL::Processing" )
get_raw_target_name( ${LIBRARY_NAME} LIBRARY_NAME_RAW )

add_library( ${LIBRARY_NAME_RAW} SHARED
    
    include/Processing.h

    include/cvl/processing/Test.h
    include/cvl/processing/Threshold.h

    src/Test.cpp
)

add_library( ${LIBRARY_NAME} ALIAS ${LIBRARY_NAME_RAW} )

target_include_directories( ${LIBRARY_NAME_RAW}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
        $<INSTALL_INTERFACE:include/>
        src/
)

set_compiler_warning_flags( 
    STRICT
    TARGET ${LIBRARY_NAME_RAW}
)

if( ENABLE_SOLUTION_FOLDERS )
    set_target_properties (${LIBRARY_NAME_RAW} PROPERTIES FOLDER "libraries" )
endif()

add_version_information(
    TARGET
        ${LIBRARY_NAME_RAW}
)

set_target_properties( ${LIBRARY_NAME_RAW} 
    PROPERTIES PUBLIC_HEADER 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
)

# Move to own cmake file
generate_export_header(${LIBRARY_NAME_RAW} EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/include/cvl/processing/export.h)
target_include_directories ( ${LIBRARY_NAME_RAW}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include/>
)

install (
    FILES ${CMAKE_CURRENT_BINARY_DIR}/include/cvl/processing/export.h 
    DESTINATION "include/cvl/core"
)

target_link_libraries( ${LIBRARY_NAME_RAW}
    PRIVATE
        # Define any private dependencies here
)

if( BUILD_TESTING )
    # Build unit tests only if testing is enabled
    add_subdirectory( test )
endif( BUILD_TESTING )

if( BUILD_BENCHMARK )
    # Build benchmark tests only if benchmarking is enabled
    add_subdirectory( benchmark )
endif( BUILD_BENCHMARK )

if ( STATIC_CODE_ANALYSIS )
    set_target_properties(${pargs_TARGET} PROPERTIES
        VS_GLOBAL_RunCodeAnalysis true

        # Use visual studio core guidelines
        VS_GLOBAL_EnableMicrosoftCodeAnalysis true

        # Use clangtidy
        VS_GLOBAL_EnableClangTidyCodeAnalysis true
        VS_GLOBAL_ClangTidyChecks "--config-file=${CMAKE_SOURCE_DIR}/.clang-tidy"
    )
endif ( STATIC_CODE_ANALYSIS )


# Tell CMake how to install our library
install(
    TARGETS ${LIBRARY_NAME_RAW}

    PUBLIC_HEADER DESTINATION "include"
    
    ARCHIVE  DESTINATION lib
    LIBRARY  DESTINATION bin
    RUNTIME  DESTINATION bin
)

install(
    FILES $<TARGET_PDB_FILE:${LIBRARY_NAME_RAW}> 
    DESTINATION bin
    OPTIONAL
)