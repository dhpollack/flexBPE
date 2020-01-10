cmake_minimum_required(VERSION 3.10)


# set the project name
project(flexbpe 
	VERSION 0.0.1 
	DESCRIPTION "Byte Pair Encoding library based on fastBPE, but a bit bendier.")

# tests
option(BUILD_TEST "Build c++ tests" OFF)
if (BUILD_TEST)
  enable_testing()
  find_package(GTest)
  include(GoogleTest)
  add_subdirectory(test)
endif()

# Code Coverage Configuration
add_library(coverage_config INTERFACE)

option(CODE_COVERAGE "Enable coverage reporting" OFF)
if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  # Add required flags (GCC & LLVM/Clang)
  target_compile_options(coverage_config INTERFACE
    -O0        # no optimization
    -g         # generate debug info
    --coverage # sets all required flags
  )
  if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.13)
    target_link_options(coverage_config INTERFACE --coverage)
  else()
    target_link_libraries(coverage_config INTERFACE --coverage)
  endif()
endif()


# add subdirectory
add_subdirectory(flexBPE)



# create package file to add to other projects (generated from template)
configure_file(flexbpe.pc.in flexbpe.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/flexbpe.pc
	DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
include(CMakePackageConfigHelpers)
configure_package_config_file( 
  "flexbpe.cmake.in" 
  "flexbpeConfig.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/flexbpe
  PATH_VARS
    CMAKE_INSTALL_LIBDIR)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/flexbpeConfig.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/flexbpe")
install(EXPORT flexbpeTargets
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/flexbpe"
        FILE flexbpeTargets.cmake)
# make flag
set(CMAKE_CXX_FLAGS "-O3")

