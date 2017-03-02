# Project Name
PROJECT(tests)

set(tests 
		test_parser
		test_parser2
	)

if (NOT MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
endif()

enable_testing()


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

foreach(test ${tests})

add_definitions(-DGTEST_LINKED_AS_SHARED_LIBRARY)

	add_executable(${test} ${test}.cpp)
	add_test(NAME ${test} COMMAND ${test})

	target_link_libraries(${test} ${TBB_LINK})
	target_link_libraries(${test} ${FFMPEG_LINK})
	target_link_libraries(${test} ${ZEROMQ_LINK})
	target_link_libraries(${test} ${GTEST_LINK})
	target_link_libraries(${test} capture)
	target_link_libraries(${test} common)

	if (MSVC)
		add_custom_command(TARGET ${test} POST_BUILD
		   COMMAND ${CMAKE_COMMAND} -E copy_if_different
			   "${GTEST_BINS}/gtest.dll"
		$<TARGET_FILE_DIR:${test}>)
		add_custom_command(TARGET ${test} POST_BUILD
		   COMMAND ${CMAKE_COMMAND} -E copy_if_different
			   "${GTEST_BINS}/gtest-d.dll"
		$<TARGET_FILE_DIR:${test}>)
	endif()
	
	# set_target_properties(${test} PROPERTIES FOLDER "Tests")
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