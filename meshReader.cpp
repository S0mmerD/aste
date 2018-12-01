#include <vtksys/SystemTools.hxx>
#include <vtkDataSet.h>
#include <vtkDataSetReader.h>
#include <vtkPolyData.h>
#include <stdexcept>

#include "meshReader.hpp"
#include "mesh.hpp"

using namespace std;

MeshReader::MeshReader(string filename) : _filename{filename} 
{}

void MeshReader::readMesh()
{
    _data = readPolyData();
    std::cout << "PolyData:" << vtkPolyData::SafeDownCast(_data) << "\n";
    _mesh = make_shared<Mesh>(_data);
    _ready = true;
}

vtkSmartPointer<vtkDataSet> MeshReader::readPolyData()
{
    auto extension = vtksys::SystemTools::GetFilenameLastExtension(_filename);
    if (extension == ".vtk"){
        auto reader = vtkDataSetReader::New();
        reader->SetFileName(_filename.c_str());
        reader->Update();
        return reader->GetOutput();
    }
    throw runtime_error(std::string("Unknown filetype: ") + extension);
}

std::shared_ptr<Mesh> MeshReader::mesh()
{
    if (!_ready)
        throw logic_error("MeshReader is not ready yet. Did you forget to call readMesh()?");
    return _mesh;
}
