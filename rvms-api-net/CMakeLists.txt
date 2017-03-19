cmake_minimum_required(VERSION 2.8.10)

# Project Name
project(rvms-api-net)

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
	message (STATUS "64 bit mode")
	set (64_BIT 1)
else()
	message (STATUS "32 bit mode")
	set (64_BIT 0)	
endif()

set(VERSION_MAJOR "1" CACHE STRING "Major")
set(VERSION_MINOR "0" CACHE STRING "Minor")
set(VERSION_TYPE "0" CACHE STRING "Type")
set(VERSION_BUILD "0" CACHE STRING "Build")

FILE(GLOB TARGET_H 			"src/*.h*" "include/*.h*")
FILE(GLOB TARGET_SRC		"src/*.c*")

include_directories(${TBB_INCLUDE})
include_directories("../capture/include/")
include_directories("../common/include/")
include_directories("../communication/include/")
include_directories(${FFMPEG_INCLUDE})
include_directories(${ZEROMQ_INCLUDE})
link_directories(${TBB_LIBS})
link_directories(${ZEROMQ_LIBS})
link_directories(${FFMPEG_LIBS})

add_library(${PROJECT_NAME} SHARED ${TARGET_SRC} ${TARGET_H})

target_link_libraries(${PROJECT_NAME} ${TBB_LINK})
target_link_libraries(${PROJECT_NAME} ${FFMPEG_LINK})
target_link_libraries(${PROJECT_NAME} ${ZEROMQ_LINK})
target_link_libraries(${PROJECT_NAME} ${DLL_FILE_NAME})
target_link_libraries(${PROJECT_NAME} capture)
target_link_libraries(${PROJECT_NAME} communication)

set(CAPTURE_DLL "${CMAKE_BINARY_DIR}/capture/Debug/capture.dll")
set(COMMON_DLL "${CMAKE_BINARY_DIR}/common/Debug/common.dll")
set(COMMUNICATION_DLL "${CMAKE_BINARY_DIR}/communication/Debug/communication.dll")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /clr")
SET (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
SET (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SAFESEH:NO")
SET (CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /SAFESEH:NO")
STRING(REPLACE "/EHsc" "/EHa" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
STRING(REPLACE "/RTC1" "" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
set_target_properties(${PROJECT_NAME} PROPERTIES VS_DOTNET_REFERENCES
"System;System.Drawing;System.Windows.Forms")
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Utility")

	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	   COMMAND ${CMAKE_COMMAND} -E copy_if_different
		   ${ZEROMQ_DLL}
			$<TARGET_FILE_DIR:${PROJECT_NAME}>)
			
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	   COMMAND ${CMAKE_COMMAND} -E copy_if_different
		   ${ZEROMQ_DLL_D}
			$<TARGET_FILE_DIR:${PROJECT_NAME}>)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${FFMPEG_LIBS}/avcodec-54.dll"
			$<TARGET_FILE_DIR:${PROJECT_NAME}>)		
			
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${FFMPEG_LIBS}/postproc-52.dll"
			$<TARGET_FILE_DIR:${PROJECT_NAME}>)
			
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${FFMPEG_LIBS}/swresample-0.dll"
			$<TARGET_FILE_DIR:${PROJECT_NAME}>)
			
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${FFMPEG_LIBS}/avutil-52.dll"
			$<TARGET_FILE_DIR:${PROJECT_NAME}>)

	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${FFMPEG_LIBS}/avformat-54.dll"
			$<TARGET_FILE_DIR:${PROJECT_NAME}>)
			
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${FFMPEG_LIBS}/avfilter-3.dll"
			$<TARGET_FILE_DIR:${PROJECT_NAME}>)
			
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${FFMPEG_LIBS}/avdevice-54.dll"
			$<TARGET_FILE_DIR:${PROJECT_NAME}>)

	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${FFMPEG_LIBS}/swscale-2.dll"
			$<TARGET_FILE_DIR:${PROJECT_NAME}>)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${TBB_BINS}/tbb.dll"
			$<TARGET_FILE_DIR:${PROJECT_NAME}>)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${TBB_BINS}/tbb_debug.dll"
			$<TARGET_FILE_DIR:${PROJECT_NAME}>)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			${CAPTURE_DLL}
			$<TARGET_FILE_DIR:${PROJECT_NAME}>)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			${COMMON_DLL}
			$<TARGET_FILE_DIR:${PROJECT_NAME}>)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			${COMMUNICATION_DLL}
			$<TARGET_FILE_DIR:${PROJECT_NAME}>)
