set(CMAKE_UNITY_BUILD ON)
set(CMAKE_UNITY_BUILD_BATCH_SIZE 25)

add_executable(${CMAKE_PROJECT_NAME} 
  main.cpp 
  
  SceneWidget.hpp
  SceneWidget.cpp

  CubeManager.cpp 
  CubeManager.hpp
)

target_link_libraries(${CMAKE_PROJECT_NAME} Qt::Widgets Qt::3DExtras Qt::3DInput Qt::3DRender Qt::3DCore Qt::Core)