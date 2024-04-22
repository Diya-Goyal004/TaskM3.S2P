#include <mpi.h>  // MPI library
#include <stdio.h>  // Standard I/O functions
#include <string.h>  // String manipulation functions

int main(int argc, char *argv[]) {
    int rank, size;  // Rank of the process and the total number of processes
    char message[20];  // Buffer for the message
    int tag = 0;  // Arbitrary message tag used in MPI communication

    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    // Get the rank of the current process in the communicator
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Get the total number of processes in the communicator
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // If the current process is the master (rank 0)
    if (rank == 0) {
        strcpy(message, "Hello World!");  // Prepare the message to send

        // Send the message to each worker process (from rank 1 to size - 1)
        for (int i = 1; i < size; i++) {
            MPI_Send(message, strlen(message) + 1, MPI_CHAR, i, tag, MPI_COMM_WORLD);  
            // Send the message (including the null terminator) to the process with the given rank
        }

        // Print a confirmation message for the master process
        printf("Master (rank 0) sent 'Hello World!' to all workers.\n");
    } else {  // If the process is a worker (rank > 0)
        // Receive the message from the master process (rank 0)
        MPI_Recv(message, 20, MPI_CHAR, 0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        // Print the message received along with the rank of the process
        printf("Worker (rank %d) received message: %s\n", rank, message);
    }

    // Finalize the MPI environment
    MPI_Finalize();
    
    return 0;
}
