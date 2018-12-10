#pragma once
#include <Eigen/Dense>
#include <boost/iterator/iterator_facade.hpp>
#include <vtkDataSet.h>
#include <vtkSmartPointer.h>

#include "indexIterator.hpp"
using Vertex = Eigen::Map<Eigen::Vector3d>;

class PointIterator;

class PointRange
{
    public:
        PointRange(vtkSmartPointer<vtkDataSet> data);
        PointIterator begin() const;
        PointIterator end() const;
    private:
        vtkSmartPointer<vtkDataSet> _data;
};

class PointIterator : public IndexIterator<PointIterator, Vertex, Vertex>
{
    public:
        PointIterator(vtkSmartPointer<vtkDataSet> polyData);
        PointIterator(vtkSmartPointer<vtkDataSet> polyData, int pos);
    private:
        friend class IndexIterator<PointIterator, Vertex, Vertex>;
        bool _equal(const PointIterator& other) const;
        Vertex _dereference() const;

        vtkSmartPointer<vtkDataSet> _data;
};
