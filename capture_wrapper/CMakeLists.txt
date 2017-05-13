# Project Name
project(capture_wrapper)

set(VERSION_MAJOR "1" CACHE STRING "Major")
set(VERSION_MINOR "0" CACHE STRING "Minor")
set(VERSION_TYPE "0" CACHE STRING "Type")
set(VERSION_BUILD "0" CACHE STRING "Build")

message(STATUS "Version: " ${VERSION_MAJOR} "." ${VERSION_MINOR} "." ${VERSION_TYPE} "." ${VERSION_BUILD})

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/version.rc.in
  ${CMAKE_CURRENT_BINARY_DIR}/version.rc
  @ONLY)

  message(STATUS ${CMAKE_CURRENT_SOURCE_DIR})
  
#remove old files if any exist
file(GLOB ALL_JAVA_FILES "${CMAKE_CURRENT_SOURCE_DIR}/java/src/*.java")
file(GLOB ALL_CLASS_FILES "${CMAKE_CURRENT_SOURCE_DIR}/java/bin/*.class")
file(GLOB ALL_JAR_FILES "${CMAKE_CURRENT_SOURCE_DIR}/java/lib/*.jar")

foreach(f ${ALL_JAVA_FILES})
	message(STATUS "deleting ${f}")
	file(REMOVE ${f})
endforeach()
foreach(f ${ALL_CLASS_FILES})
	message(STATUS "deleting ${f}")
	file(REMOVE ${f})
endforeach()
foreach(f ${ALL_JAR_FILES})
	message(STATUS "deleting ${f}")
	file(REMOVE ${f})
endforeach()
  
execute_process(COMMAND ${SWIG_EXECUTABLE} -c++ -java -outdir "${CMAKE_CURRENT_SOURCE_DIR}/java/src" -package tr.tmg.murat.guvenc "${CMAKE_CURRENT_SOURCE_DIR}/include/webcam.i")
FILE(RENAME "${CMAKE_CURRENT_SOURCE_DIR}/include/webcam_wrap.cxx" "${CMAKE_CURRENT_SOURCE_DIR}/src/webcam_wrap.cxx")
  
FILE(GLOB TARGET_H 							"include/*.h*" "src/*.h*")
FILE(GLOB TARGET_SRC		 				"src/*.c*")

if (NOT MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
endif()

include_directories(include/)
include_directories("../common/include/")
include_directories("../capture/include/")
include_directories(${FFMPEG_INCLUDE})
include_directories(${TBB_INCLUDE})

include_directories($ENV{JAVA_HOME}/include)
include_directories($ENV{JAVA_HOME}/include/win32)

link_directories(${TBB_LIBS})
link_directories(${FFMPEG_LIBS})

add_library(${PROJECT_NAME} SHARED ${TARGET_SRC} ${TARGET_H} ${CMAKE_CURRENT_BINARY_DIR}/version.rc)

target_link_libraries(${PROJECT_NAME} capture)
target_link_libraries(${PROJECT_NAME} ${TBB_LINK})
# target_link_libraries(${PROJECT_NAME} ${CMAKE_BINARY_DIR}/capture/$<CONFIGURATION>/capture.lib)

if(NOT MSVC)
	target_link_libraries(${PROJECT_NAME} pthread)
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
					   COMMAND ${Java_JAVAC_EXECUTABLE} -d "${CMAKE_CURRENT_SOURCE_DIR}/java/bin/" -cp "${CMAKE_CURRENT_SOURCE_DIR}/java/bin/" "${CMAKE_CURRENT_SOURCE_DIR}/java/src/*.java")
					   
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
					   COMMAND ${Java_JAR_EXECUTABLE} cvf "${CMAKE_CURRENT_SOURCE_DIR}/java/lib/${PROJECT_NAME}.jar" -C "${CMAKE_CURRENT_SOURCE_DIR}/java/bin/" .)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Utility")