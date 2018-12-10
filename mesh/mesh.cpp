#include "mesh.hpp"

Mesh::Mesh(vtkSmartPointer<vtkDataSet> data) : _data(data) {}

vtkSmartPointer<vtkDataSet> Mesh::data()
{
    return _data;
}

Vertex Mesh::point(int i)
{
    auto ptr = _data->GetPoint(i);
    return Vertex(ptr);
}

PointRange Mesh::allPoints()
{
    return PointRange(_data);
}

int Mesh::pointCount()
{
    return _data->GetNumberOfPoints();
}

Cell Mesh::cell(int i)
{
    auto ptr = _data->GetCell(i);
    return Cell(ptr);
}

CellRange Mesh::allCells()
{
    return CellRange(_data);
}

int Mesh::cellCount()
{
    return _data->GetNumberOfCells();
}

ValueRange Mesh::allValues(std::string tag)
{
    return ValueRange(_data->GetPointData(), tag);
}
int Mesh::valueCount(std::string tag)
{
    auto range = allValues(tag);
    return range.end() - range.begin();
}
