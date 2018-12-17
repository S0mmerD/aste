add_library(mesh mesh.cpp meshReader.cpp meshWriter.cpp cellRange.cpp pointRange.cpp valueRange.cpp)
target_link_libraries(mesh PRIVATE ${Boost_LIBRARIES})
target_include_directories(mesh PRIVATE ${Boost_INCLUDE_DIRS})
target_include_directories(mesh PUBLIC ${EIGEN3_INCLUDE_DIR})
