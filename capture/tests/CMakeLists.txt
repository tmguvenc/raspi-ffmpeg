# CMake build script for ZeroMQ tests
cmake_minimum_required(VERSION "2.8.1")

# name of Windows project will tests.sln
PROJECT(tests)

set(tests test_webcam)
# add library and include dirs for all targets
link_libraries(capture)
include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories("${CMAKE_SOURCE_DIR}/src")

foreach(test ${tests})
  add_executable(${test} ${test}.cpp)
  add_test(NAME ${test} COMMAND ${test})
  set_tests_properties(${test} PROPERTIES TIMEOUT 10)
endforeach()

#Check whether all tests in the current folder are present
file(READ "${CMAKE_CURRENT_LIST_FILE}" CURRENT_LIST_FILE_CONTENT)
file(GLOB ALL_TEST_SOURCES "test_*.cpp")
foreach(TEST_SOURCE ${ALL_TEST_SOURCES})
  get_filename_component(TESTNAME "${TEST_SOURCE}" NAME_WE)
  string(REGEX MATCH "${TESTNAME}" MATCH_TESTNAME "${CURRENT_LIST_FILE_CONTENT}")
  if (NOT MATCH_TESTNAME)
    message(AUTHOR_WARNING "Test '${TESTNAME}' is not known to CTest.")
  endif()
endforeach()

