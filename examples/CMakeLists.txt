cmake_minimum_required(VERSION 3.14)

# Example executables (only built in developer mode)
if(psr_database_DEVELOPER_MODE AND PROJECT_IS_TOP_LEVEL)
    add_executable(sqlite_demo sqlite_demo.cpp)
    target_link_libraries(sqlite_demo PRIVATE psr_database::psr_database)
    target_compile_features(sqlite_demo PRIVATE cxx_std_17)
    
    # Set the output directory
    set_target_properties(sqlite_demo PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/examples"
    )
endif()
