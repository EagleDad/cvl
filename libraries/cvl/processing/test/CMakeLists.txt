set(EXECUTABLE_NAME "test_processing")

add_gtest_executable(
    TARGET
        ${EXECUTABLE_NAME}

    HEADERS

    SOURCES
        src/test_Area.cpp
        src/test_BoundingBox.cpp
        src/test_Center.cpp
        src/test_ConnectedComponents.cpp
        src/test_Threshold.cpp

    DEPENDENCIES
        CVL::Core
        CVL::Processing
)

if ( MSVC )
    target_compile_options(${EXECUTABLE_NAME} PRIVATE /bigobj)
endif( )