
# Internal function
function(copy_runtime_dependency_sync file_path dest)
    # Uncomment following line for verbose output:
    # message(STATUS "Syncing \"${dll_path}\" to \"${dest}\"")

    # file(COPY) copies the file only if the timestamp of the source and destination are different.
    file(COPY ${file_path} DESTINATION "${dest}")
endfunction()

# Internal function
function(copy_runtime_dependency_for_config arg config dest)
    if(TARGET ${arg})
        # If the build type is not supported by the target, get_target_property
        # will return the default LOCATION.
        get_target_property(dll_path ${arg} "LOCATION_${config}")
    else()
        set(dll_path ${arg})
    endif()

    if("${dll_path}" MATCHES "\.a$" OR "${dll_path}" MATCHES "\.lib$")
        # Skip static libraries
        return()
    elseif(NOT "${dll_path}" MATCHES "\.dll$")
        message(WARNING "Cannot deploy DLL for ${arg}: ${dll_path}")
        return()
    else()
        copy_runtime_dependency_sync(${dll_path} "${dest}")
    endif()
endfunction()

# Internal function
function(copy_runtime_dependency arg)
    if("${arg}" MATCHES "\.a$" OR "${arg}" MATCHES "\.lib$")
        # Skip static libraries
        return()
    endif()

    if(NOT ${arg} MATCHES "\.dll$")
        string(REGEX MATCH ".*TARGET_FILE:(.*)>" dummy ${arg})
        if(NOT ${CMAKE_MATCH_1} EQUAL "")
            # "arg" is a generator expression
            message(FATAL_ERROR "Generator expression makes no sense, DLLs are copied during configuration phase")
        else()
            # "arg" is a target name, e.g. "Qt5::Core"

            # Reset _type, it might still be set from the parent scope in case
            # of recursive calls, and get_target_property will not
            # reset it in case of error.
            set(_type)
            get_target_property(_type ${arg} TYPE)

            if("${_type}" STREQUAL "INTERFACE_LIBRARY")
                # For INTERFACE_LIBRARY's, we need to copy the referenced libraries
                get_target_property(_list ${arg} "INTERFACE_LINK_LIBRARIES")
                foreach(_link_lib ${_list})
                    copy_runtime_dependency(${_link_lib})
                endforeach()
                return()
            endif()
        endif()
    endif()

    get_property(_isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)

    if("${_isMultiConfig}")
        foreach(_config ${CMAKE_CONFIGURATION_TYPES})
            # Unfortunately, CMAKE_RUNTIME_OUTPUT_DIRECTORY_${_config} isn't set, so
            # we have to generate the path ourselves
            set(_dest "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${_config}")
            copy_runtime_dependency_for_config(${arg} ${_config} ${_dest})
        endforeach()
    else()
        copy_runtime_dependency_for_config(${arg} ${CMAKE_BUILD_TYPE} "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
    endif()
endfunction()

# Main function
function(copy_runtime_dependencies)
    if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
        foreach(arg ${ARGN})
            copy_runtime_dependency(${arg})
        endforeach()
    endif()
endfunction()
