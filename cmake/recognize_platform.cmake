
if(CMAKE_SYSTEM_NAME STREQUAL Windows)
  set(IS_WINDOWS true)
else()
  set(IS_WINDOWS false)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL Linux)
  set(IS_LINUX true)
else()
  set(IS_LINUX false)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL Darwin)
  set(IS_MAC true)
else()
  set(IS_MAC false)
endif()
