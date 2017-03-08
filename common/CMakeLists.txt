# Project Name
project(common)

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

include_directories(include/)
include_directories(${TBB_INCLUDE})
include_directories(${FFMPEG_INCLUDE})
include_directories(${SPDLOG_INCLUDE})
include_directories(${ZEROMQ_INCLUDE})

# add library directories
link_directories(${TBB_LIBS})
link_directories(${FFMPEG_LIBS})
link_directories(${ZEROMQ_LIBS})

add_library(${PROJECT_NAME} STATIC ${TARGET_SRC} ${TARGET_H} ${CMAKE_CURRENT_BINARY_DIR}/version.rc)

target_link_libraries(${PROJECT_NAME} ${TBB_LINK})
target_link_libraries(${PROJECT_NAME} ${FFMPEG_LINK})
target_link_libraries(${PROJECT_NAME} ${ZEROMQ_LINK})

if(NOT MSVC)
	target_link_libraries(${PROJECT_NAME} pthread)
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Utility")