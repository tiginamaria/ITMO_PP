#include <iostream>
#include <vector>
#include "mpi.h"

using namespace std;

int const NSIZE = 500;
int const NTIMES = 10;

vector< vector<int> > generate_random_matrix(int n) {
    srand(unsigned(time(nullptr)));
    vector< vector<int> > v;

    for (int i = 0; i < n; ++i) {
        vector<int> row(n);
        generate(row.begin(), row.end(), std::rand);
        v.push_back(row);
    }

    return v;
}

vector< vector<int> > generate_zero_matrix(int n) {
    vector< vector<int> > v;

    for (int i = 0; i < n; ++i) {
        vector<int> row(n, 0);
        v.push_back(row);
    }

    return v;
}


vector< vector<int> > multiply_matrix(int n, vector< vector<int> >& a, vector< vector<int> >& b) {
    vector< vector<int> > c = generate_zero_matrix(n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < n; ++k) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return c;
}

int main(int argc, char **argv) {
    int rank, size;
    double time_start, time_finish;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    vector< vector<int> > a = generate_random_matrix(NSIZE);
    vector< vector<int> > b = generate_random_matrix(NSIZE);

    MPI_Barrier(MPI_COMM_WORLD);

    time_start = MPI_Wtime();

    for (int t = 0; t < NTIMES; ++t) {
        multiply_matrix(NSIZE, a, b);
    }

    time_finish = MPI_Wtime();
    cout << rank << ' ' << (time_finish - time_start) / NTIMES << '\n';

    MPI_Finalize();
}