set(TARGET_NAME UI)

add_testable_lib(${TARGET_NAME}  
  UI.hpp
  UI.cpp
  
  IModel.hpp
  IModel.cpp

  detail/SceneWidget.hpp
  detail/SceneWidget.cpp
  
  detail/initializeContent.hpp 
  detail/initializeContent.cpp 
)

target_link_libraries(${TARGET_NAME} PUBLIC
  Qt::Widgets 
  Qt::3DExtras 
  Qt::3DInput 
  Qt::3DRender 
  Qt::3DCore 
  Qt::Core
)

target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
