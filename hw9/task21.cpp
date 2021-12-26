#include <iostream>
#include <algorithm>
#include <stdio.h>
#include "mpi.h"

#define BUFSIZE 100
#define STRSIZE 150

using namespace std;

char get_random_char() {
    const char charset[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
    const size_t max_index = (sizeof(charset) - 1);
    return charset[rand() % max_index];
}

string get_random_string(size_t length) {
    string str(length, 0);
    generate_n(str.begin(), length, get_random_char);
    return str;
}

void create_file(string filename, size_t length) {
    MPI_Status status;
    MPI_File fh;
    MPI_File_open(MPI_COMM_SELF, filename.c_str(), MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);
    string content = get_random_string(length);
    MPI_File_write(fh, content.c_str(), length, MPI_CHAR, &status);
    MPI_File_close(&fh);
}

void delete_file(string filename) {
    MPI_File_delete(filename.c_str(), MPI_INFO_NULL);
}

int main(int argc, char **argv) {
    int rank;
    MPI_Init(&argc, &argv);
    MPI_File fh;
    char buf[BUFSIZE + 1] = {0};
    string filename = "file.txt";
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        printf("create file: %s size:%d\n", filename.c_str(), STRSIZE);
        create_file(filename, STRSIZE);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_File_open(MPI_COMM_WORLD, filename.c_str(), MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
    MPI_File_set_view(fh, rank * BUFSIZE, MPI_CHAR, MPI_CHAR, "native", MPI_INFO_NULL);
    MPI_File_read_all(fh, buf, BUFSIZE, MPI_CHAR, MPI_STATUS_IGNORE);
    printf("process %d, size=%d, buf=%s\n", rank, strlen(buf), buf);

    MPI_File_close(&fh);
    MPI_Finalize();
}