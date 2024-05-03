
set(DOT_NET_VERSION "v4.7.2" CACHE STRING "The .NET version to use for the projects")
set_property(CACHE DOT_NET_VERSION PROPERTY STRINGS v4.8 v4.7.2 v4.6.2 v4.6.1 )

include(CppCliSupport)
include(CSharpUtilities)

######################################################################
#
# load_nuget_package
# net40 net461 net35 net 45 etc need to be a parameter
function(load_nuget_package packageName packageVersion)

    if ( ${packageName} STREQUAL "" OR ${packageVersion} STREQUAL "" )
        message(FATAL_ERROR "Invalid nuget package information for packageName (${packageName}) with packageVersion (${packageVersion})!")
    else()
        message(STATUS "Loading nuget package ${packageName} with version ${packageVersion}")
    endif()
    
    set(NUGET_PKG_FOUND FALSE PARENT_SCOPE)
    set(NUGET_DOTNET_REFERENCE)
    set(CMP_STRING "${packageName} ${packageVersion}")
    set(PACKAGE_DEST "${CMAKE_BINARY_DIR}/packages/${packageName}/${packageVersion}")
    set(OUT_VAL "EMPTY")
    set(RET_VAL)
    
    # Find nuget. It is loated in the repo
    if ( NOT NUGET_EXE )
        message(STATUS "Searching for nuget")
        find_program(NUGET_EXE NAMES nuget nuget
            PATHS "${CMAKE_SOURCE_DIR}/common/.nuget"
        )
    else( NOT NUGET_EXE )
        message(STATUS "nuget already found")
    endif( NOT NUGET_EXE )
    
    if ( NOT NUGET_EXE )
        message(FATAL_ERROR "nuget not found. Consider installation or check path")
    endif()
    
    # Check if package is already installed
    if ( EXISTS ${PACKAGE_DEST}/packages )
        exec_program(${NUGET_EXE}
            ARGS list ${packageName} -AllVersions -Source ${PACKAGE_DEST}
            OUTPUT_VARIABLE OUT_VAL
            RETURN_VALUE RET_VAL
        )
    endif()
    
    message(STATUS "OUT_VAL: ${OUT_VAL}")
    message(STATUS "RET_VAL: ${RET_VAL}")
    
    if ( ${CMP_STRING} STREQUAL ${OUT_VAL} )
        message(STATUS "${CMP_STRING} already installed. Skipping installation")
    else()
        message(STATUS "${CMP_STRING} not installed. Installing package")
        exec_program(${NUGET_EXE}
            ARGS install ${packageName} -Version ${packageVersion} -ExcludeVersion -OutputDirectory ${PACKAGE_DEST}
            OUTPUT_VARIABLE OUT_VAL
            RETURN_VALUE RET_VAL
        )
        
        if (NOT ${RET_VAL} EQUAL 0)
           message(FATAL_ERROR "Unable to install nuget package ${packageVersion} with version ${packageVersion}")
        endif()
        
    endif()
    
    ## OUT VAR
    set(NUGET_DOTNET_REFERENCE "${CMAKE_BINARY_DIR}/packages/${packageName}/${packageVersion}/${packageName}/lib/net45")
    FILE(GLOB OUT "${NUGET_DOTNET_REFERENCE}/*.dll")
    set(NUGET_DOTNET_REFERENCE ${OUT} PARENT_SCOPE)
    #message(STATUS "NUGET_DOTNET_REFERENCE: ${NUGET_DOTNET_REFERENCE}")

    set(NUGET_PKG_FOUND TRUE PARENT_SCOPE)

endfunction(load_nuget_package)
######################################################################

######################################################################
#
# load_nuget_package
function(add_nuget_package)

    message(STATUS "ARGN: ${ARGN}")

     cmake_parse_arguments(
        pargs 
        ""
        "TARGET;PACKAGE;VERSION"
        ""
        ${ARGN}
    )
    
    if(pargs_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "Unknown arguments: ${pargs_UNPARSED_ARGUMENTS}")
    endif()
    
    if ( NOT TARGET ${pargs_TARGET} )
         message(FATAL_ERROR "Invalid target")
    endif()

    message(STATUS "Adding nuget package ${pargs_PACKAGE} with version ${pargs_VERSION}")
    load_nuget_package(${pargs_PACKAGE} ${pargs_VERSION})
    
    if ( NOT ${NUGET_PKG_FOUND} )
        message(FATAL_ERROR "nuget package ${packageName} with version ${packageVersion} not foud")
    endif()
    
    get_target_property(
        EXISTING_PROPS
        ${LIBRARY_NAME} 
        VS_DOTNET_REFERENCES
    )
    
    message(STATUS "NUGET_DOTNET_REFERENCE: ${NUGET_DOTNET_REFERENCE}")
    message(STATUS "EXISTING_PROPS: ${EXISTING_PROPS}")
    
    set_property(
        TARGET
            ${pargs_TARGET} 
        PROPERTY VS_DOTNET_REFERENCES
            ${EXISTING_PROPS}
            ${NUGET_DOTNET_REFERENCE}
    )

endfunction(add_nuget_package)
######################################################################

######################################################################
#
# add_csharp_executable
function(add_csharp_executable)

    cmake_parse_arguments(
        pargs 
        "CONSOLE"
        "TARGET"
        "SOURCES;CONFIGURATION;PROPERTIES;RESOURCES;DOT_NET_REFERENCES;FILES"
        ${ARGN}
    )
    
    if(pargs_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "Unknown arguments: ${pargs_UNPARSED_ARGUMENTS}")
    endif()
    
    if( "TARGET" IN_LIST pargs_KEYWORDS_MISSING_VALUES)
        message(FATAL_ERROR "TARGET: not defined!")
    endif()
    
    source_group("Source Files"
        FILES
            ${pargs_SOURCES}
    )
    
    source_group("Resource Files"
        FILES
            ${pargs_RESOURCES}
    )
    
    add_executable(${pargs_TARGET}
        ${pargs_CONFIGURATION}
        ${pargs_SOURCES}
        ${pargs_PROPERTIES}
        ${pargs_RESOURCES}
        ${pargs_FILES}
    )

    if ( ${pargs_CONSOLE})
        set(pargs_CONSOLE FALSE)
    else()
        set(pargs_CONSOLE TRUE)
    endif()
    
    set_property(
        TARGET
            ${pargs_TARGET}
        
        PROPERTY
            WIN32_EXECUTABLE ${pargs_CONSOLE}
    )
    
    set_property(
        TARGET
            ${pargs_TARGET}
        
        PROPERTY
            VS_DOTNET_TARGET_FRAMEWORK_VERSION "${DOT_NET_VERSION}"
    )
    
    set_property(
        TARGET
            ${pargs_TARGET} 
            
        PROPERTY VS_DOTNET_REFERENCES
            ${pargs_DOT_NET_REFERENCES}
    )
    
    set_property(
        SOURCE 
            ${pargs_RESOURCES}
            
        PROPERTY VS_TOOL_OVERRIDE
            "Resource"
    )
    
    set_property(
        SOURCE 
            ${pargs_FILES}
            
        PROPERTY VS_TOOL_OVERRIDE
            "Resource"
    )
    
    set_property(
        SOURCE
            ${pargs_FILES}
        PROPERTY HEADER_FILE_ONLY
          TRUE # Since it's just a text file, it doesn't need compiled
    )

endfunction(add_csharp_executable)
######################################################################

######################################################################
#
# add_csharp_library
function(add_csharp_library)

    cmake_parse_arguments(
        pargs 
        ""
        "TARGET"
        "SOURCES;CONFIGURATION;PROPERTIES;RESOURCES;DOT_NET_REFERENCES;FILES"
        ${ARGN}
    )
    
    if(pargs_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "Unknown arguments: ${pargs_UNPARSED_ARGUMENTS}")
    endif()
    
    if( "TARGET" IN_LIST pargs_KEYWORDS_MISSING_VALUES)
        message(FATAL_ERROR "TARGET: not defined!")
    endif()
    
    source_group("Source Files"
        FILES
            ${pargs_SOURCES}
    )
    
    source_group("Resource Files"
        FILES
            ${pargs_RESOURCES}
    )
    
    add_library(${pargs_TARGET}
        ${pargs_CONFIGURATION}
        ${pargs_SOURCES}
        ${pargs_PROPERTIES}
        ${pargs_RESOURCES}
        ${pargs_FILES}
    )
    
    set_property(
        TARGET
            ${pargs_TARGET}
        
        PROPERTY
            WIN32_EXECUTABLE FALSE
    )
    
    set_property(
        TARGET
            ${pargs_TARGET}
        
        PROPERTY
            VS_DOTNET_TARGET_FRAMEWORK_VERSION "${DOT_NET_VERSION}"
    )
    
    set_property(
        TARGET
            ${pargs_TARGET} 
            
        PROPERTY VS_DOTNET_REFERENCES
            ${pargs_DOT_NET_REFERENCES}
    )
    
    set_property(
        SOURCE 
            ${pargs_RESOURCES}
            
        PROPERTY VS_TOOL_OVERRIDE
            "Resource"
    )

endfunction(add_csharp_library)
######################################################################