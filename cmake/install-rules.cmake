if(PROJECT_IS_TOP_LEVEL)
  set(
      CMAKE_INSTALL_INCLUDEDIR "include/psr_database-${PROJECT_VERSION}"
      CACHE STRING ""
  )
  set_property(CACHE CMAKE_INSTALL_INCLUDEDIR PROPERTY TYPE PATH)
endif()

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# find_package(<package>) call for consumers to find this project
set(package psr_database)

install(
    DIRECTORY
    include/
    "${PROJECT_BINARY_DIR}/export/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT psr_database_Development
)

install(
    TARGETS psr_database_psr_database
    EXPORT psr_databaseTargets
    RUNTIME #
    COMPONENT psr_database_Runtime
    LIBRARY #
    COMPONENT psr_database_Runtime
    NAMELINK_COMPONENT psr_database_Development
    ARCHIVE #
    COMPONENT psr_database_Development
    INCLUDES #
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

write_basic_package_version_file(
    "${package}ConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
)

# Allow package maintainers to freely override the path for the configs
set(
    psr_database_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${package}"
    CACHE STRING "CMake package config location relative to the install prefix"
)
set_property(CACHE psr_database_INSTALL_CMAKEDIR PROPERTY TYPE PATH)
mark_as_advanced(psr_database_INSTALL_CMAKEDIR)

install(
    FILES cmake/install-config.cmake
    DESTINATION "${psr_database_INSTALL_CMAKEDIR}"
    RENAME "${package}Config.cmake"
    COMPONENT psr_database_Development
)

install(
    FILES "${PROJECT_BINARY_DIR}/${package}ConfigVersion.cmake"
    DESTINATION "${psr_database_INSTALL_CMAKEDIR}"
    COMPONENT psr_database_Development
)

install(
    EXPORT psr_databaseTargets
    NAMESPACE psr_database::
    DESTINATION "${psr_database_INSTALL_CMAKEDIR}"
    COMPONENT psr_database_Development
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
