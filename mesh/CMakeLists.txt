add_library(mesh mesh.cpp meshReader.cpp meshWriter.cpp cellRange.cpp pointRange.cpp valueRange.cpp)
target_link_libraries(mesh PUBLIC Eigen3::Eigen)
