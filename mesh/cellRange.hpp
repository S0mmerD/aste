#pragma once
#include <boost/iterator/iterator_facade.hpp>
#include <vtkDataSet.h>
#include <vtkSmartPointer.h>
#include <vtkCell.h>
#include <vtkCell.h>

#include "indexIterator.hpp"

class Cell;
class CellIterator;

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
        vtkIdType* data();
        int size() const;
        vtkIdType vertex(int i) const;
    private:
        vtkCell* _data;
};
class CellIterator : public IndexIterator<CellIterator, Cell, Cell>
{
    public:
        CellIterator(vtkSmartPointer<vtkDataSet> polyData);
        CellIterator(vtkSmartPointer<vtkDataSet> polyData, int pos);
    private:
        friend class IndexIterator<CellIterator, Cell, Cell>;
        bool _equal(const CellIterator& other) const;
        Cell _dereference() const;

        vtkSmartPointer<vtkDataSet> _data;
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
