if(CMAKE_BUILD_TYPE MATCHES Release)
  set(IS_RELEASE TRUE)
else()
  set(IS_RELEASE FALSE)
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
  set(IS_DEBUG TRUE)
else()
  set(IS_DEBUG FALSE)
endif()

