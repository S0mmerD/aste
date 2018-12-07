#include <iostream>
#include <mpi.h>

#include "common.hpp"
#include "mpiUtils.hpp"
#include "precice.hpp"
#include "mesh/mesh.hpp"
#include "mesh/meshReader.hpp"
#include "mesh/meshWriter.hpp"
#include "metisUtils.hpp"

using namespace std;

int main(int argc, char *argv[]) {
    auto globalMPI = make_shared<MPIEnv>(argc, argv);
    auto options = getOptions(argc, argv);
    auto meshReader = MeshReader(options["meshFile"].as<string>());
    auto mesh = meshReader.readMesh();
    auto part = partition(mesh, 4);
    colorMesh(mesh, part);
    MeshWriter("colored.vtk").writeMesh(mesh);
}
