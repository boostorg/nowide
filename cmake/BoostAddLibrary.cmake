# Add boost library X as boost_X with alias target Boost::X and useful properties set
# Arguments:
#   SYSTEM: Add include directory as system include
#   LAYOUT: system or versioned to append version, compiler, ...
#   SOURCES: Source files
function(boost_add_library name)
  cmake_parse_arguments(PARSE_ARGV 1 ARG "SYSTEM;INTERFACE" "LAYOUT" "SOURCES")
  if(ARG_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR "Invalid argument(s): ${ARG_UNPARSED_ARGUMENTS}")
  endif()

  set(targetName boost_${name})
  string(TOUPPER "${targetName}" targetNameUpper)

  if(NOT ARG_LAYOUT)
    set(ARG_LAYOUT system)
  endif()
  if(ARG_SYSTEM)
    set(system SYSTEM)
  else()
    set(system "")
  endif()
  if(ARG_INTERFACE)
    if(ARG_SOURCES)
      message(FATAL_ERROR "Cannot add sources to an interface library")
    endif()
    add_library(${targetName} INTERFACE)
    set(scope INTERFACE)
  else()
    add_library(${targetName} ${ARG_SOURCES})
    set(scope PUBLIC)
  endif()

  set(libname ${targetName})
  if(ARG_LAYOUT STREQUAL "versioned")
    string(APPEND libname -$<LOWER_CASE:${CMAKE_CXX_COMPILER_ID}>)
    if(CMAKE_CXX_COMPILER_VERSION MATCHES "^([0-9]+)\.")
      string(APPEND libname ${CMAKE_MATCH_1})
    endif()
    set(abiTag "$<$<CONFIG:Debug>:-d>") # TODO: add remaining abitags
    string(APPEND libname -$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},4>,x32,x64>)
    string(APPEND libname -${PROJECT_VERSION_MAJOR}_${PROJECT_VERSION_MINOR})
    if(NOT PROJECT_VERSION_PATCH EQUAL "0")
      string(APPEND libname _${PROJECT_VERSION_PATCH})
    endif()
    string(APPEND libname )
  elseif(NOT ARG_LAYOUT STREQUAL "system")
    message(FATAL_ERROR "Unsupported layout: ${ARG_LAYOUT}")
  endif()

  set_target_properties(${targetName} PROPERTIES
    DEFINE_SYMBOL ${targetNameUpper}_DYN_LINK
    POSITION_INDEPENDENT_CODE ON
    EXPORT_NAME ${name}
    OUTPUT_NAME ${libname}
    VERSION ${PROJECT_VERSION}
  )
  add_library(Boost::${name} ALIAS ${targetName})
  target_compile_definitions(${targetName} ${scope} ${targetNameUpper}_NO_LIB)
  target_include_directories(${targetName} ${system} ${scope}
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  )
endfunction()
