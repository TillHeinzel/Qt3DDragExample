if(GCC)
  set(CONAN_SETTINGS "compiler.libcxx=libstdc++11")
endif()

include(${PROJECT_ROOT_DIR}/cmake/conan.cmake)

conan_cmake_run(CONANFILE conanfile.txt BUILD missing PROFILE_AUTO ALL SETTINGS ${CONAN_SETTINGS})
