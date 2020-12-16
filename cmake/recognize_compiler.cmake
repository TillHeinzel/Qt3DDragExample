
if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    set(GCC true)
else()
    set(GCC false)
endif()

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" )
    set(CLANG true)
else()
    set(CLANG false)
endif()

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "AppleClang")
    set(APPLE_CLANG true)
else()
    set(APPLE_CLANG false)
endif()
