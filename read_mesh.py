import vtk
import os
import numpy as np
from ctypes import *
from tqdm import tqdm
import argparse


def main():
    args = parse_args()
    mesh = read_mesh(args.in_meshname)
    numparts = args.numparts
    part = partition(mesh, numparts)
    meshes = generate_meshes(mesh, part, numparts, "Colors")
    if not args.out_meshname:
        out_meshname = args.in_meshname[:-4]
        print("No --out given. Setting output to: " + out_meshname)
    else:
        out_meshname = args.out_meshname
    write_meshes(meshes, out_meshname)

def read_mesh(filename):
    result = []
    reader = vtk.vtkDataSetReader()
    reader.SetFileName(filename)
    reader.Update()
    return reader.GetOutput()

def partition(mesh, numparts):
    cellPtr = [0]
    cellData = []
    for i in tqdm(range(mesh.GetNumberOfCells())):
        cell = mesh.GetCell(i)
        for j in range(cell.GetNumberOfPoints()):
            cellData.append(cell.GetPointId(j))
        cellPtr.append(cellPtr[-1] + cell.GetNumberOfPoints())
    libmetis = cdll.LoadLibrary(os.path.abspath("libmetisAPI.so"))
    idx_t = c_int if libmetis.typewidth() == 32 else c_longlong
    cell_count = idx_t(mesh.GetNumberOfCells())
    point_count = idx_t(mesh.GetNumberOfPoints())
    num_parts = idx_t(numparts)
    partition = (idx_t * mesh.GetNumberOfPoints())()
    cell_ptr = (idx_t * len(cellPtr))(*cellPtr)
    cell_data = (idx_t * len(cellData))(*cellData)
    libmetis.partitionMetis(cell_count, point_count, cell_ptr, cell_data, num_parts, partition)
    arr = np.ctypeslib.as_array(partition)
    return arr

def generate_meshes(mesh, part, numparts, tag = None):
    meshes = [[] for _ in range(numparts)]
    fieldData = mesh.GetPointData()
    dataArray = fieldData.GetArray(tag)
    for i in range(mesh.GetNumberOfPoints()):
        selected = meshes[part[i]]
        entry = ()
        entry += (mesh.GetPoint(i))
        if dataArray:
            entry += dataArray.GetTuple(i) 
        selected.append(entry)
    return meshes

def write_meshes(meshes, dirname):
    dirname = os.path.abspath(dirname)
    if not os.path.exists(dirname):
        os.mkdir(dirname)
    for i in range(len(meshes)):
        mesh = meshes[i]
        with open(dirname + "/" + str(i), "w+") as file:
            for entry in mesh:
                file.write(" ".join(map(str, entry)) + "\n")
def parse_args():
    parser = argparse.ArgumentParser(description="Read vtk meshes, partition them and write them out in internal format")
    parser.add_argument("in_meshname", metavar="inputmesh", help="The vtk mesh used as input")
    parser.add_argument("--out", "-o", dest="out_meshname", help="The output mesh directory name")
    parser.add_argument("--numparts", "-n", dest="numparts", type=int, help="The number of parts to split into")
    return parser.parse_args()

if __name__ == "__main__":
    main()
