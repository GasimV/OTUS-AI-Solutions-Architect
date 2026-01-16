cmake_minimum_required(VERSION 3.15)
project(AIArchitectCalculators)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add executable
add_executable(calculator_server
    server/main.cpp
)

# Platform-specific settings
if(WIN32)
    target_link_libraries(calculator_server ws2_32)
endif()

# Set output directory
set_target_properties(calculator_server PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)
