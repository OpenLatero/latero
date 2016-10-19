cmake_minimum_required (VERSION 2.4)
project (latero) 

set(Boost_USE_STATIC_LIBS   ON)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost COMPONENTS date_time REQUIRED)

link_directories(${Boost_LIBRARY_DIRS})
include_directories(${Boost_INCLUDE_DIRS})

# todo: The DEPS_LDFLAGS and DEPS_CFLAGS strings separate the arguments with semicolons, which break
# when passed to latero.pc. This replaces them with spaces. There must be a better way of doing this.
STRING(REPLACE ";" " " DEPS_LDFLAGS_WSPACE "${Boost_LIBRARIES}" )
STRING(REPLACE ";" " " DEPS_CFLAGS_WSPACE "${Boost_INCLUDE_DIRS}" )

# process subdirectories
add_subdirectory (latero)

# generate latero.pc file and install it for pkg-config
configure_file(latero.pc.cmake ${CMAKE_CURRENT_BINARY_DIR}/latero.pc @ONLY)
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/latero.pc DESTINATION lib/pkgconfig)
