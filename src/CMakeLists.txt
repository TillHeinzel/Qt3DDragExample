#set(CMAKE_UNITY_BUILD ON)
#set(CMAKE_UNITY_BUILD_BATCH_SIZE 25)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(Model)
add_subdirectory(UI)
add_subdirectory(Program)
