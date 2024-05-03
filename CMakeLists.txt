cmake_minimum_required( VERSION 3.11 )

# Configure C++
set( CMAKE_CXX_STANDARD 20 )

set( CMAKE_CONFIGURATION_TYPES "Debug;Release" )

set( COPYRIGHT_YEAR_START 2024 )

set( PROJECT_VERSION_MAJOR "1" CACHE STRING "The major version. Will only change if interface in incompatible to the version before." )
set( PROJECT_VERSION_MINOR "0" CACHE STRING "The minor version. Will change with essential changes." )
set( PROJECT_VERSION_PATCH "0" CACHE STRING "The patch version. Will change for bug fixed." )
set( PROJECT_VERSION_TWEAK "0" CACHE STRING "The tewak. Will change with every buil in development status" )

list( APPEND CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Checks
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Common
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/External
)

# Force all libraries to be build dynamic if ON, also they are set to STATIC
# If we set this flag to on and use gtest with FectContent we have to
# implement the main function for our unit tests by ourself
option( BUILD_SHARED_LIBS            "Build project libraries as shared libraries" OFF )
option( BUILD_TESTING                "Build with tests" OFF )
option( BUILD_BENCHMARK              "Build with benchmark tests" OFF )
option( BUILD_DOC                    "Build documentation" OFF )
option( DEBUG_POSTFIX                "Set to ON to force _d postfix for the lib names." ON )
option( ENABLE_SOLUTION_FOLDERS      "Solution folder in Visual Studio or in other IDEs" ON )
option( STATIC_CODE_ANALYSIS         "Enables static code analysis with clang-tidy and CppCheck, if installed." OFF )
option( COVERAGE_ON                  "Enable code coverage" OFF )

# 3rd party dependencies needs to be build first
option ( USE_SUPERBUILD              "Whether or not a superbuild should be invoked" ON )

# Build type
set( BUILD_TYPE "Engineering" CACHE STRING "The build type to use for the projects" )
set_property( CACHE BUILD_TYPE PROPERTY STRINGS Internal Official Engineering )

if ( USE_SUPERBUILD )
    # execute the superbuild (this script will be invoked again without the
    # USE_SUPERBUILD option this time)
    include( SuperBuild )
    # stop processing this file further
    return( )
else( USE_SUPERBUILD )
    include( VersionInfo )
    project( Cvl-Core VERSION ${PROJECT_VERSION_FULL} )
endif( USE_SUPERBUILD )

if( ENABLE_SOLUTION_FOLDERS )
    set_property( GLOBAL PROPERTY USE_FOLDERS ON )
    set_property( GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMakeTargets" )
endif( )

# Standard CMake modules
include(FindPackageHandleStandardArgs) 
include(GenerateExportHeader)

# Modules
include(Common)

if ( MSVC )
    list( APPEND CMAKE_MODULE_PATH
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/.NetSupport
    )
endif( MSVC )

if ( MSVC )
    option( BUILD_WITH_CLR            "Build with projects" OFF )
endif( MSVC )

if( DEBUG_POSTFIX )
    # For debug libs and exes, add "-d" postfix
    set( CMAKE_DEBUG_POSTFIX "-d" CACHE STRING "Set debug library postfix" )
endif( DEBUG_POSTFIX )

if( BUILD_TESTING )
     if( COVERAGE_ON )
        include( CodeCoverageSupport )
        if ( MSVC )
            include( OpenCppCoverageCheck )
        endif( MSVC )
    endif( COVERAGE_ON )

    # Activate test support
    # Enable integration with CTest
    # The shared libs option needs to be turne off to be alble to use gtest main function 
    # instead of writing every time our own. Maybe for all test executables a own fucntion is required
    include( CTest )
    include( GoogleTestTesting )
    enable_testing( )
endif( BUILD_TESTING )

if( BUILD_BENCHMARK )
    include( BenchmarkIntegration )
endif( BUILD_BENCHMARK )

if( BUILD_DOC )
    #include(DoxygenSupport)
    #add_subdirectory(doc)
endif( BUILD_DOC )

# Add clang-format for all C++ projects
# and editorconfig for all .NET projects
add_custom_target(ALL
    SOURCES 
        .clang-format
        .editorconfig
        .clang-tidy
)

set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Installation Directory" FORCE)

message(STATUS "")
message(STATUS "  Install to: " "${CMAKE_INSTALL_PREFIX}")
message(STATUS "-----------------------------------------------------------------")
message(STATUS "")

# Let CMake load the CMakeLists.txt in the subdirectory to load the
# sub-projects. Prefer lower-case names.
add_subdirectory(libraries)
add_subdirectory(applications)

message(STATUS "Version: ${PROJECT_VERSION_FULL}")

set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION_FULL})

#Set the name of the generated installation package
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})

#Set the directory path to install to
set(CPACK_PACKAGE_DIRECTORY "${CMAKE_BINARY_DIR}/packages") 

set(CPACK_PACKAGE_VENDOR "bucher emhart glass")

#set(CPACK_NUGET_PACKAGE_LICENSE_FILE_NAME "${CMAKE_SOURCE_DIR}/License")

if(WIN32)
    set(CPACK_NUGET_PACKAGE_TAGS "Native" "native")
    set(CPACK_GENERATOR ZIP NuGet)
else()
    set(CPACK_GENERATOR TGZ)
endif()

set(CPACK_INSTALL_CMAKE_CONFIGURATIONS Debug Release)

include(CPack)

