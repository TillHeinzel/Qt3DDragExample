set(TARGET_NAME UI)

add_library(${TARGET_NAME} STATIC 
  UI.hpp
  UI.cpp
  
  SceneWidget.hpp
  SceneWidget.cpp

  CubeManager.cpp 
  CubeManager.hpp
)

target_link_libraries(${TARGET_NAME} 
  Qt::Widgets 
  Qt::3DExtras 
  Qt::3DInput 
  Qt::3DRender 
  Qt::3DCore 
  Qt::Core
)
