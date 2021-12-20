#include <iostream>
#include <mpi.h>
#include <vector>

using namespace std;

int const NSIZE = 1000000;

int main(int argc, char **argv) {
    int rank, size, buf;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        int batch_size = NSIZE / (size - 1);
        cout << "batch_size: " << batch_size << '\n';

        cout << "init vectors" << '\n';
        vector<int> a(NSIZE, 1);
        vector<int> b(NSIZE, 1);

        for (int i = 1; i < size; ++i) {
            if (i == size - 1) {
                batch_size = a.size() - (i - 1) * batch_size;
                cout << "last batch_size: " << batch_size << '\n';
            }
            cout << "start sending to process " << i << '\n';
            MPI_Send(&batch_size, 1, MPI_INT, i, i, MPI_COMM_WORLD);
            cout << "from " << (i - 1) * batch_size << " to " << i * batch_size <<'\n';
            MPI_Send(&a[(i - 1) * batch_size], batch_size, MPI_INT, i, i, MPI_COMM_WORLD);
            MPI_Send(&b[(i - 1) * batch_size], batch_size, MPI_INT, i, i, MPI_COMM_WORLD);
            cout << "finish sending to process " << i << '\n';
        }
        int ans = 0;
        for (int i = 1; i < size; ++i) {
            MPI_Recv(&buf, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            cout << "start receiving from process " << status.MPI_TAG << '\n';
            cout << "receiving value " << buf << '\n';
            ans += buf;
            cout << "finish receiving from process " << status.MPI_TAG << '\n';
        }
        cout << "product is " << ans << '\n';
    } else {
        int ans = 0;
        int batch_size;
        vector<int> a, b;

        cout << "start processing process " << rank << '\n';
        MPI_Recv(&batch_size, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        a.resize(batch_size);
        b.resize(batch_size);
        MPI_Recv(&a[0], batch_size, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(&b[0], batch_size, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        cout << "finish processing process " << rank << '\n';
        for (int i = 0; i < batch_size; ++i) {
            ans += a[i] * b[i];
        }
        cout << "start sending results " << ans << '\n';
        MPI_Send(&ans, 1, MPI_INT, 0, rank, MPI_COMM_WORLD);
        cout << "finish sending results" << '\n';
    }
    MPI_Finalize();
}