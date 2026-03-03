cmake_minimum_required (VERSION 3.10...3.31)
project (latero)

set(Boost_USE_STATIC_LIBS   ON)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost CONFIG COMPONENTS date_time REQUIRED)

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
