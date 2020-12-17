set(TARGET_NAME ${CMAKE_PROJECT_NAME})

add_executable(${TARGET_NAME} 
  main.cpp 
)

target_link_libraries(${TARGET_NAME} UI)