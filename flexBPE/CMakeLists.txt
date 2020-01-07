# list sourcefiles for convenience
set(LIB_SRC 
    flexBPE.cpp)
set(APP_SRC
    main.cpp)

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
                              PUBLIC_HEADER flexBPE.h)

# makes working with subdirectories easier, but right now not used
target_include_directories(flexbpe PRIVATE .)

# install
include(GNUInstallDirs)
install(TARGETS flexbpe
        EXPORT flexbpeTargets
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(TARGETS flexbpe-bin
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
# preferred way of getting pthreads
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
# link shared library to pthread
target_link_libraries(flexbpe Threads::Threads)
# link executable to shared library and threads
target_link_libraries(flexbpe-bin flexbpe Threads::Threads)

