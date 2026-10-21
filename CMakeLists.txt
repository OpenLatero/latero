cmake_minimum_required (VERSION 3.21...3.31)
project (latero VERSION 0.2.0)

# on Windows, install into the MSYS2 environment of the compiler (e.g. C:/msys64/ucrt64) by default,
# instead of C:/Program Files
if (WIN32 AND CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	get_filename_component(MSYS2_PREFIX "${CMAKE_CXX_COMPILER}" DIRECTORY)
	get_filename_component(MSYS2_PREFIX "${MSYS2_PREFIX}" DIRECTORY)
	set(CMAKE_INSTALL_PREFIX "${MSYS2_PREFIX}" CACHE PATH "Install path prefix" FORCE)
endif()

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
# 0.x releases may break compatibility between minor versions, so only accept the same minor version
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/lateroConfigVersion.cmake
    COMPATIBILITY SameMinorVersion
)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/lateroConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/lateroConfigVersion.cmake
    DESTINATION lib/cmake/latero
)
