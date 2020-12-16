# remove defaults for warning level, so it can be set later
string(REGEX REPLACE "/W[0-4]" "" CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT}")

#get_cmake_property(_varNames VARIABLES)
#list (REMOVE_DUPLICATES _varNames)
#list (SORT _varNames)
#foreach (_varName ${_varNames})
#        message(STATUS "${_varName}=${${_varName}}")
#endforeach()
