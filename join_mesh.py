import vtk
import os
import numpy as np
import argparse

def main():
    args = parse_args()
    mesh = read_mesh(args.in_meshname, args.numparts)
    out_meshname = args.out_meshname if args.out_meshname else args.in_meshname + ".vtk"
    write_mesh(mesh, out_meshname, args.valuetag)
def read_mesh(dirname, length):
    dirname = os.path.abspath(dirname)
    if not os.path.exists(dirname):
        raise Exception("Directory not found")
    mesh = []
    for i in range(length):
        with open(dirname + "/" + str(i), "r") as file:
            lines = file.readlines()
            for line in lines:
                splitted = line.split(" ")
                if (len(splitted) < 3):
                    print("Empty line. This is bad.")
                    continue
                entry = ()
                for part in splitted:
                    entry += (float(part),)
                mesh.append(entry)
    return mesh
def write_mesh(mesh, out_meshname, valuetag):
    if not valuetag:
        valuetag = "Values"
    writer = vtk.vtkUnstructuredGridWriter()
    writer.SetFileName(out_meshname)
    data = vtk.vtkUnstructuredGrid() # is also vtkDataSet
    scalars = vtk.vtkDoubleArray()
    points = vtk.vtkPoints()
    i = 0
    for meshentry in mesh:
        coords = meshentry[:3]
        values = meshentry[3:]
        points.InsertPoint(i, coords)
        scalars.InsertTuple1(i, 1)
        i += 1
    pointData = data.GetPointData()
    pointData.SetScalars(scalars)
    data.SetPoints(points)
    writer = vtk.vtkUnstructuredGridWriter()
    writer.SetFileName(out_meshname)
    writer.SetInputData(data)
    writer.Write()

def parse_args():
    parser = argparse.ArgumentParser(description="Read vtk meshes, partition them and write them out in internal format")
    parser.add_argument("in_meshname", metavar="inputmesh", help="The vtk mesh used as input")
    parser.add_argument("--out", "-o", dest="out_meshname", help="The output mesh directory name")
    parser.add_argument("--numparts", "-n", dest="numparts", type=int, help="The number of parts to split into")
    parser.add_argument("--valuetag", "-t", dest="valuetag", help="The Tag used for the mesh point values inside the vtk file")
    return parser.parse_args()
if __name__ == "__main__":
    main()
