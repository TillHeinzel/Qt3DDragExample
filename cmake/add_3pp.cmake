
if(GET_DEPENDENCIES)
  include(${PROJECT_ROOT_DIR}/cmake/install_dependencies.cmake)
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR} ${CMAKE_MODULE_PATH})

find_package(Boost 1.73.0 REQUIRED COMPONENTS headers)

if(MSVC)
  target_compile_definitions(Boost::Boost INTERFACE _WIN32_WINNT=0x0A00)
endif()

# Magic_enum
list(APPEND CMAKE_PREFIX_PATH "${PROJECT_ROOT_DIR}/3pp/magic_enum/cmake_config")
find_package(magic_enum REQUIRED)

# modified Boost::hof
list(APPEND CMAKE_PREFIX_PATH "${PROJECT_ROOT_DIR}/3pp/Hof/cmake_config")
find_package(hof REQUIRED)

# Doctest
list(APPEND CMAKE_PREFIX_PATH "${PROJECT_ROOT_DIR}/3pp/doctest/cmake")
find_package(doctest REQUIRED)

target_compile_definitions(Doctest::Doctest INTERFACE DOCTEST_CONFIG_SUPER_FAST_ASSERTS)

message(STATUS "PREFIX path: ${CMAKE_PREFIX_PATH}")

list(APPEND CMAKE_PREFIX_PATH "C:/Qt/5.15.2/msvc2019_64/lib/cmake")
# Qt
find_package(Qt5 COMPONENTS 
	Core 
    3DCore
    3DRender
    3DInput
    3DExtras
	Widgets 
REQUIRED)
