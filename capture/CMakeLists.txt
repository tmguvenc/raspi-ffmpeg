cmake_minimum_required(VERSION 2.8)

project(capture)

set(VERSION_MAJOR "1" CACHE STRING "Major")
set(VERSION_MINOR "0" CACHE STRING "Minor")
set(VERSION_TYPE "0" CACHE STRING "Type")
set(VERSION_BUILD "0" CACHE STRING "Build")

set(INSTALL_PREFIX "/usr/local/" CACHE PATH "Installation and Dependency Path")

message(STATUS "Version: " ${VERSION_MAJOR} "." ${VERSION_MINOR} "." ${VERSION_TYPE} "." ${VERSION_BUILD})

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/version.rc.in
  ${CMAKE_CURRENT_BINARY_DIR}/version.rc
  @ONLY)
  
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(FFMPEG_INCLUDE ${INSTALL_PREFIX}/include)
set(FFMPEG_LIBS ${INSTALL_PREFIX}/lib)
set(FFMPEG_LINK avcodec avformat avutil swscale avdevice)

FILE(GLOB TARGET_H 	 "include/*.h*" "src/*.h*")
FILE(GLOB TARGET_SRC "src/*.c*")

include_directories(include/)
include_directories(${FFMPEG_INCLUDE})

link_directories(${FFMPEG_LIBS})
add_library(${PROJECT_NAME} SHARED ${TARGET_SRC} ${TARGET_H} ${CMAKE_CURRENT_BINARY_DIR}/version.rc)
target_link_libraries(${PROJECT_NAME} ${FFMPEG_LINK})
target_link_libraries(${PROJECT_NAME} pthread)

enable_testing () # Enable testing only works in root scope
add_subdirectory(tests)
  
set (PUBLIC_H 	"include/capture.h"
				"include/VAFrameContainer.h"
				"include/CaptureSettings.h"
				"include/Decoder.h"
				"include/ICapture.h"
				"include/ICaptureFactory.h"
				"include/RTSPCaptureFactory.h"
				"include/VideoFileCaptureFactory.h"
				"include/WebcamCaptureFactory.h"
						)

install(FILES ${PUBLIC_H} DESTINATION include/capture)
install(TARGETS ${PROJECT_NAME} 
  EXPORT ${PROJECT_NAME}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)