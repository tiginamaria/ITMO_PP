#include <iostream>
#include <mpi.h>
#include <vector>

using namespace std;

int const NSIZE = 1000000;

int main(int argc, char **argv) {
    int rank, size, sum, tmp_sum;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int batch_size;
    vector<int> a;

    if (rank == 0) {
        cout << "batch_size: " << batch_size << '\n';
        batch_size = NSIZE / size;
        cout << "init vectors" << '\n';
        a.resize(NSIZE, 1);

        for (int i = 1; i < size; ++i) {
            if (i == size) {
                batch_size = a.size() - i * batch_size;
                cout << "last batch_size: " << batch_size << '\n';
            }
            cout << "start sending to process " << i << '\n';
            MPI_Send(&batch_size, 1, MPI_INT, i, i, MPI_COMM_WORLD);
            cout << "from " << i * batch_size << " to " << (i + 1) * batch_size <<'\n';
            MPI_Send(&a[i * batch_size], batch_size, MPI_INT, i, i, MPI_COMM_WORLD);
            cout << "finish sending to process " << i << '\n';
        }
    }

    if (rank == 0) {
        batch_size = NSIZE / size;
        a.resize(batch_size);
    } else {
        cout << "process " << rank << " get init vector" << '\n';
        MPI_Recv(&batch_size, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        a.resize(batch_size);
        MPI_Recv(&a[0], batch_size, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    }

    for (int i = 0; i < batch_size; ++i) {
        sum += a[i];
    }

    for (int i = 1; i < NSIZE; i *= 2) {
        if (rank % (2 * i) == 0 && rank + i < size) {
            cout << "process " << rank << " receiving from process " << rank + i << '\n';
            MPI_Recv(&tmp_sum, 1, MPI_INT, rank + i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            sum += tmp_sum;
            cout << "process " << rank << " successfully received from process " << rank + i << '\n';
        } else if (rank % (2 * i) == i) {
            cout << "process " << rank << " sending to process " << rank - i << '\n';
            MPI_Send(&sum, 1, MPI_INT, rank - i, 0, MPI_COMM_WORLD);
            cout << "process " << rank << " successfully sent to process " << rank + i << '\n';
        }
    }

    if (rank == 0) {
        cout << "sum is " << sum << '\n';
    }

    MPI_Finalize();
}