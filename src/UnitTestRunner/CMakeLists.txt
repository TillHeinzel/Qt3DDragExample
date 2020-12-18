set(TARGET_NAME UnitTestRunner)

add_executable(${TARGET_NAME} Main.cpp)
target_link_libraries(${TARGET_NAME} PUBLIC Doctest::Doctest)

get_property(TESTABLE_LIBS GLOBAL PROPERTY TESTABLE_LIBS)

target_link_libraries(${TARGET_NAME} PUBLIC
  ${TESTABLE_LIBS} 
)

doctest_discover_tests(${TARGET_NAME})
