#pragma once
#include <string>
#include <vtkDataSet.h>
#include <vtkSmartPointer.h>
#include <memory>

#include "mesh.hpp"

class MeshReader 
{
    public: 
        MeshReader(std::string filename);

        std::shared_ptr<Mesh> readMesh();
    private:
        std::string _filename;
};
