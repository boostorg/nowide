#.rst:
# FindOpenCppCoverage
# -----------
#
# Find OpenCppCoverage and define functions for using it
#
# Usage
# ^^^^^^^^^^^^^^^^
#
# 1. Call `OpenCppCoverage_add_target(myTarget)` for each target that should be run to generate coverage
# 2. At the end call `OpenCppCoverage_add_merge_target(target outputFile)` to create a target which will create the combined coverage report
#
# Functions defined
# ^^^^^^^^^^^^^^^^
# OpenCppCoverage_add_target(target WORKING_DIRECTORY dir SOURCES [src, ...] MODULES [module, ...] OCC_ARGS [arg, ...] ARGS [arg, ...])
#   Register `target` to be run with OpenCppCoverage.
#   WORKING_DIRECTORY -- Working directory in which to execute, defaults to the current binary dir
#   SOURCES           -- RegExps to specify source files to include, defaults to PROJECT_SOURCE_DIR
#   MODULES           -- RegExps to specify modules (binaries) to include, defaults to PROJECT_BINARY_DIR
#   OCC_ARGS          -- Extra arguments to pass to OpenCppCoverage
#   ARGS              -- Arguments to pass to the executable
#
# OpenCppCoverage_add_merge_target(target outputFile [FORMAT <format>])
#  Create `target` which merges all output from the previous `OpenCppCoverage_add_target` calls into `outputFile`.
#  A relative `outputFile` will be treated relative to the current binary dir.
#  FORMAT -- Output format (e.g. "html"). Defaults to "cobertura"

if(NOT MSVC)
  message(AUTHOR_WARNING "OpenCppCoverage works only on MSVC")
endif()

find_program(OpenCppCoverage_BINARY OpenCppCoverage.exe)

if(OpenCppCoverage_BINARY)
  execute_process(COMMAND "${OpenCppCoverage_BINARY}" --help ERROR_VARIABLE _out OUTPUT_QUIET)
  if(_out MATCHES "OpenCppCoverage Version: ([.0-9]+)")
    set(OpenCppCoverage_VERSION ${CMAKE_MATCH_1})
  endif()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(OpenCppCoverage
  REQUIRED_VARS OpenCppCoverage_BINARY
  VERSION_VAR OpenCppCoverage_VERSION
)

if(OpenCppCoverage_FOUND)
  function(OpenCppCoverage_add_target target)
    cmake_parse_arguments(PARSE_ARGV 1 ARG "" "WORKING_DIRECTORY" "SOURCES;MODULES;OCC_ARGS;ARGS")
    if(ARG_UNPARSED_ARGUMENTS)
      message(FATAL_ERROR "Invalid argument(s): ${ARG_UNPARSED_ARGUMENTS}")
    endif()

    get_property(counter GLOBAL PROPERTY OpenCppCoverage_COUNTER)
    if(NOT counter)
      set(counter 1)
    else()
      math(EXPR counter "${counter} + 1")
    endif()
    set(outputFile ${CMAKE_CURRENT_BINARY_DIR}/OpenCppCoverage/cov-${counter}-${target}.bin)
    set_property(GLOBAL PROPERTY OpenCppCoverage_COUNTER "${counter}")
    set_property(GLOBAL APPEND PROPERTY OpenCppCoverage_SOURCES "${outputFile}")

    if(NOT ARG_WORKING_DIRECTORY)
      set(ARG_WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    endif()
    if(NOT ARG_SOURCES)
      file(TO_NATIVE_PATH ${PROJECT_SOURCE_DIR} ARG_SOURCES)
    endif()
    if(NOT ARG_MODULES)
      file(TO_NATIVE_PATH ${PROJECT_BINARY_DIR} ARG_MODULES)
    endif()

    set(args "${ARG_OCC_ARGS}")
    foreach(el IN LISTS ARG_SOURCES)
        list(APPEND args --sources ${el})
    endforeach()
    foreach(el IN LISTS ARG_MODULES)
        list(APPEND args --modules ${el})
    endforeach()
    add_custom_command(OUTPUT ${outputFile}
      DEPENDS ${target}
      COMMENT "Creating coverage for ${target}"
      COMMAND ${OpenCppCoverage_BINARY}
        --working_dir ${ARG_WORKING_DIRECTORY}
        --export_type binary:${outputFile}
        --cover_children
        --quiet
        ${args}
        -- $<TARGET_FILE:${target}> ${ARG_ARGS}
      VERBATIM
    )
  endfunction()

  function(OpenCppCoverage_add_merge_target target outputFile)
    cmake_parse_arguments(PARSE_ARGV 2 ARG "" "FORMAT" "")
    if(ARG_UNPARSED_ARGUMENTS)
      message(FATAL_ERROR "Invalid argument(s): ${ARG_UNPARSED_ARGUMENTS}")
    endif()

    if(NOT ARG_FORMAT)
      set(ARG_FORMAT cobertura)
    endif()

    get_property(sources GLOBAL PROPERTY OpenCppCoverage_SOURCES)
    set_property(GLOBAL PROPERTY OpenCppCoverage_SOURCES "")
    get_filename_component(outputFile ${outputFile} ABSOLUTE BASE_DIR ${CMAKE_CURRENT_BINARY_DIR})
    set(sourceArg "")
    foreach(source IN LISTS sources)
      list(APPEND sourceArg --input_coverage=${source})
    endforeach()
    add_custom_command(OUTPUT ${outputFile}
      DEPENDS ${sources}
      COMMENT "Merging coverage data"
      COMMAND ${OpenCppCoverage_BINARY} --export_type ${ARG_FORMAT}:${outputFile} ${sourceArg}
      VERBATIM
    )
    add_custom_target(${target} DEPENDS ${outputFile})
  endfunction()
endif()
