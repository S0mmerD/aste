#pragma once
#include <string>
#include <memory>

#include "mesh.hpp"

class MeshWriter
{
    public:
        MeshWriter(std::string filename);

        void writeMesh(std::shared_ptr<Mesh> mesh);
    private:
        std::string _filename;
};
