#pragma once
#include <Eigen/Dense>
#include <boost/iterator/iterator_facade.hpp>
#include <vtkDataSet.h>
#include <vtkSmartPointer.h>
#include <vtkCell.h>

#include "pointRange.hpp"
#include "cellRange.hpp"
#include "valueRange.hpp"

class ValueRange;

class Mesh 
{
    public:
        Mesh() =default;
        Mesh(vtkSmartPointer<vtkDataSet> data);

        vtkSmartPointer<vtkDataSet> data();

        Vertex point(int i);
        PointRange allPoints();
        int pointCount();

        Cell cell(int i);
        CellRange allCells();
        int cellCount();

        ValueRange allValues(std::string tag);
        int valueCount(std::string tag);
    private:
        vtkSmartPointer<vtkDataSet> _data;
};

