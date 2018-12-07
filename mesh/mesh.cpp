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

PointRange::PointRange(vtkSmartPointer<vtkDataSet> data) : _data(data) {}

PointIterator PointRange::begin() const
{
    return PointIterator(_data); 
}

PointIterator PointRange::end() const
{
    int count = _data->GetNumberOfPoints();
    return PointIterator(_data, count);
}

PointIterator::PointIterator(vtkSmartPointer<vtkDataSet> data) : PointIterator(data, 0) {}

PointIterator::PointIterator(vtkSmartPointer<vtkDataSet> data, int pos) : _data(data), i(pos) {}

void PointIterator::increment()
{
    i++;
}

void PointIterator::decrement()
{
    i--;
}

bool PointIterator::equal(const PointIterator& other) const
{
    return i == other.i && _data == other._data;
}

Vertex PointIterator::dereference() const
{
    auto ptr = _data->GetPoint(i);
    return Vertex(ptr);
}

CellRange::CellRange(vtkSmartPointer<vtkDataSet> data) : _data(data) {}

CellIterator CellRange::begin() const
{
    return CellIterator(_data);
}

CellIterator CellRange::end() const
{
    int count = _data->GetNumberOfCells();
    return CellIterator(_data, count);
}

vtkIdType* Cell::data()
{
    return _data->GetPointIds()->GetPointer(0);
}

int Cell::size() const
{
    return _data->GetNumberOfPoints();
}

vtkIdType Cell::vertex(int i) const
{
    return _data->GetPointId(i);
}

CellIterator::CellIterator(vtkSmartPointer<vtkDataSet> data) : CellIterator(data, 0) {}

CellIterator::CellIterator(vtkSmartPointer<vtkDataSet> data, int pos) : _data(data), i(pos) {}

void CellIterator::increment()
{
    i++;
}

void CellIterator::decrement()
{
    i--;
}

bool CellIterator::equal(const CellIterator& other) const
{
    return i == other.i && _data == other._data;
}

Cell CellIterator::dereference() const
{
    auto ptr = _data->GetCell(i);
    return Cell(ptr);
}
