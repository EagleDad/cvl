set(EXECUTABLE_NAME "qtImageViewer")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(Qt6 COMPONENTS Widgets)

qt_add_executable( ${EXECUTABLE_NAME}
    imageviewer.cpp imageviewer.h
    main.cpp
)

set_compiler_warning_flags( 
    STRICT
    TARGET ${EXECUTABLE_NAME}
)


if(ENABLE_SOLUTION_FOLDERS)
    set_target_properties(${EXECUTABLE_NAME} PROPERTIES FOLDER "applications")
endif()

target_include_directories(${EXECUTABLE_NAME}
    INTERFACE
    
    PUBLIC
)

target_link_libraries(${EXECUTABLE_NAME}
    PUBLIC
        CVL::Core
        CVL::Processing
        Qt::Core
        Qt::Gui
        Qt::Widgets

    PRIVATE

    INTERFACE
)

target_compile_options( ${EXECUTABLE_NAME}
                PRIVATE
                    /wd4464
            )

copy_runtime_dependencies(
    Qt::Core
    Qt::Gui
    Qt::Widgets
)
