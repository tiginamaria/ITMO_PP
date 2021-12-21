#include <iostream>
#include "mpi.h"

using namespace std;

int main(int argc, char **argv) {
    int rank, size, prev, next;
    int buf[2];
    MPI_Init(&argc, &argv);
    MPI_Request reqs[4];
    MPI_Status stats[4];
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    prev = rank - 1;
    next = rank + 1;
    if (rank == 0) prev = size - 1;
    if (rank == size - 1) next = 0;

    MPI_Sendrecv(&rank, 1, MPI_INT, next, 5, &buf[0], 1, MPI_INT, prev, 5, MPI_COMM_WORLD, &stats[0]);
    MPI_Sendrecv(&rank, 1, MPI_INT, prev, 5, &buf[1], 1, MPI_INT, next, 5, MPI_COMM_WORLD, &stats[1]);

    cout << "process " << rank << " from process " << prev << " got " << buf[0] << '\n';
    cout << "process " << rank << " from process " << next << " got " << buf[1] << '\n';
    MPI_Finalize();
}