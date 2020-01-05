cmake_minimum_required(VERSION 3.10)

# list sourcefiles for convenience
set(LIB_SRC 
    flexBPE/flexBPE.cpp)
set(APP_SRC
    flexBPE/main.cpp)

# set the project name
project(flexbpe 
	VERSION 0.0.1 
	DESCRIPTION "Byte Pair Encoding library based on fastBPE, but a bit bendier.")

# create library and executable
add_library(flexbpe SHARED ${LIB_SRC})
add_executable(flexbpe-bin ${APP_SRC})

# link the executable to the shared library
set(CMAKE_INSTALL_RPATH "${ORIGIN}")
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

# set some properties
set_target_properties(flexbpe-bin PROPERTIES OUTPUT_NAME "flexbpe")
set_target_properties(flexbpe PROPERTIES 
	                              VERSION ${PROJECT_VERSION} 
                                      SOVERSION 0
                                      PUBLIC_HEADER flexBPE/flexBPE.h)

# makes working with subdirectories easier, but right now not used
target_include_directories(flexbpe PRIVATE .)

# install
include(GNUInstallDirs)
install(TARGETS flexbpe
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(TARGETS flexbpe-bin
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

# create package file to add to other projects (generated from template)
configure_file(flexbpe.pc.in flexbpe.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/flexbpe.pc
	DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

# make flag
set(CMAKE_CXX_FLAGS "-O3")

# preferred way of getting pthreads
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
# link shared library to pthread
target_link_libraries(flexbpe Threads::Threads)
# link executable to shared library and threads
target_link_libraries(flexbpe-bin flexbpe Threads::Threads)
