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

void writeToPrecice(const OptionMap& opts, std::vector<idx_t> partition, shared_ptr<MPIEnv> env);

int main(int argc, char *argv[]) {
    auto globalMPI = make_shared<MPIEnv>(argc, argv);
    auto options = getOptions(argc, argv);
    auto meshReader = MeshReader(options["meshFile"].as<string>());
    auto mesh = meshReader.readMesh();
    auto part = partition(mesh, globalMPI->size());
    colorMesh(mesh, part);
    MeshWriter("colored.vtk").writeMesh(mesh);
    auto precice = makePreciceFromOptions(globalMPI, options, mesh);
    precice.couple();
}

