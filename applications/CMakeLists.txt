add_subdirectory( helloCvlCore )
add_subdirectory( helloCvlProcessing )
add_subdirectory( imageAllocatorComparison )

if ( MSVC )

    if ( BUILD_WITH_CLR )

    endif( BUILD_WITH_CLR )

endif( MSVC )

if (  Qt6_FOUND )
    add_subdirectory( qtImageViewer )
endif ( )