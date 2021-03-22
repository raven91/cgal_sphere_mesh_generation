cmake_minimum_required(VERSION 3.17)
project(cgal_sphere_mesh_generation)

set(CMAKE_CXX_STANDARD 17)

include_directories(/usr/local/Cellar/cgal/5.2/include/ /usr/local/include/)
link_directories(/usr/local/lib/)

set(GCC_COVERAGE_LINK_FLAGS "-lmpfr -lgmp")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

add_executable(cgal_sphere_mesh_generation main.cpp)