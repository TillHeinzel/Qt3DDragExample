set(TARGET_NAME ${CMAKE_PROJECT_NAME})

add_executable(${TARGET_NAME} 
  main.cpp 

  Glue/ModelAdapter.hpp
  Glue/ModelAdapter.cpp
)

target_link_libraries(${TARGET_NAME} 
  UI 
  Model
)
