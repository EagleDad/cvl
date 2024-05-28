include(CompanyInfo)

set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}")

if ( ${BUILD_TYPE} STREQUAL "Official")
    set(PROJECT_VERSION_FULL "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
else()
    set(PROJECT_VERSION_FULL "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}.${PROJECT_VERSION_TWEAK}")
endif()


if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.git")
    
    # SET TWEAK only in un-offiicaila builds- Provide cmake option
    
    execute_process(COMMAND git describe --always --first-parent HEAD
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        OUTPUT_VARIABLE GIT_DESCRIBE_TAGS ERROR_QUIET)
        
    if(GIT_DESCRIBE_TAGS)
        string ( SUBSTRING ${GIT_DESCRIBE_TAGS} 0 1 VERSION_IDENT )
        if ( ${BUILD_TYPE} STREQUAL "Official" OR VERSION_IDENT STREQUAL "v" )
            set(PROJECT_VERSION_FULL "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
        else()
            math(EXPR decval "0x${GIT_DESCRIBE_TAGS}" OUTPUT_FORMAT DECIMAL)
            set(PROJECT_VERSION_TWEAK ${decval})
            set(PROJECT_VERSION_FULL "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}.${PROJECT_VERSION_TWEAK}")
        endif()
            
    endif(GIT_DESCRIBE_TAGS)
    
endif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.git")

function( add_version_information )

    if(NOT CMAKE_CXX_COMPILER_ID MATCHES MSVC)
		return ()
	endif()

    set(options)
    set(oneValueArgs TARGET )
    set(multiValueArgs)
    cmake_parse_arguments(pargs "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    
    # TARGET must be a CMake executable or shared library
    if ( NOT TARGET ${pargs_TARGET} )
        message( FATAL_ERROR "TARGET '${pargs_TARGET}' is not a CMake target" )
    endif()
    
    # Get the target type.
    get_target_property( _type ${pargs_TARGET} TYPE )
    if ( NOT ( ${_type} STREQUAL "EXECUTABLE" OR ${_type} STREQUAL "SHARED_LIBRARY" ) )
        message( FATAL_ERROR "TARGET '${pargs_TARGET}' must be an executable or shared library." )
    endif()
    
    # Get targets output name
    if (  ${_type} STREQUAL "EXECUTABLE" )
        set( OUTPUT_NAME ${pargs_TARGET}.exe )
    else()
        set( OUTPUT_NAME ${pargs_TARGET}.dll )
    endif()
    
    get_target_property( _target_binary_dir ${pargs_TARGET} BINARY_DIR )
	
	if ( ${BUILD_TYPE} STREQUAL "Official" OR VERSION_IDENT STREQUAL "v" )
        # Create the final rc file based on the parameters
		configure_file( ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../templates/VERSIONINFO_OFFICIAL.rc.in
			${_target_binary_dir}/VERSIONINFO.rc
		)

		# Add the rc file to the target
		target_sources( ${pargs_TARGET} PRIVATE ${_target_binary_dir}/VERSIONINFO.rc )
    else()
		# Create the final rc file based on the parameters
		configure_file( ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../templates/VERSIONINFO.rc.in
			${_target_binary_dir}/VERSIONINFO.rc
		)

		# Add the rc file to the target
		target_sources( ${pargs_TARGET} PRIVATE ${_target_binary_dir}/VERSIONINFO.rc )
	endif()
    
   
endfunction( add_version_information )