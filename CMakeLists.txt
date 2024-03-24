cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(3d-renderer-from-scratch)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_subdirectory(eigen-3.4.0)
add_subdirectory(3DRenderer)
