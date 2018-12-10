#include "pointRange.hpp"

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

PointIterator::PointIterator(vtkSmartPointer<vtkDataSet> data, int pos) : _data(data)
{
    _index = pos;
}

bool PointIterator::_equal(const PointIterator& other) const
{
    return _data == other._data;
}

Vertex PointIterator::_dereference() const
{
    auto ptr = _data->GetPoint(_index);
    return Vertex(ptr);
}
