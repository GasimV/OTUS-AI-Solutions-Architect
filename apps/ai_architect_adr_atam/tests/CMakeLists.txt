add_executable(adr_atam_tests
    test_main.cpp
    test_util.cpp
    test_domain.cpp
    test_persistence.cpp
    test_services.cpp
)
target_link_libraries(adr_atam_tests PRIVATE adr_atam_core)
target_include_directories(adr_atam_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_test(NAME adr_atam_tests COMMAND adr_atam_tests)
