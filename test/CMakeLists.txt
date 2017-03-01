# Project Name
project(tests)

set(VERSION_MAJOR "1" CACHE STRING "Major")
set(VERSION_MINOR "0" CACHE STRING "Minor")
set(VERSION_TYPE "0" CACHE STRING "Type")
set(VERSION_BUILD "0" CACHE STRING "Build")

message(STATUS "Version: " ${VERSION_MAJOR} "." ${VERSION_MINOR} "." ${VERSION_TYPE} "." ${VERSION_BUILD})

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/version.rc.in
  ${CMAKE_CURRENT_BINARY_DIR}/version.rc
  @ONLY)

FILE(GLOB TARGET_H 							"include/*.h*" "src/*.h*")
FILE(GLOB TARGET_SRC		 				"src/*.c*")

if (NOT MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
endif()

enable_testing()

add_definitions(-DGTEST_LINKED_AS_SHARED_LIBRARY)

include_directories(include/)
include_directories("../capture-api/include/")
include_directories("../common/include/")
include_directories(${TBB_INCLUDE})
include_directories(${FFMPEG_INCLUDE})
include_directories(${SPDLOG_INCLUDE})
include_directories(${ZEROMQ_INCLUDE})
include_directories(${GTEST_INCLUDE})

# add library directories
link_directories(${TBB_LIBS})
link_directories(${FFMPEG_LIBS})
link_directories(${ZEROMQ_LIBS})
link_directories(${GTEST_LIBS})

add_executable(${PROJECT_NAME} ${TARGET_SRC} ${TARGET_H} ${CMAKE_CURRENT_BINARY_DIR}/version.rc)

target_link_libraries(${PROJECT_NAME} ${TBB_LINK})
target_link_libraries(${PROJECT_NAME} ${FFMPEG_LINK})
target_link_libraries(${PROJECT_NAME} ${ZEROMQ_LINK})
target_link_libraries(${PROJECT_NAME} ${GTEST_LINK})
target_link_libraries(${PROJECT_NAME} capture)
target_link_libraries(${PROJECT_NAME} common)

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

if(NOT MSVC)
	target_link_libraries(${PROJECT_NAME} pthread)
endif()

if(MSVC)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	   COMMAND ${CMAKE_COMMAND} -E copy_if_different
		   "${GTEST_BINS}/gtest.dll"
	$<TARGET_FILE_DIR:${PROJECT_NAME}>)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	   COMMAND ${CMAKE_COMMAND} -E copy_if_different
		   "${GTEST_BINS}/gtest-d.dll"
	$<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif()