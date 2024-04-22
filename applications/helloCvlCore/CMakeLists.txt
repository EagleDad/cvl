set(EXECUTABLE_NAME "helloCvlCore")

find_package( TBB REQUIRED )

add_executable(${EXECUTABLE_NAME}
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
        TBB::tbb
        TBB::tbbmalloc
        TBB::tbbmalloc_proxy

    PRIVATE

    INTERFACE
)