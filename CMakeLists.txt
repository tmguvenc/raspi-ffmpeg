cmake_minimum_required(VERSION 3.1)

project(rvms)

if (MSVC)
	set(DEPENDS_FOLDER "D:/External" CACHE PATH "Dependency Path")

	# caffe
	set(BOOST_INCLUDE ${DEPENDS_FOLDER}/boost_1_61_0)
	set(BOOST_LIBS ${DEPENDS_FOLDER}/boost_1_61_0/lib64-msvc-12.0)
	set(GFLAGS_INCLUDE ${DEPENDS_FOLDER}/gflags/include)
	set(GLOG_INCLUDE ${DEPENDS_FOLDER}/google-glog/include)
	set(OPENBLAS_INCLUDE ${DEPENDS_FOLDER}/xianyi-OpenBLAS-3f6398a)
	set(CAFFE_INCLUDE ${DEPENDS_FOLDER}/caffe/include)
	set(CAFFE_LIBS ${DEPENDS_FOLDER}/caffe/lib)
	set(CAFFE_BINS ${DEPENDS_FOLDER}/caffe/bin)

	set(CAFFE_LINK_DEBUG debug "caffe-d" debug "proto-d" debug "libboost_system-vc120-mt-gd-1_61" debug "libboost_thread-vc120-mt-gd-1_61" debug "libboost_filesystem-vc120-mt-gd-1_61" debug "libboost_chrono-vc120-mt-gd-1_61" debug "libboost_date_time-vc120-mt-gd-1_61")
	set(CAFFE_LINK_RELEASE optimized "caffe" optimized "proto" optimized "libboost_system-vc120-mt-1_61" optimized "libboost_thread-vc120-mt-1_61" optimized "libboost_filesystem-vc120-mt-1_61" optimized "libboost_chrono-vc120-mt-1_61" optimized "libboost_date_time-vc120-mt-1_61")
	# set(CAFFE_LINK_DEBUG caffe-d proto-d)
	# set(CAFFE_LINK_RELEASE caffe proto)

	# zeromq
	set(ZEROMQ_INCLUDE ${DEPENDS_FOLDER}/zeromq_secure/x64/include)
	set(ZEROMQ_LIBS ${DEPENDS_FOLDER}/zeromq_secure/x64/lib)
	set(ZEROMQ_BINS ${DEPENDS_FOLDER}/zeromq_secure/x64/bin)
	set(ZEROMQ_LINK libzmq)
	set(ZEROMQ_DLL "${ZEROMQ_BINS}/libzmq.dll")
	set(ZEROMQ_DLL_D "${ZEROMQ_BINS}/libsodium.dll")

	# sodium
	set(SODIUM_INCLUDE ${DEPENDS_FOLDER}/libsodium/x64/include)
	set(SODIUM_LIBS ${DEPENDS_FOLDER}/libsodium/x64/staticlib)
	set(SODIUM_BINS ${DEPENDS_FOLDER}/libsodium/x64/bin)
	set(SODIUM_LINK optimized libsodium-x64-static debug libsodium-x64-static-d)

	# opencv
	set(OPENCV_INCLUDE ${DEPENDS_FOLDER}/opencv/build/include)
	set(OPENCV_LIBS ${DEPENDS_FOLDER}/opencv/build/x64/vc12/lib)	
	set(OPENCV_BINS ${DEPENDS_FOLDER}/opencv/build/x64/vc12/bin)	
	set(OPENCV_LINK_DEBUG debug "opencv_calib3d248d" debug "opencv_contrib248d" debug "opencv_core248d" debug "opencv_features2d248d" debug "opencv_flann248d" debug "opencv_gpu248d" debug "opencv_highgui248d" debug "opencv_imgproc248d" debug "opencv_legacy248d" debug "opencv_ml248d" debug "opencv_nonfree248d" debug "opencv_objdetect248d" debug "opencv_photo248d" debug "opencv_stitching248d" debug "opencv_superres248d" debug "opencv_ts248d" debug "opencv_ts248d" debug "opencv_video248d" debug "opencv_videostab248d")
	set(OPENCV_LINK_RELEASE optimized "opencv_calib3d248" optimized "opencv_contrib248" optimized "opencv_core248" optimized "opencv_features2d248" optimized "opencv_flann248" optimized "opencv_gpu248" optimized "opencv_highgui248" optimized "opencv_imgproc248" optimized "opencv_legacy248" optimized "opencv_ml248" optimized "opencv_nonfree248" optimized "opencv_objdetect248" optimized "opencv_photo248" optimized "opencv_stitching248" optimized "opencv_superres248" optimized "opencv_ts248" optimized "opencv_ts248" optimized "opencv_video248" optimized "opencv_videostab248")

	# tbb
	set(TBB_INCLUDE ${DEPENDS_FOLDER}/tbb/include)
	set(TBB_LIBS ${DEPENDS_FOLDER}/tbb/lib/intel64/vc12)
	set(TBB_BINS ${DEPENDS_FOLDER}/tbb/bin/intel64/vc12)
	set(TBB_LINK optimized "tbb" debug "tbb_debug")

	# protocol buffers
	set(PROTOBUF_INCLUDE ${DEPENDS_FOLDER}/protobuf/include)
	set(PROTOBUF_LIBS ${DEPENDS_FOLDER}/protobuf/lib/x64/Md)
	set(PROTOBUF_BIN ${DEPENDS_FOLDER}/protobuf/bin)
	set(PROTOBUF_LINK optimized "libprotobuf" debug "libprotobuf-d")
	
	#spdlog
	set(SPDLOG_INCLUDE ${DEPENDS_FOLDER}/spdlog/include)

	#ffmpeg
	set(FFMPEG_INCLUDE ${DEPENDS_FOLDER}/ffmpeg/x64/include)
	set(FFMPEG_LIBS ${DEPENDS_FOLDER}/ffmpeg/x64/lib)
	set(FFMPEG_BINS ${DEPENDS_FOLDER}/ffmpeg/x64/bin)
	set(FFMPEG_LINK avcodec avdevice avfilter avformat avutil postproc swresample swscale)

	#google-test
	set(GTEST_INCLUDE ${DEPENDS_FOLDER}/googletest/googletest-distribution/include)
	set(GTEST_LIBS ${DEPENDS_FOLDER}/googletest/googletest-distribution/lib)
	set(GTEST_BINS ${DEPENDS_FOLDER}/googletest/googletest-distribution/lib)
	set(GTEST_LINK debug "gtest-d" optimized "gtest")
else()
	set(LOCAL /usr/local)

	set(ZEROMQ_INCLUDE ${LOCAL}/include)
	set(ZEROMQ_LIBS ${LOCAL}/lib)
	set(ZEROMQ_BINS ${LOCAL}/lib)
	set(ZEROMQ_LINK zmq)

	# sodium
	set(SODIUM_INCLUDE ${LOCAL}/include)
	set(SODIUM_LIBS ${LOCAL}/lib)
	set(SODIUM_BINS ${LOCAL}/lib)
	set(SODIUM_LINK sodium)

	# opencv
	set(OPENCV_INCLUDE ${LOCAL}/include)
	set(OPENCV_LIBS ${LOCAL}/lib)	
	set(OPENCV_BINS ${LOCAL}/lib)	
	set(OPENCV_LINK opencv_calib3d opencv_contrib opencv_core opencv_features2d opencv_flann opencv_gpu opencv_highgui opencv_imgproc opencv_legacy opencv_ml opencv_nonfree opencv_objdetect opencv_photo opencv_stitching opencv_superres opencv_ts opencv_ts opencv_video opencv_videostab)

	# tbb
	set(TBB_INCLUDE /usr/include)
	set(TBB_LIBS /usr/lib)
	set(TBB_BINS /usr/lib)
	set(TBB_LINK tbb)

	# protocol buffers
	set(PROTOBUF_INCLUDE ${LOCAL}/include)
	set(PROTOBUF_LIBS ${LOCAL}/lib)
	set(PROTOBUF_BIN ${LOCAL}/bin)
	set(PROTOBUF_LINK protobuf)

	#ffmpeg
	set(FFMPEG_INCLUDE ${LOCAL}/include)
	set(FFMPEG_LIBS ${LOCAL}/lib)
	set(FFMPEG_LINK avcodec avdevice avfilter avformat avutil postproc swresample swscale)
	
	#google test
	set(GTEST_INCLUDE ${LOCAL}/include)
	set(GTEST_LIBS ${LOCAL}/lib)
	set(GTEST_BINS ${LOCAL}/lib)
	set(GTEST_LINK gtest)
endif()

enable_testing ()

add_subdirectory (capture)
add_subdirectory (rvms-server)
add_subdirectory (common)
add_subdirectory (communication)
add_subdirectory (test)
if (MSVC)
	add_subdirectory (rvms-api-net)	
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)