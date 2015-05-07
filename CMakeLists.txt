cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(tbjcexample)

# GEANT4 core
find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(~/eigen/)
# CADMesh
find_package(cadmesh)

SET(GCC_COVERAGE_COMPILE_FLAGS "-std=c++0x -lsqlite3")
add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})


# User code
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

add_executable(tbjcexample tbjcexample.cc ${sources} ${headers})
target_link_libraries(tbjcexample ${Geant4_LIBRARIES})
target_link_libraries(tbjcexample ${cadmesh_LIBRARIES})
target_link_libraries(tbjcexample sqlite3)
target_link_libraries(tbjcexample json)

