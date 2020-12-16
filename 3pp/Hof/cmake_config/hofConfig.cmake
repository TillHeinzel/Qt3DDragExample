include(FindPackageHandleStandardArgs)
set(${CMAKE_FIND_PACKAGE_NAME}_CONFIG ${CMAKE_CURRENT_LIST_FILE})
find_package_handle_standard_args(hof CONFIG_MODE)

if(NOT TARGET hof::hof)
    include("${CMAKE_CURRENT_LIST_DIR}/hofTargets.cmake")
    if((NOT TARGET hof) AND
       (NOT hof_FIND_VERSION OR
        hof_FIND_VERSION VERSION_LESS 3.2.0))
        add_library(hof INTERFACE IMPORTED)
        set_target_properties(hof PROPERTIES
            INTERFACE_LINK_LIBRARIES hof::hof
        )
    endif()
endif()
