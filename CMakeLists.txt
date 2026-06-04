cmake_minimum_required (VERSION 3.21...3.31)
project (latero)

set(Boost_USE_MULTITHREADED ON)
find_package(Boost CONFIG COMPONENTS date_time REQUIRED)

# process subdirectories
add_subdirectory (latero)

# generate doc if Doxygen is found
FIND_PACKAGE(Doxygen)
IF (DOXYGEN_FOUND)
  ADD_CUSTOM_TARGET(doc ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/doxygen.cfg
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
ENDIF(DOXYGEN_FOUND)

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
