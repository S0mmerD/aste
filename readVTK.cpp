#include <iostream>

#include <vtkSmartPointer.h>
#include <vtkGenericDataObjectReader.h>
#include <vtkPointData.h>
#include <vtkDataArray.h>
#include <vtkDataArrayAccessor.h>
#include <vtkDataSetReader.h>
#include <vtkPolyDataReader.h>
#include <vtkPolyData.h>
#include <vtkArrayIterator.h>
#include <vtkArrayIteratorTemplate.h>
#include <vtkCellIterator.h>
#include <vtkIndent.h>

int main(int argc, char *argv[]) {
    const char* fileName = "example.vtk";
    vtkSmartPointer<vtkDataSetReader> reader =
          vtkSmartPointer<vtkDataSetReader>::New();
    reader->SetFileName(fileName);
    reader->Update();
    reader->GetOutput()->Register(reader);
    //vtkSmartPointer<vtkDataSet> polyData = reader->GetOutput();
    auto polyData = reader->GetOutput();
    auto pointCount = polyData->GetNumberOfPoints();
    auto pointData = polyData->GetPointData();
    for (int i = 0; i < pointData->GetNumberOfArrays(); i++){
        std::cout << (pointData->GetArrayName(i) ? pointData->GetArrayName(i) : "NULL") << std::endl;
        auto arr = pointData->GetAbstractArray(i);
        std::cout << "Size " << arr->GetSize() << std::endl;
        std::cout << "Numeric " << arr->IsNumeric() << std::endl;
        auto numArr = vtkDataArray::SafeDownCast(arr);
        for (int i = 0; i < numArr->GetNumberOfTuples(); i++){
            //auto tuple = numArr->GetTuple(i);
            auto tuple = polyData->GetPoint(i);
            std::cout << tuple[0] << " " << tuple[1] << " " << tuple[2] << std::endl;
        }
    }
    std::cout << "-------------Cells------------------\n";
    for (auto iterator = polyData->NewCellIterator(); !iterator->IsDoneWithTraversal(); iterator->GoToNextCell()){
        std::cout << "IDs:\t\tCoords:\n";
        auto ids = iterator->GetPointIds();
        auto coords = iterator->GetPoints();
        for (auto i = 0; i < ids->GetNumberOfIds(); i++){
            std::cout << "\t" << ids->GetId(i);
            auto ptr = coords->GetPoint(i);
            std::cout << "\t" << ptr[0] << " " << ptr[1] << " " << ptr[2] << "\n";
        }
    }
}
