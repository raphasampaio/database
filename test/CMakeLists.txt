cmake_minimum_required(VERSION 3.14)

project(psr_databaseTests LANGUAGES CXX)

include(../cmake/project-is-top-level.cmake)
include(../cmake/folders.cmake)

# ---- Dependencies ----

if(PROJECT_IS_TOP_LEVEL)
  find_package(psr_database REQUIRED)
  enable_testing()
endif()

# ---- Tests ----

add_executable(psr_database_test source/psr_database_test.cpp)
target_link_libraries(psr_database_test PRIVATE psr_database::psr_database)
target_compile_features(psr_database_test PRIVATE cxx_std_17)

add_test(NAME psr_database_test COMMAND psr_database_test)

# ---- End-of-file commands ----

add_folders(Test)
