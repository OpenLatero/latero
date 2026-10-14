cmake_minimum_required(VERSION 3.10...3.31)
Project(latero-example-vibrate-loop)

find_package(latero REQUIRED)

set(SOURCES main.cpp)

add_executable(latero-example-vibrate-loop ${SOURCES})
set_target_properties(latero-example-vibrate-loop PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
target_link_libraries(latero-example-vibrate-loop latero::latero)
install_programs(/bin latero-example-vibrate-loop)
