#include <stdio.h>
#include "mpi.h"
int main(int argc, char **argv)
{
	int r;
    int v = 42;
	MPI_Init(&argc, &argv);
	char port_name[MPI_MAX_PORT_NAME];
	MPI_Status status;
	MPI_Comm intercomm;
	MPI_Open_port(MPI_INFO_NULL, port_name);
	printf("Portname: %s\n", port_name);
	MPI_Comm_accept(port_name, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm);
    printf("Waiting for the client ...");
	MPI_Recv(&r, 1, MPI_INT, 0, 0, intercomm, &status);
    printf("Client connected: %d\n", r);
    MPI_Send(&v, 1, MPI_INT, r, 0, intercomm);
    printf("Server sent value: %d\n", v);
    MPI_Recv(&v, 1, MPI_INT, 0, 0, intercomm, &status);
    printf("Server got value: %d\n", v);
	MPI_Comm_free(&intercomm);
	MPI_Close_port(port_name);
	MPI_Finalize();
	return 0;
}