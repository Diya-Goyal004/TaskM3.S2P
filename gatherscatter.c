#include <chrono>  
#include <cstdlib>  // Standard library functions including memory management
#include <iostream>  // Standard I/O functions
#include <mpi.h>  // MPI library for parallel communication
#include <time.h>  

using namespace std::chrono;  // For duration and timing operations
using namespace std;  // To use standard C++ library functions without explicit std:: prefix

// Function to initialize a vector with random values
void randomVector(int vector[], int size) {
  for (int i = 0; i < size; i++) {
    vector[i] = rand() % 100;  // Random integers between 0 and 99
  }
}

int main(int argc, char* argv[]) {
  // Initialize the MPI environment
  MPI_Init(&argc, &argv);

  int rank, size;  // Variables for process rank and total number of processes
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);  // Get the rank of the current process
  MPI_Comm_size(MPI_COMM_WORLD, &size);  // Get the total number of processes

  // Define the total size of the vectors and the chunk size for each process
  unsigned long total_size = 100000000;  // Total number of elements in the original vectors
  int chunk_size = total_size / size;  // Number of elements for each process

  srand(time(0) + rank);  // Seed random number generator with a unique value for each process

  // Allocate memory for local chunks in each process
  int* local_v1 = (int*)malloc(chunk_size * sizeof(int));
  int* local_v2 = (int*)malloc(chunk_size * sizeof(int));
  int* local_v3 = (int*)malloc(chunk_size * sizeof(int));

  auto start = high_resolution_clock::now();  // Start the timer

  if (rank == 0) {  // Master process
    int* v1 = (int*)malloc(total_size * sizeof(int));  // Full vectors for the master process
    int* v2 = (int*)malloc(total_size * sizeof(int));

    // Initialize the original vectors with random values
    randomVector(v1, total_size);
    randomVector(v2, total_size);

    // Scatter the chunks of v1 and v2 to all processes
    MPI_Scatter(v1, chunk_size, MPI_INT, local_v1, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);  // Scatter v1
    MPI_Scatter(v2, chunk_size, MPI_INT, local_v2, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);  // Scatter v2

    free(v1);  // Free the original vectors in the master process
    free(v2);
  } else {  // Worker processes
    // Receive the scattered chunks from the master process
    MPI_Scatter(nullptr, chunk_size, MPI_INT, local_v1, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);  
    MPI_Scatter(nullptr, chunk_size, MPI_INT, local_v2, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);
  }

  // Perform the addition on the local chunks
  for (int i = 0; i < chunk_size; i++) {
    local_v3[i] = local_v1[i] + local_v2[i];  // Element-wise addition of local chunks
  }

  if (rank == 0) {  // Master process
    int* v3 = (int*)malloc(total_size * sizeof(int));  // Full result vector in the master process
    // Gather the results from all processes
    MPI_Gather(local_v3, chunk_size, MPI_INT, v3, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);  

    auto stop = high_resolution_clock::now();  // Stop the timer
    auto duration = duration_cast<microseconds>(stop - start);  // Calculate the duration

    cout << "Time taken by function: " << duration.count() << " microseconds" << endl;  // Output the time taken

    free(v3);  // Free the result vector in the master process
  } else {  // Worker processes
    // Send the local chunks back to the master process
    MPI_Gather(local_v3, chunk_size, MPI_INT, nullptr, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);  
  }

  // Free the memory allocated for local chunks
  free(local_v1);
  free(local_v2);
  free(local_v3);

  // Finalize the MPI environment
  MPI_Finalize();

  return 0;  // Indicate successful program completion
}
