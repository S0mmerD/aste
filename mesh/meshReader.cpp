#include <vtksys/SystemTools.hxx>
#include <vtkDataSet.h>
#include <vtkDataSetReader.h>
#include <vtkPolyData.h>
#include <stdexcept>

#include "meshReader.hpp"
#include "mesh.hpp"

using namespace std;

MeshReader::MeshReader(string filename) : _filename{std::move(filename)} 
{}

std::shared_ptr<Mesh> MeshReader::readMesh()
{
    auto extension = vtksys::SystemTools::GetFilenameLastExtension(_filename);
    vtkSmartPointer<vtkDataSet> dataSet;
    if (extension == ".vtk"){
        auto reader = vtkDataSetReader::New();
        reader->SetFileName(_filename.c_str());
        reader->Update();
        dataSet = reader->GetOutput();
    }
    else
        throw runtime_error(std::string("Unknown filetype: ") + extension);
    return make_shared<Mesh>(dataSet);
}
