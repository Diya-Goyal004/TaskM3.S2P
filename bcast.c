#include <mpi.h>  // MPI library for parallel communication
#include <stdio.h>  // Standard I/O functions
#include <string.h>  // String manipulation functions

int main(int argc, char *argv[]) {
    int rank, size;  // Variables to store the rank of the process and the total number of processes
    char message[20];  // Buffer to hold the message being broadcasted

    // Initialize the MPI environment
    MPI_Init(&argc, &argv);  // Required initialization for MPI programs

    // Get the rank of the current process in the communicator
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // Determines the unique identifier for each process

    // Get the total number of processes in the communicator
    MPI_Comm_size(MPI_COMM_WORLD, &size);  // Retrieves the total number of processes in the communicator

    // If the current process is the master (rank 0), initialize the message
    if (rank == 0) {
        strcpy(message, "Hello World!");  // The message to be broadcasted to all processes
    }

    // Broadcast the message from the master process (rank 0) to all other processes
    MPI_Bcast(message, 20, MPI_CHAR, 0, MPI_COMM_WORLD);  
    // MPI_Bcast sends a message from the master process (rank 0) to all other processes in the communicator
    // message: the data to be broadcasted
    // 20: the maximum size of the message (including the null terminator)
    // MPI_CHAR: the MPI data type indicating that the message contains characters
    // 0: the rank of the master process from which the broadcast originates
    // MPI_COMM_WORLD: the communicator within which the broadcast occurs

    // Each process, including the master, prints the received message
    printf("Process (rank %d) received message: %s\n", rank, message);  
    // Outputs the message received and the rank of the current process

    // Finalize the MPI environment
    MPI_Finalize();  // Required cleanup to properly release MPI-related resources

    return 0;  // Indicate successful program completion
}
