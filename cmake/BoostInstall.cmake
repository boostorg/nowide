include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Install all passed libraries including *Config.cmake and *ConfigVersion.cmake
# Requires a file Config.cmake.in the projects source dir
# Arguments:
#   - TARGETS: List of targets to install
function(boost_install)
  cmake_parse_arguments(PARSE_ARGV 0 ARG "" "LAYOUT" "TARGETS")
  if(ARG_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR "Invalid argument(s): ${ARG_UNPARSED_ARGUMENTS}")
  endif()
  if(NOT ARG_LAYOUT)
    set(ARG_LAYOUT system)
  endif()

  if(ARG_LAYOUT STREQUAL "versioned")
    set(install_includedir ${CMAKE_INSTALL_INCLUDEDIR}/boost-${PROJECT_VERSION_MAJOR}_${PROJECT_VERSION_MINOR})
    set(configFolder ${PROJECT_NAME}-${PROJECT_VERSION})
  elseif(ARG_LAYOUT STREQUAL "system")
    set(install_includedir ${CMAKE_INSTALL_INCLUDEDIR})
    set(configFolder ${PROJECT_NAME})
  endif()
  foreach(tgt IN LISTS ARG_TARGETS)
    target_include_directories(${tgt} INTERFACE $<INSTALL_INTERFACE:${install_includedir}>)
  endforeach()

  install(TARGETS ${ARG_TARGETS}
    EXPORT ${PROJECT_NAME}Targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  )
  install(DIRECTORY include/ DESTINATION ${install_includedir})

  set(configFile ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake)
  set(versionFile ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake)
  set(configInstallDestination lib/cmake/${configFolder})

  configure_package_config_file(
    ${PROJECT_SOURCE_DIR}/Config.cmake.in
    ${configFile} 
    INSTALL_DESTINATION ${configInstallDestination}
  )
  write_basic_package_version_file(
    ${versionFile}
    COMPATIBILITY SameMajorVersion
  )

  install(FILES ${configFile} ${versionFile} DESTINATION ${configInstallDestination})

  install(
    EXPORT ${PROJECT_NAME}Targets
    NAMESPACE "Boost::"
    DESTINATION ${configInstallDestination}
  )
endfunction()
