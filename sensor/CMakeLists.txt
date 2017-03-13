# Project Name
project(sensor)

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
add_definitions(-DSENSOR_EXPORTS)

if (NOT MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
endif()

include_directories(include/)
include_directories(${SPDLOG_INCLUDE})

# add library directories

add_library(${PROJECT_NAME} SHARED ${TARGET_SRC} ${TARGET_H} ${CMAKE_CURRENT_BINARY_DIR}/version.rc)

if (NOT MSVC)
	target_link_libraries(${PROJECT_NAME} wiringPi)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Utility")