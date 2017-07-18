cmake_minimum_required(VERSION 2.6)

option(RUN_WITH_WINE		"Use wine to run tests" OFF)
option(BUILD_SHARED         "Build shared library"  ON)
option(BUILD_STATIC         "Build static library"  ON)
option(BUILD_TESTS          "Build tests"           ON)

if (NOT BUILD_SHARED AND NOT BUILD_STATIC)
    message(FATAL_ERROR "Specify at least one library to build")
endif()

include_directories(.)

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

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_FLAGS}")

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	if(MSVC)
		set(NOWIDE_SUFFIX "-d")
	endif()
endif()

if(BUILD_SHARED)
    add_library(nowide SHARED src/iostream.cpp)
    set_target_properties(nowide	PROPERTIES VERSION 0.0.0 SOVERSION 0)
    set_target_properties(nowide 	PROPERTIES
    				CLEAN_DIRECT_OUTPUT 1
    				OUTPUT_NAME "nowide${NOWIDE_SUFFIX}")
    set_target_properties(nowide PROPERTIES COMPILE_DEFINITIONS DLL_EXPORT)
    list(APPEND INSTALL_TARGETS nowide)
endif()

if(BUILD_STATIC)
    add_library(nowide-static STATIC src/iostream.cpp)
    set_target_properties(nowide-static PROPERTIES
    				CLEAN_DIRECT_OUTPUT 1
    				OUTPUT_NAME "nowide${NOWIDE_SUFFIX}")
    if(MSVC)
    	set_target_properties(nowide-static PROPERTIES PREFIX "lib")
    endif()

	list(APPEND INSTALL_TARGETS nowide-static)
endif()

install(TARGETS ${INSTALL_TARGETS}
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION ${LIBDIR}
	ARCHIVE DESTINATION ${LIBDIR})
install(DIRECTORY nowide DESTINATION include)

if(BUILD_TESTS)
    enable_testing()

    set(NOWIDE_TESTS
        test_convert
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
    
    if(BUILD_SHARED)
        add_executable(test_iostream_shared test/test_iostream.cpp)
        set_target_properties(test_iostream_shared PROPERTIES COMPILE_DEFINITIONS DLL_EXPORT)
        target_link_libraries(test_iostream_shared nowide)
    endif()
    
    if(BUILD_STATIC)
        add_executable(test_iostream_static test/test_iostream.cpp)
        target_link_libraries(test_iostream_static nowide-static)
    endif()
    
    add_executable(test_system test/test_system.cpp)
    
    add_executable(test_env_proto test/test_env.cpp)
    add_executable(test_env_win test/test_env.cpp)
    set_target_properties(test_env_win PROPERTIES COMPILE_DEFINITIONS NOWIDE_TEST_INCLUDE_WINDOWS)
    
    set(OTHER_TESTS test_iostream_shared test_iostream_static test_env_win test_env_proto)
    
    if(RUN_WITH_WINE)
    	foreach(T ${OTHER_TESTS})
    		add_test(NAME ${T} WORKING_DIRECTORY ${CMAKE_BUILD_DIR} COMMAND wine ./${T}.exe)
    	endforeach()
    
    	add_test(NAME test_system_n WORKING_DIRECTORY ${CMAKE_BUILD_DIR} COMMAND wine ./test_system.exe "-n")
    	add_test(NAME test_system_w WORKING_DIRECTORY ${CMAKE_BUILD_DIR} COMMAND wine ./test_system.exe "-w")
    else()
    	foreach(T ${OTHER_TESTS})
    		add_test(${T} ${T})
    	endforeach()
    
    	add_test(test_system_n test_system "-n")
    	add_test(test_system_w test_system "-w")
    endif()

endif()

