cmake_minimum_required(VERSION 2.6)

include_directories(.)
if(NOT BOOST_PATH)
	set(BOOST_PATH ${CMAKE_CURRENT_SOURCE_PATH})
endif()
include_directories(${BOOST_PATH})

enable_testing()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(BOOST_NOWIDE_TESTS
    test_convert
    test_env
    test_stdio
    test_fstream
    test_iostream
    )

foreach(TEST ${BOOST_NOWIDE_TESTS})
    add_executable(${TEST} libs/nowide/test/${TEST}.cpp)
    add_test(${TEST} ${TEST})
endforeach()

add_library(boost_nowide SHARED libs/nowide/src/iostream.cpp)
set_target_properties(boost_nowide PROPERTIES COMPILE_DEFINITIONS BOOST_NOWIDE_DYN_LINK)
set_target_properties(test_iostream PROPERTIES COMPILE_DEFINITIONS BOOST_NOWIDE_DYN_LINK)
target_link_libraries(test_iostream boost_nowide)

add_executable(test_system libs/nowide/test/test_system.cpp)
add_test(test_system_n test_system "-n")
add_test(test_system_w test_system "-w")
