#include <exception>

#include "valueRange.hpp"

ValueRange::ValueRange(vtkSmartPointer<vtkPointData> data, std::string tag)
    : _tag{std::move(tag)} 
{
    int i = 42;
    _data = vtkDataArray::SafeDownCast(data->GetAbstractArray(_tag.c_str(), i));
    if (!_data)
        throw std::runtime_error("Invalid Data Array");
    if (_data->GetNumberOfComponents() != 1)
        throw std::runtime_error("Tuples have more than one element");
}
ValueIterator ValueRange::begin() const
{
    return ValueIterator(_data);
}
ValueIterator ValueRange::end() const
{
    return ValueIterator(_data, _data->GetNumberOfTuples()); 
}


ValueIterator::ValueIterator(vtkSmartPointer<vtkDataArray> data) : ValueIterator(data, 0) {}

ValueIterator::ValueIterator(vtkSmartPointer<vtkDataArray> data, int pos) : _data{data}
{
    _index = pos;
}
