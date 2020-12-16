if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")

    add_compile_options(/wd4068) #no warning on unknown pragmas

elseif(GCC)
    add_compile_options(-Wfatal-errors) # stop on first error
    add_compile_options(-Wall -Wno-long-long -Wno-unknown-pragmas)
elseif(CLANG)
    add_compile_options(-Weverything
    -Wno-c++98-compat -Wno-c++98-c++11-compat -Wno-c++98-c++11-c++14-compat -Wno-c++98-compat-pedantic 
    -Wno-padded -Wno-newline-eof -Wno-unknown-pragmas -Wno-global-constructors -Wno-exit-time-destructors 
    -Wno-weak-vtables -Wno-missing-noreturn -Wno-covered-switch-default -Wno-braced-scalar-init 
    -Wno-unknown-warning-option -Wno-unused-macros -Wno-missing-field-initializers -Wno-missing-braces)
    
    add_compile_options(-Wno-header-hygiene) # header hygiene does not play nicely with unity builds, which include cpp files
endif()
