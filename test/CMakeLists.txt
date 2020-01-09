cmake_minimum_required(VERSION 3.10)

mark_as_advanced(
    BUILD_GMOCK BUILD_GTEST BUILD_SHARED_LIBS
    gmock_build_tests gtest_build_samples gtest_build_tests
    gtest_disable_pthreads gtest_force_shared_crt gtest_hide_internal_symbols
)

include_directories(${PROJECT_SOURCE_DIR})

set(SOURCES "flexbpe_test.cpp")
add_executable(flexbpe-gtest ${SOURCES})
target_link_libraries(flexbpe-gtest gtest gtest_main flexbpe )
gtest_discover_tests(flexbpe-gtest WORKING_DIRECTORY .)

add_custom_command(TARGET flexbpe-gtest PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   "${CMAKE_CURRENT_SOURCE_DIR}/assets" $<TARGET_FILE_DIR:flexbpe-gtest>/assets)

