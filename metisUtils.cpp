#include "metisUtils.hpp"

#include <vtkLongLongArray.h>
#include <vtkPointData.h>
#include <utils/prettyprint.hpp>

std::vector<idx_t> partition(std::shared_ptr<Mesh> mesh, idx_t nparts)
{
    using namespace std;
    vector<idx_t> cellsizes;
    vector<idx_t> celldata;
    cellsizes.reserve(mesh->cellCount() + 1);
    celldata.reserve(mesh->cellCount() * 5); //TODO: Arbitrary parameter! Tune this!
    cellsizes.push_back(0);
    for (const auto& cell : mesh->allCells()){
        auto sum = cellsizes[cellsizes.size() - 1];
        cellsizes.push_back(sum + cell.size());
        for (int i = 0; i < cell.size(); i++){
            celldata.push_back(cell.vertex(i));
        }
    }
    idx_t el_count = mesh->cellCount();
    idx_t point_count = mesh->pointCount();
    idx_t objval;
    idx_t options[METIS_NOPTIONS];
    METIS_SetDefaultOptions(options);
    vector<idx_t> point_partition(mesh->pointCount());
    vector<idx_t> cell_partition(mesh->cellCount());
    int result = METIS_PartMeshNodal(&el_count, &point_count, cellsizes.data(), celldata.data(), 0, 0, &nparts, 0, options, &objval, cell_partition.data(), point_partition.data());
    return point_partition;
}

void colorMesh(std::shared_ptr<Mesh> mesh, std::vector<idx_t> partition)
{
    auto colors = vtkSmartPointer<vtkLongLongArray>::New();
    colors->SetNumberOfComponents(1);
    colors->SetName("Colors");
    for (auto i = 0; i < partition.size(); i++){
        double value = partition[i];
        colors->InsertNextTuple(&value);
    }
    auto polyData = mesh->data();
    polyData->GetPointData()->SetScalars(colors);
}
