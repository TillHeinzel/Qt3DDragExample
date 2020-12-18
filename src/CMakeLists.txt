#set(CMAKE_UNITY_BUILD ON)
#set(CMAKE_UNITY_BUILD_BATCH_SIZE 25)


define_property(GLOBAL PROPERTY TESTABLE_LIBS BRIEF_DOCS " " FULL_DOCS " ")
set_property(GLOBAL PROPERTY TESTABLE_LIBS "")

function(add_testable_lib TARGET_NAME)
  add_library(${TARGET_NAME} OBJECT ${ARGN})
  set_property(GLOBAL APPEND PROPERTY TESTABLE_LIBS ${TARGET_NAME})
endfunction()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(Model)
add_subdirectory(UI)
add_subdirectory(Program)
add_subdirectory(UnitTestRunner)
