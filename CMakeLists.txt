cmake_minimum_required (VERSION 3.1)
project(ASTE)

list (APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake-modules")

set (CMAKE_CXX_STANDARD 11)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()
message(STATUS "Build configuration: " ${CMAKE_BUILD_TYPE})

find_package(precice PATHS $ENV{PRECICE_ROOT}/build REQUIRED)

find_package(Boost 1.60.0 REQUIRED COMPONENTS system program_options filesystem)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

find_package(METIS REQUIRED)

add_subdirectory(mesh)

add_executable(readMesh readMesh.cpp)
add_executable(aste aste.cpp)
add_executable(readVTK readVTK.cpp metisUtils.cpp)
add_library(common common.cpp)

target_link_libraries(common preCICE)

target_link_libraries(readVTK ${Boost_LIBRARIES})
target_include_directories(readVTK PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(readVTK ${MPI_LIBRARIES})
target_link_libraries(readVTK preCICE)
target_link_libraries(readVTK ${VTK_LIBRARIES})
target_link_libraries(readVTK ${METIS_LIBRARIES})
target_link_libraries(readVTK common)
target_link_libraries(readVTK mesh)

target_link_libraries(aste ${Boost_LIBRARIES})
target_include_directories(readVTK PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(aste ${MPI_LIBRARIES})
target_link_libraries(aste preCICE)
target_link_libraries(aste common)

target_link_libraries(readMesh ${Boost_LIBRARIES})
target_include_directories(readVTK PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(readMesh ${MPI_LIBRARIES})
target_link_libraries(readMesh preCICE)
target_link_libraries(readMesh common)

