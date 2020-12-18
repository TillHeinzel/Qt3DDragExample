set(TARGET_NAME ${CMAKE_PROJECT_NAME})

add_testable_lib(${TARGET_NAME}_obj 
  Glue/ModelAdapter.hpp
  Glue/ModelAdapter.cpp
)

target_link_libraries(${TARGET_NAME}_obj PUBLIC 
  Qt::Gui
  Doctest::Doctest
)

add_executable(${TARGET_NAME} 
  main.cpp 
  DummyTestConfig.cpp
)

target_link_libraries(${TARGET_NAME} PRIVATE
  ${TARGET_NAME}_obj

  UI 
  Model
)
