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
        MPI_Comm_size(comm, &_size);
        MPI_Comm_rank(comm, &_rank);
    }
    int rank() { return _rank;}
    int size() { return _size;}
    MPI_Comm comm() {return _comm;}
};

class MPIEnv{
    std::shared_ptr<MPICommunicator> _comm;
    public:
        MPIEnv()
        {
            MPI_Init(0, 0);
            _comm = std::make_shared<MPICommunicator>();
        }
        MPIEnv(int& argc, char**& argv)
        {
            MPI_Init(&argc, &argv);
            _comm = std::make_shared<MPICommunicator>();
        }
        MPIEnv(const MPIEnv& other) =delete;
        MPIEnv& operator=(const MPIEnv& other) =delete;
        std::shared_ptr<MPICommunicator> comm() {return _comm;}
        int rank() {return _comm->rank();}
        int size() {return _comm->size();}
        ~MPIEnv()
        {
            MPI_Barrier(MPI_COMM_WORLD);
            MPI_Finalize();
        }
};
