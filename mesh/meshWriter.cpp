#include "meshWriter.hpp"

#include <vtkDataSetWriter.h>

MeshWriter::MeshWriter(std::string filename) : _filename{std::move(filename)} {}

void MeshWriter::writeMesh(std::shared_ptr<Mesh> mesh)
{
    auto writer = vtkSmartPointer<vtkDataSetWriter>::New();
    writer->SetFileName(_filename.c_str());
    writer->SetInputData(mesh->data()); //VTK does not give much about const-correctness.
    writer->Write();
}
