set(EXECUTABLE_NAME "test_core")

add_gtest_executable(
    TARGET
        ${EXECUTABLE_NAME}

    HEADERS

    SOURCES
        src/test_AccessTraits.cpp
        src/test_AlignedAllocator.cpp
        src/test_Alignment.cpp
        src/test_Compare.cpp
        src/test_Contour.cpp
        src/test_DimensionTraits.cpp
        src/test_Ellipse.cpp
        src/test_Error.cpp
        src/test_Handle.cpp
        src/test_Image.cpp
        src/test_Line.cpp
        src/test_Logger.cpp
        src/test_NormTraits.cpp
        src/test_ObserverHandle.cpp
        src/test_Point.cpp
        src/test_Rectangle.cpp
        src/test_Region.cpp
        src/test_Size.cpp
        src/test_SynchronizedQueue.cpp
        src/test_Vector.cpp

    DEPENDENCIES
        CVL::Core
)

if ( MSVC )
    target_compile_options(${EXECUTABLE_NAME} PRIVATE /bigobj)
endif( )