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

        vtkSmartPointer<vtkDataSet> _data; //TODO: make this private again ASAP!!
    private:
        vtkSmartPointer<vtkDataSet> readPolyData();

        bool _ready = false;
        std::string _filename;
        std::shared_ptr<Mesh> _mesh;
};
