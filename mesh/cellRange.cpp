#include "cellRange.hpp"

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

CellIterator::CellIterator(vtkSmartPointer<vtkDataSet> data, int pos) : _data(data)
{
    _index = pos;
}

bool CellIterator::_equal(const CellIterator& other) const
{
    return _data == other._data;
}

Cell CellIterator::_dereference() const
{
    auto ptr = _data->GetCell(_index);
    return Cell(ptr);
}
