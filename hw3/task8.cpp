#include <iostream>
#include <mpi.h>
#include <vector>
#include <algorithm>

using namespace std;

int const NSIZE = 1000000;
int const NDELTA = 10;
int const NTIMES = 1000;

vector<int> generate_random_vector(int n) {
    srand(unsigned(time(nullptr)));
    vector<int> v(n);
    generate(v.begin(), v.end(), std::rand);
    return v;
}

int main(int argc, char **argv) {
    int rank, size, ibuf;
    double time_start, time_finish;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        for (int i = 1; i < NSIZE; i *= NDELTA) {
            vector<int> msg = generate_random_vector(i);
            time_start = MPI_Wtime();
            for (int j = 0; j < NTIMES; ++j) {
//                cout << "start sending to process 1 size " << i << '\n';
                MPI_Send(&msg[0], i, MPI_INT, 1, 0, MPI_COMM_WORLD);
//                cout << "finish sending to process 1 size " << i << '\n';
//                cout << "start receiving from process 1 size " << i << '\n';
                MPI_Recv(&msg[0], i, MPI_INT, 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
//                cout << "finish receiving from process 1 size " << i << '\n';
            }
            time_finish = MPI_Wtime();
            double T = (time_finish - time_start);
            cout << i << ':' << T / NTIMES << '\n';
            double L = i * sizeof(int);
            cout << i << ':' << 2 * L * NTIMES / T << '\n';
        }
    }
    if (rank == 1) {
        for (int i = 1; i < NSIZE; i *= NDELTA) {
            vector<int> msg(i);
            for (int j = 0; j < NTIMES; ++j) {
//                cout << "start receiving from process 0 size " << i << '\n';
                MPI_Recv(&msg[0], i, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
//                cout << "finish receiving from process 0 size " << i << '\n';
//                cout << "start sending to process 0 size " << i << '\n';
                MPI_Send(&msg[0], i, MPI_INT, 0, 1, MPI_COMM_WORLD);
//                cout << "finish sending to process 0 size " << i << '\n';
            }
        }
    }
    MPI_Finalize();
}