#include <iostream>
#include "mpi.h"

using namespace std;

int main(int argc, char **argv) {
    int rank, size, prev, next;
    int rbuf[2];
    int sbuf[2];
    MPI_Init(&argc, &argv);
    MPI_Request reqs[4];
    MPI_Status stats[4];
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    prev = rank - 1;
    next = rank + 1;
    if (rank == 0) prev = size - 1;
    if (rank == size - 1) next = 0;

    sbuf[0] = rank;
    sbuf[1] = rank;

    MPI_Recv_init(&rbuf[0], 1, MPI_INT, prev, 5, MPI_COMM_WORLD, &reqs[0]);
    MPI_Recv_init(&rbuf[1], 1, MPI_INT, next, 6, MPI_COMM_WORLD, &reqs[1]);
    MPI_Send_init(&sbuf[0], 1, MPI_INT, prev, 6, MPI_COMM_WORLD, &reqs[2]);
    MPI_Send_init(&sbuf[1], 1, MPI_INT, next, 5, MPI_COMM_WORLD, &reqs[3]);

    for (int i = 0; i < size; i++) {
        if (i == prev) {
            MPI_Start(&reqs[0]);
            MPI_Wait(&reqs[0], &stats[0]);
        }

        if (i == rank) {
            MPI_Start(&reqs[2]);
            MPI_Start(&reqs[3]);

            MPI_Wait(&reqs[2], &stats[2]);
            MPI_Wait(&reqs[3], &stats[3]);
        }

        if (i == next) {
            MPI_Start(&reqs[1]);
            MPI_Wait(&reqs[1], &stats[1]);
        }
    }

    MPI_Waitall(4, reqs, stats);

    cout << "process " << rank << " from process " << prev << " got " << rbuf[0] << '\n';
    cout << "process " << rank << " from process " << next << " got " << rbuf[1] << '\n';

    MPI_Finalize();
}