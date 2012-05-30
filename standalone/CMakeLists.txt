cmake_minimum_required(VERSION 2.6)

include_directories(.)
enable_testing()

option(RUN_WITH_WINE		"Use wine to run tests" OFF)


if(NOT LIBDIR)
	set(LIBDIR lib CACHE STRING "Library installation directory" FORCE)
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
	set(CXX_FLAGS "-Wall -Wextra")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	set(CXX_FLAGS "-Wall -Wextra")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
	set(CXX_FLAGS "-Wall")
elseif(MSVC)
	set(CXX_FLAGS "/EHsc /W3")
endif()

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	if(MSVC)
		set(NOWIDE_SUFFIX "-d")
	endif()
endif()



set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_FLAGS}")

set(NOWIDE_TESTS
    test_convert
    test_env
    test_stdio
    test_fstream
    )

foreach(TEST ${NOWIDE_TESTS})
    add_executable(${TEST} test/${TEST}.cpp)
    if(RUN_WITH_WINE)
       add_test(NAME ${TEST} WORKING_DIRECTORY ${CMAKE_BUILD_DIR} COMMAND wine ./${TEST}.exe)
    else()
       add_test(${TEST} ${TEST})
    endif()
endforeach()

add_library(nowide SHARED src/iostream.cpp)
set_target_properties(nowide	PROPERTIES VERSION 0.0.0 SOVERSION 0)
set_target_properties(nowide 	PROPERTIES
				CLEAN_DIRECT_OUTPUT 1
				OUTPUT_NAME "nowide${NOWIDE_SUFFIX}"
			)

add_library(nowide-static STATIC src/iostream.cpp)
set_target_properties(nowide-static PROPERTIES
				CLEAN_DIRECT_OUTPUT 1
				OUTPUT_NAME "nowide${NOWIDE_SUFFIX}"
			)

if(MSVC)
	set_target_properties(nowide-static PROPERTIES PREFIX "lib")
endif()

add_executable(test_iostream_shared test/test_iostream.cpp)
set_target_properties(nowide PROPERTIES COMPILE_DEFINITIONS DLL_EXPORT)
set_target_properties(test_iostream_shared PROPERTIES COMPILE_DEFINITIONS DLL_EXPORT)
target_link_libraries(test_iostream_shared nowide)

add_executable(test_iostream_static test/test_iostream.cpp)
target_link_libraries(test_iostream_static nowide-static)

add_executable(test_system test/test_system.cpp)


if(RUN_WITH_WINE)
	add_test(NAME test_iostream_shared WORKING_DIRECTORY ${CMAKE_BUILD_DIR} COMMAND wine ./test_iostream_shared.exe)
	add_test(NAME test_iostream_static WORKING_DIRECTORY ${CMAKE_BUILD_DIR} COMMAND wine ./test_iostream_static.exe)
	add_test(NAME test_system_n WORKING_DIRECTORY ${CMAKE_BUILD_DIR} COMMAND wine ./test_system.exe "-n")
	add_test(NAME test_system_w WORKING_DIRECTORY ${CMAKE_BUILD_DIR} COMMAND wine ./test_system.exe "-w")
else()
	add_test(test_iostream_shared test_iostream_shared)
	add_test(test_iostream_static test_iostream_static)
	add_test(test_system_n test_system "-n")
	add_test(test_system_w test_system "-w")
endif()

install(TARGETS nowide nowide-static
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION ${LIBDIR}
	ARCHIVE DESTINATION ${LIBDIR})

install(DIRECTORY nowide DESTINATION include)

