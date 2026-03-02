cmake_minimum_required (VERSION 3.10...3.31)
project (latero)

set(Boost_USE_STATIC_LIBS   ON)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost CONFIG COMPONENTS date_time REQUIRED)

# todo: The DEPS_LDFLAGS and DEPS_CFLAGS strings separate the arguments with semicolons, which break
# when passed to latero.pc. This replaces them with spaces. There must be a better way of doing this.
# Adding -I in front of include dirs as well.
STRING(REPLACE ";" " " DEPS_LDFLAGS_WSPACE "${Boost_LIBRARIES}" )
STRING(REPLACE ";" " -I" DEPS_CFLAGS_WSPACE "-I${Boost_INCLUDE_DIRS}" )

# process subdirectories
add_subdirectory (latero)


# Create and install cmake config file

include(CMakePackageConfigHelpers)

configure_package_config_file(
    cmake/lateroConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/lateroConfig.cmake
    INSTALL_DESTINATION lib/cmake/latero
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/lateroConfig.cmake
    DESTINATION lib/cmake/latero
)
