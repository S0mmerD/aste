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

        void readMesh();
        std::shared_ptr<Mesh> mesh();
    private:
        vtkSmartPointer<vtkDataSet> readPolyData();

        bool _ready = false;
        std::string _filename;
        std::shared_ptr<Mesh> _mesh;
        vtkSmartPointer<vtkDataSet> _data;
};
