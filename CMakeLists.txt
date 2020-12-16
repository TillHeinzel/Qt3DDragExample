cmake_minimum_required (VERSION 3.16)
set (CMAKE_USER_MAKE_RULES_OVERRIDE "${CMAKE_CURRENT_LIST_DIR}/cmake/modify_default_settings.cmake") 
project (Qt3DDragExample)

set(PROJECT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
## basics
include("cmake/recognize_platform.cmake")
include("cmake/recognize_compiler.cmake")
include("cmake/recognize_build_type.cmake")

include("cmake/set_output_paths.cmake")

## Compilation
include("cmake/set_cpp_standard.cmake")

include("cmake/set_warning_level_to_max.cmake")
include("cmake/set_warnings_as_errors.cmake")
include("cmake/set_ignore_warnings_from_thirdparty.cmake")

## final project specific steps. Order matters
include("cmake/add_3pp.cmake")
include("cmake/setup_Qt.cmake")
include("cmake/setup_testing.cmake")

add_subdirectory("src")
