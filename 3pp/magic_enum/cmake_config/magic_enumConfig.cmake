include(FindPackageHandleStandardArgs)
set(${CMAKE_FIND_PACKAGE_NAME}_CONFIG ${CMAKE_CURRENT_LIST_FILE})
find_package_handle_standard_args(magic_enum CONFIG_MODE)

if(NOT TARGET magic_enum::magic_enum)
    include("${CMAKE_CURRENT_LIST_DIR}/magic_enumTargets.cmake")
    if((NOT TARGET magic_enum) AND
       (NOT magic_enum_FIND_VERSION OR
        magic_enum_FIND_VERSION VERSION_LESS 3.2.0))
        add_library(magic_enum INTERFACE IMPORTED)
        set_target_properties(magic_enum PROPERTIES
            INTERFACE_LINK_LIBRARIES magic_enum::magic_enum
        )
    endif()
endif()
