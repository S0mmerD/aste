#pragma once
#include <mpi.h>
class MPICommunicator 
{
    int _rank;
    int _size;
    MPI_Comm _comm = MPI_COMM_WORLD;
    public:
    MPICommunicator() : MPICommunicator(MPI_COMM_WORLD) {}
    MPICommunicator(MPI_Comm comm)
    {
        int initialized;
        MPI_Initialized(&initialized);
        if (!initialized)
            MPI_Init(0, 0);  //TODO: Fix MPIEnv class member initialization fiasco
        MPI_Comm_size(comm, &_size);
        MPI_Comm_size(comm, &_rank);
    }
    int rank() { return _rank;}
    int size() { return _size;}
};
class MPIEnv{
    MPICommunicator _comm;
    public:
        MPIEnv()
        {
            MPI_Init(0, 0);
        }
        MPIEnv(int& argc, char**& argv)
        {
            MPI_Init(&argc, &argv);
        }
        MPICommunicator comm() {return _comm;}
        int rank() {return _comm.rank();}
        int size() {return _comm.size();}
        ~MPIEnv()
        {
            MPI_Finalize();
        }
};
