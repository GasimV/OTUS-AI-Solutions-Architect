cmake_minimum_required(VERSION 3.15)
project(AIArchitectADRATAM LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

if(MSVC)
    add_compile_options(/W4 /permissive- /Zc:__cplusplus /utf-8)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS NOMINMAX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

if(WIN32)
    add_compile_definitions(_WIN32_WINNT=0x0A00 WINVER=0x0A00)
endif()

set(ADR_ATAM_CORE_SOURCES
    server/util/util.cpp
    server/domain/adr.cpp
    server/domain/atam.cpp
    server/persistence/file_store.cpp
    server/persistence/adr_repository.cpp
    server/persistence/atam_repository.cpp
    server/services/template_service.cpp
    server/services/adr_service.cpp
    server/services/atam_service.cpp
    server/services/export_service.cpp
    server/services/reuse_service.cpp
    server/ai/gemini_client.cpp
    server/ai/ollama_client.cpp
    server/ai/provider_llm_client.cpp
    server/ai/ai_service.cpp
    server/http/api_routes.cpp
)

add_library(adr_atam_core STATIC ${ADR_ATAM_CORE_SOURCES})

target_include_directories(adr_atam_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/server
    ${CMAKE_CURRENT_SOURCE_DIR}/server/third_party
)

if(WIN32)
    target_link_libraries(adr_atam_core PUBLIC ws2_32 crypt32)
endif()

find_package(OpenSSL QUIET)
if(OpenSSL_FOUND)
    target_compile_definitions(adr_atam_core PUBLIC CPPHTTPLIB_OPENSSL_SUPPORT)
    target_link_libraries(adr_atam_core PUBLIC OpenSSL::SSL OpenSSL::Crypto)
else()
    message(WARNING "OpenSSL not found; Gemini provider will build but report unavailable at runtime.")
endif()

if(NOT WIN32)
    find_package(Threads REQUIRED)
    target_link_libraries(adr_atam_core PUBLIC Threads::Threads)
endif()

add_executable(adr_atam_server server/main.cpp)
target_link_libraries(adr_atam_server PRIVATE adr_atam_core)

set_target_properties(adr_atam_server PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

enable_testing()
add_subdirectory(tests)
