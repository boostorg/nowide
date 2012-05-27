cmake_minimum_required(VERSION 2.6)

include_directories(.)
include_directories(/home/artik/Packages/boost/boost_1_49_0)

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

if(WIN32)
    add_library(boost_nowide SHARED libs/nowide/src/iostream.cpp)
    set_target_properties(boost_nowide PROPERTIES COMPILE_DEFINITIONS BOOST_NOWIDE_DYN_LINK)
    set_target_properties(test_iostream PROPERTIES COMPILE_DEFINITIONS BOOST_NOWIDE_DYN_LINK)
    target_link_libraries(test_iostream boost_nowide)
endif()

add_executable(test_system libs/nowide/test/test_system.cpp)
add_test(test_system_1 test_system "-1")
add_test(test_system_2 test_system "-2")
add_test(test_system_3 test_system "-3")
add_test(test_system_w test_system "-2")
