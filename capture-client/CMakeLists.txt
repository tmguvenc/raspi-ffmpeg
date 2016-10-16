cmake_minimum_required(VERSION 2.8.10)

# Project Name
project(raspi-client)

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

set(ZEROMQ_PATH "D:/External/zeromq_secure" CACHE PATH "Zeromq Path")
set(PROVIDEO_DEPENDS_FOLDER "D:/External" CACHE PATH "Dependency Folder")

if(64_BIT)
	set(ZEROMQ_INCLUDE ${ZEROMQ_PATH}/x64/include)
	set(ZEROMQ_LIBS ${ZEROMQ_PATH}/x64/lib)
	set(ZEROMQ_BINS ${ZEROMQ_PATH}/x64/bin)
else()
	set(ZEROMQ_INCLUDE ${ZEROMQ_PATH}/x86/include)
	set(ZEROMQ_LIBS ${ZEROMQ_PATH}/x86/lib)
	set(ZEROMQ_BINS ${ZEROMQ_PATH}/x86/bin)
endif()

# tbb
set(TBB_INCLUDE ${PROVIDEO_DEPENDS_FOLDER}/tbb/include)
if(64_BIT)
	set(TBB_LIBS ${PROVIDEO_DEPENDS_FOLDER}/tbb/lib/intel64/vc12)
	set(TBB_BINS ${PROVIDEO_DEPENDS_FOLDER}/tbb/bin/intel64/vc12)
else()
	set(TBB_LIBS ${PROVIDEO_DEPENDS_FOLDER}/tbb/lib/ia32/vc12)
	set(TBB_BINS ${PROVIDEO_DEPENDS_FOLDER}/tbb/bin/ia32/vc12)
endif()
set(TBB_LINK optimized "tbb" debug "tbb_debug")

# opencv
if(64_BIT)
	set(OPENCV_INCLUDE ${PROVIDEO_DEPENDS_FOLDER}/opencv/build/include)
	set(OPENCV_LIBS ${PROVIDEO_DEPENDS_FOLDER}/opencv/build/x64/vc12/lib)	
	set(OPENCV_BINS ${PROVIDEO_DEPENDS_FOLDER}/opencv/build/x64/vc12/bin)	
else()
	set(OPENCV_INCLUDE ${PROVIDEO_DEPENDS_FOLDER}/opencv/build/include)
	set(OPENCV_LIBS ${PROVIDEO_DEPENDS_FOLDER}/opencv/build/x86/vc12/lib)	
	set(OPENCV_BINS ${PROVIDEO_DEPENDS_FOLDER}/opencv/build/x86/vc12/bin)	
endif()

# ffmpeg
if(64_BIT)
	set(FFMPEG_INCLUDE ${PROVIDEO_DEPENDS_FOLDER}/ffmpeg/x64/include)
	set(FFMPEG_LIBS ${PROVIDEO_DEPENDS_FOLDER}/ffmpeg/x64/lib)
else()
	set(FFMPEG_INCLUDE ${PROVIDEO_DEPENDS_FOLDER}/ffmpeg/x86/include)
	set(FFMPEG_LIBS ${PROVIDEO_DEPENDS_FOLDER}/ffmpeg/x86/lib)
endif()
set(FFMPEG_LINK avcodec avformat avutil swscale avdevice)

set(OPENCV_LINK_DEBUG debug "opencv_calib3d248d" debug "opencv_contrib248d" debug "opencv_core248d" debug "opencv_features2d248d" debug "opencv_flann248d" debug "opencv_gpu248d" debug "opencv_highgui248d" debug "opencv_imgproc248d" debug "opencv_legacy248d" debug "opencv_ml248d" debug "opencv_nonfree248d" debug "opencv_objdetect248d" debug "opencv_photo248d" debug "opencv_stitching248d" debug "opencv_superres248d" debug "opencv_ts248d" debug "opencv_ts248d" debug "opencv_video248d" debug "opencv_videostab248d")
set(OPENCV_LINK_RELEASE optimized "opencv_calib3d248" optimized "opencv_contrib248" optimized "opencv_core248" optimized "opencv_features2d248" optimized "opencv_flann248" optimized "opencv_gpu248" optimized "opencv_highgui248" optimized "opencv_imgproc248" optimized "opencv_legacy248" optimized "opencv_ml248" optimized "opencv_nonfree248" optimized "opencv_objdetect248" optimized "opencv_photo248" optimized "opencv_stitching248" optimized "opencv_superres248" optimized "opencv_ts248" optimized "opencv_ts248" optimized "opencv_video248" optimized "opencv_videostab248")

set(ZEROMQ_LINK libzmq)
set(ZEROMQ_DLL "${ZEROMQ_BINS}/libzmq.dll")
set(ZEROMQ_DLL_D "${ZEROMQ_BINS}/libsodium.dll")

FILE(GLOB TARGET_H 			"src/*.h*" "include/*.h*")
FILE(GLOB TARGET_SRC		"src/*.c*")

include_directories("../libarrs/include")
include_directories(${TBB_INCLUDE})
include_directories(${FFMPEG_INCLUDE})
include_directories(${ZEROMQ_INCLUDE})
include_directories(${OPENCV_INCLUDE})
link_directories(${TBB_LIBS})
link_directories(${ZEROMQ_LIBS})
link_directories(${OPENCV_LIBS})
link_directories(${FFMPEG_LIBS})

add_executable(${PROJECT_NAME} ${TARGET_SRC} ${TARGET_H})

target_link_libraries(${PROJECT_NAME} ${TBB_LINK})
target_link_libraries(${PROJECT_NAME} ${FFMPEG_LINK})
target_link_libraries(${PROJECT_NAME} ${ZEROMQ_LINK})
target_link_libraries(${PROJECT_NAME} ${OPENCV_LINK_DEBUG} ${OPENCV_LINK_RELEASE})
target_link_libraries(${PROJECT_NAME} ${DLL_FILE_NAME})
		
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
			"${OPENCV_BINS}/opencv_video248d.dll"
			$<TARGET_FILE_DIR:${PROJECT_NAME}>)		
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${OPENCV_BINS}/opencv_objdetect248d.dll"
			$<TARGET_FILE_DIR:${PROJECT_NAME}>)			
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${OPENCV_BINS}/opencv_imgproc248d.dll"
			$<TARGET_FILE_DIR:${PROJECT_NAME}>)			
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${OPENCV_BINS}/opencv_highgui248d.dll"
			$<TARGET_FILE_DIR:${PROJECT_NAME}>)			
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${OPENCV_BINS}/opencv_core248d.dll"
			$<TARGET_FILE_DIR:${PROJECT_NAME}>)		
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${OPENCV_BINS}/opencv_ml248d.dll"
			$<TARGET_FILE_DIR:${PROJECT_NAME}>)
			
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${OPENCV_BINS}/opencv_video248.dll"
			$<TARGET_FILE_DIR:${PROJECT_NAME}>)		
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${OPENCV_BINS}/opencv_objdetect248.dll"
			$<TARGET_FILE_DIR:${PROJECT_NAME}>)			
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${OPENCV_BINS}/opencv_imgproc248.dll"
			$<TARGET_FILE_DIR:${PROJECT_NAME}>)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${OPENCV_BINS}/opencv_highgui248.dll"
			$<TARGET_FILE_DIR:${PROJECT_NAME}>)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${OPENCV_BINS}/opencv_core248.dll"
			$<TARGET_FILE_DIR:${PROJECT_NAME}>)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${OPENCV_BINS}/opencv_ml248.dll"
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
