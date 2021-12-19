#include <iostream>
#include "mpi.h"
#define NTIMES 100
using namespace std;
int main(int argc, char **argv)
{
    double time_start, time_finish;
    int len, rank, n, i, message;
    char *name = new char[100];
    MPI_Init(&argc, &argv);
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &n);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Get_processor_name(name, &len);
    time_start = MPI_Wtime();
    for (i = 0; i < NTIMES; i++) {
        if (rank == 1) {
            // cout << "Hello from process " << rank << "\n";
            for (i = 0; i < n; i++) {
                if (i == rank) {
                    continue;
                }
                MPI_Recv(&message, 1, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                // cout << "Hello from process " << message << endl;
            }
        } else MPI_Send(&rank, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    }
    time_finish = MPI_Wtime();
    cout << "processor " << name << ", process " << rank << "time = " << (time_finish - time_start) / NTIMES << endl;
    MPI_Finalize();
}
