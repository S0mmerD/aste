#pragma once
#include <Eigen/Dense>
#include <boost/iterator/iterator_facade.hpp>
#include <vtkDataSet.h>
#include <vtkSmartPointer.h>
#include <vtkCell.h>
using namespace std;
using namespace Eigen;
using Vertex = Map<Vector3d>;

class PointRange;
class PointIterator;
class CellRange;
class Cell;
class CellIterator;

class Mesh 
{
    public:
        Mesh() =default;
        Mesh(vtkSmartPointer<vtkDataSet> data);

        Vertex point(int i);
        PointRange allPoints();
        int pointCount();

        Cell cell(int i);
        CellRange allCells();
        int cellCount();
    private:
        vtkSmartPointer<vtkDataSet> _data;
};

class PointRange
{
    public:
        PointRange(vtkSmartPointer<vtkDataSet> data);
        PointIterator begin() const;
        PointIterator end() const;
    private:
        vtkSmartPointer<vtkDataSet> _data;
};

class PointIterator : public boost::iterator_facade
                      <PointIterator, Vertex, boost::bidirectional_traversal_tag, Vertex>
{
    public:
        PointIterator(vtkSmartPointer<vtkDataSet> polyData);
        PointIterator(vtkSmartPointer<vtkDataSet> polyData, int pos);
    private:
        friend class boost::iterator_core_access;
        void increment();
        void decrement();
        bool equal(const PointIterator& other) const;
        Vertex dereference() const;

        vtkSmartPointer<vtkDataSet> _data;
        int i;
};

class CellRange
{
    public:
        CellRange(vtkSmartPointer<vtkDataSet> data);
        CellIterator begin() const;
        CellIterator end() const;
    private:
        vtkSmartPointer<vtkDataSet> _data;
};

class Cell
{
    public:
        Cell(vtkCell* cell) : _data{cell} {}
        int size() const;
        int vertex(int i) const;
    private:
        vtkCell* _data;
};
class CellIterator : public boost::iterator_facade
                     <CellIterator, Cell, boost::bidirectional_traversal_tag, Cell>
{
    public:
        CellIterator(vtkSmartPointer<vtkDataSet> polyData);
        CellIterator(vtkSmartPointer<vtkDataSet> polyData, int pos);
    private:
        friend class boost::iterator_core_access;
        void increment();
        void decrement();
        bool equal(const CellIterator& other) const;
        Cell dereference() const;

        vtkSmartPointer<vtkDataSet> _data;
        int i;
};

inline std::ostream& operator<<(ostream& out, const Cell& cell)
{
    const auto token = " "; 
    const auto* sep = "";
    for (int i = 0; i < cell.size(); i++){
        out << sep << cell.vertex(i);  
        sep = token;
    }
    return out;
}
