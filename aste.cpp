#include <thread>
#include <chrono>
#include <string>
#include <cmath>
#include <iostream>
#include <mpi.h>

#include "precice/SolverInterface.hpp"
#include "utils/EventTimings.hpp"
#include "utils/prettyprint.hpp"

#include "common.hpp"


using std::cout;
using std::endl;


DistMesh getMyMesh(OptionMap options, int MPIrank, int MPIsize)
{
  double xSize = options["x"].as<double>();
  double ySize = options["y"].as<double>();
  int nx =  options["nx"].as<int>();
  int ny =  options["ny"].as<int>();
  int deadrank = options["deadrank"].as<int>();
  double xInc = xSize / nx;
  double yInc = ySize / ny;

  DistMesh mesh;
  for (int i=0; i<nx; i++) {
    for (int j=0; j<ny; j++) {
      DistVertex v = { i*xInc, j*yInc};
      mesh.push_back(v);
    }
  }
  if (deadrank >= 0) {
    --MPIsize;
    if (MPIrank == deadrank)
      return DistMesh();
    if (MPIrank > deadrank)
      --MPIrank; // shift all MPIranks
  }

  return partition(mesh, MPIsize)[MPIrank];
}


/// Fills a vector with data values
Data getData(DistMesh& mesh)
{
  Data data;
  for (auto &v : mesh) {
    data.emplace_back(v[0] + v[1] + 1);
  }
  return data;
}


int main(int argc, char *argv[])
{
  MPI_Init(&argc, &argv);

  OptionMap options = getOptions(argc, argv);
  printOptions(options);
  std::string participant = options["participant"].as<std::string>();
  
  int MPIrank = 0, MPIsize = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &MPIrank);
  MPI_Comm_size(MPI_COMM_WORLD, &MPIsize);

  precice::SolverInterface interface(participant, MPIrank, MPIsize);
  interface.configure(options["precice-config"].as<std::string>());
  precice::utils::EventRegistry::instance().runName = options["runName"].as<std::string>();

  int meshID = interface.getMeshID( (participant == "A") ? "MeshA" : "MeshB" );
  int dataID = interface.getDataID("Data", meshID);

  DistMesh mesh = getMyMesh(options, MPIrank, MPIsize);
  Data data;
  if (participant == "A")
    data = getData(mesh); // only A has a mesh with values
  else
    data = Data(mesh.size(), 0); // B is mapped to
  
  printMesh(mesh, data, options["printMesh"].as<bool>());
  std::vector<int> vertexIDs;
  
  size_t localN = mesh.size();
  for (size_t i = 0; i < localN; i++) {
    vertexIDs.push_back(interface.setMeshVertex(meshID, static_cast<const double *>(mesh[i].data())));
  }
  
  interface.initialize();

  if (interface.isActionRequired(precice::constants::actionWriteInitialData())) {
    interface.writeBlockScalarData(dataID, localN, vertexIDs.data(), data.data());
    cout << "Wrote initial data = " << data << endl;
    interface.fulfilledAction(precice::constants::actionWriteInitialData());
  }
  interface.initializeData();

  while (interface.isCouplingOngoing()) {
    if (participant == "A" and not data.empty()) {
      interface.writeBlockScalarData(dataID, localN, vertexIDs.data(), data.data());
      // cout << "Rank = " << MPIrank << " A Wrote data = " << data << endl;
    }

    interface.advance(1);
    if (participant == "B") {
      interface.readBlockScalarData(dataID, localN, vertexIDs.data(), data.data());
    // cout << "Rank = " << MPIrank << " B Read data = " << data << endl;
    }
  }
  interface.finalize();

  // std::this_thread::sleep_for(std::chrono::milliseconds(100 * MPIrank));
  // printMesh(mesh, data, true);
  
  MPI_Finalize();

  return 0;
}
