cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    psr_database
    VERSION 0.1.0
    DESCRIPTION "Short description"
    HOMEPAGE_URL "https://github.com/psrenergy/database"
    LANGUAGES CXX
)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

add_library(
    psr_database_psr_database
    source/psr_database.cpp
)
add_library(psr_database::psr_database ALIAS psr_database_psr_database)

include(GenerateExportHeader)
generate_export_header(
    psr_database_psr_database
    BASE_NAME psr_database
    EXPORT_FILE_NAME export/psr_database/psr_database_export.hpp
    CUSTOM_CONTENT_FROM_VARIABLE pragma_suppress_c4251
)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(psr_database_psr_database PUBLIC PSR_DATABASE_STATIC_DEFINE)
endif()

set_target_properties(
    psr_database_psr_database PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
    VERSION "${PROJECT_VERSION}"
    SOVERSION "${PROJECT_VERSION_MAJOR}"
    EXPORT_NAME psr_database
    OUTPUT_NAME psr_database
)

target_include_directories(
    psr_database_psr_database ${warning_guard}
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
)

target_include_directories(
    psr_database_psr_database SYSTEM
    PUBLIC
    "\$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/export>"
)

target_compile_features(psr_database_psr_database PUBLIC cxx_std_17)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT psr_database_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of psr_database"
  )
endif()

include(cmake/dev-mode.cmake)
