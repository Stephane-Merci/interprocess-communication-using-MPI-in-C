// These are standard C library headers and the MPI header needed for MPI functionality.
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

// This macro defines the size of the array, which will be split among MPI processes.
#define ARRAY_SIZE 1000

int main(int argc, char *argv[]) {
    int rank, size;
    int local_sum = 0, total_sum = 0;
    int *data;

    // MPI is initialized using MPI_Init. MPI_Comm_rank and MPI_Comm_size are used to get
    // the rank (ID) and size (number of processes) of the MPI communicator (MPI_COMM_WORLD).
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Master process initializes and distributes data
    // The master process (rank 0) allocates memory for an integer array (data) and initializes it with values from 1 to ARRAY_SIZE.
    if (rank == 0) {
        data = (int *)malloc(ARRAY_SIZE * sizeof(int));
        for (int i = 0; i < ARRAY_SIZE; i++) {
            data[i] = i + 1;  // Initialize array with values 1, 2, ..., ARRAY_SIZE
        }
    }

    // Broadcast the data to all processes
    // The master process broadcasts the data array to all other processes using MPI_Bcast.
    MPI_Bcast(data, ARRAY_SIZE, MPI_INT, 0, MPI_COMM_WORLD);

    // Calculate the local sum for each process
    // Each process calculates a local sum by processing a portion of the data array based on its rank.
    int chunk_size = ARRAY_SIZE / size;
    int start = rank * chunk_size;
    int end = (rank == size - 1) ? ARRAY_SIZE : start + chunk_size;

    for (int i = start; i < end; i++) {
        local_sum += data[i];
    }

    // Sum the local sums to get the total sum
    // Use MPI_Reduce to sum up the local sums from all processes and store the result in total_sum in the master process (rank 0).
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Master process prints the result
    // In the master process, print the total sum calculated from all processes.
    if (rank == 0) {
        printf("Total sum: %d\n", total_sum);
    }

    // Clean up and finalize MPI
    // In the master process, free the memory allocated for the array, and finalize MPI to clean up resources.
    if (rank == 0) {
        free(data);
    }

    MPI_Finalize();

    return 0;
}

// This program demonstrates a simple distributed computation using MPI,
// where the task of summing up an array is split among multiple processes.
// The master process initializes the data, broadcasts it to all processes,
// and then each process calculates the sum of its assigned portion.
// Finally, the master process collects and prints the total sum.

