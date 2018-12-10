#pragma once
#include <boost/iterator/iterator_facade.hpp>
#include <vtkDataArray.h>
#include <vtkPointData.h>
#include <vtkSmartPointer.h>

#include "indexIterator.hpp"

class ValueIterator;

class ValueRange
{
    public:
        ValueRange(vtkSmartPointer<vtkPointData> data, std::string tag);
        ValueIterator begin() const;
        ValueIterator end() const;
    private:
        vtkSmartPointer<vtkDataArray> _data;
        std::string _tag;
};

class ValueIterator : public IndexIterator<ValueIterator, double>
{
    public:
        ValueIterator(vtkSmartPointer<vtkDataArray> data);
        ValueIterator(vtkSmartPointer<vtkDataArray> data, int pos);
    private:
        friend class IndexIterator<ValueIterator, double>;
        double& _dereference() const 
        {
            return *(_data->GetTuple(_index));
        }
        bool _equal(const ValueIterator& other) const
        {
            return _data == other._data;
        }

        vtkSmartPointer<vtkDataArray> _data;
};
