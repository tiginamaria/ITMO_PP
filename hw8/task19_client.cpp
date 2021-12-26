#include <stdio.h>
#include <string.h>
#include "mpi.h"
int main(int argc, char **argv)
{
	int rank;
    int v = 25;
	MPI_Init(&argc, &argv);
	char port_name[MPI_MAX_PORT_NAME];
    MPI_Status status;
	MPI_Comm intercomm;
	strcpy(port_name, argv[1]);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    printf("Attempt to connect\n");
	MPI_Comm_connect(port_name, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm);
    printf("Server connection\n");
	MPI_Send(&rank, 1, MPI_INT, 0, 0, intercomm);
    MPI_Send(&v, 1, MPI_INT, 0, 0, intercomm);
    printf("Client sent value: 25\n");
    MPI_Recv(&v, 1, MPI_INT, 0, 0, intercomm, &status);
    printf("Client get value: %d\n", v);
	MPI_Finalize();
	return 0;
}