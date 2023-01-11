#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <string.h>

#include <stdbool.h>

#define ARRAY_SIZE 60000  // Size of the array
#define ARRAY_VALUE_CAP 100 // Maximum possible value generated

//TODO: REMOVE
void print_array(int* arr, int size) {
  printf("[");
  for (int i = 0; i < size; i++) {
    printf("%d", arr[i]);
    if (i < size - 1) {  // Print a comma after each element except the last one
      printf(", ");
    }
  }
  printf("]\n");
}

// Function to compare two integer arrays
bool compare_arrays(int* a, int* b, int size) {
    bool arrays_equal = true;
    for (int i = 0; i < size; i++) {
        if (a[i] != b[i]) {
            arrays_equal = false;
            break;
        }
    }
    return arrays_equal;
}

// Comparison function for qsort
int compare(const void *a, const void *b) {
  // Cast the void pointers to int pointers and dereference them
  int x = *((int*) a);
  int y = *((int*) b);

  // Returns a negative value if x < y, a positive value if x > y, and 0 if x == y
  return x - y;
}

// Function to sort an array of positive integers using qsort
void sort_array(int* array, int size) {
	qsort(array, size, sizeof(int), compare);
}

void merge_bottom(int* result, int* a, int* b, int size) {
    int i = 0;
    int j = 0;
    int k = 0;

    // Merge the elements from the bottom of a and b into result
    while (k < size) {
        if (a[i] < b[j]) {
            result[k] = a[i];
            i++;
        } else {
            result[k] = b[j];
            j++;
        }
        k++;
    }
}

void merge_top(int* result, int* a, int* b, int size) {
    int i = size - 1;
    int j = size - 1;
    int k = size - 1;

    // Merge the elements from the top of a and b into result
    while (k >= 0) {
        if (a[i] > b[j]) {
            result[k] = a[i];
            i--;
        } else {
            result[k] = b[j];
            j--;
        }
        k--;
    }
}

int compute_partner(int phase, int my_rank, int size){
	int partner;
	if (phase % 2 == 0){
		if (my_rank % 2 != 0){
			partner = my_rank - 1;
		} else {
			partner = my_rank + 1;
		}
	} else {
		if (my_rank % 2 != 0){
			partner = my_rank + 1;
		} else {
			partner = my_rank - 1;
		}
	}
	if (partner == -1 || partner == size){
		partner = MPI_PROC_NULL;
	}
	
	return partner;
}

int main(int argc, char* argv[]) {
	clock_t start, end;
    double elapsed;

    // Start the timer
    start = clock();

    int rank, num_procs;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    
    int local_array_size = ARRAY_SIZE/num_procs;
    printf("local: %d\n",local_array_size);
    int A[ARRAY_SIZE];
    
	if (rank == 0){
		// Seed the random number generator using current time
		srand(time(NULL));
		
		for (int i = 0; i < ARRAY_SIZE; i++) {
			A[i] = rand()%ARRAY_VALUE_CAP;
		}
	}
	
	printf("1\n");
	
    // Scatter the arrays to the different processes
    int my_A[local_array_size];
    int receive_A[local_array_size];
    int merged_array[local_array_size];
    MPI_Scatter(A, local_array_size, MPI_INT, my_A, local_array_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Sort the received array using sort_array
    sort_array(my_A, local_array_size);

    
    int partner;
    
    printf("2\n");
    
    // Perform the odd-even transposition steps
	for (int phase = 0; phase < num_procs-1; phase++) {
		printf("3\n");
		partner = compute_partner(phase, rank, num_procs);
		printf("4\n");
		printf("self: %d\n", rank);
		printf("partner: %d\n", partner);
		if (partner != -2){
			MPI_Send(my_A, local_array_size, MPI_INT, partner, 0, MPI_COMM_WORLD);
			printf("5\n");
			MPI_Recv(receive_A, local_array_size, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("6\n");
			if (rank < partner){
				merge_bottom(merged_array, my_A, receive_A, local_array_size);
				memcpy(my_A, merged_array, local_array_size*sizeof(int));
			} else {
				merge_top(merged_array, my_A, receive_A, local_array_size);
				memcpy(my_A, merged_array, local_array_size*sizeof(int));
			}
		}
		printf("7\n");
    }
    
    int final_array[ARRAY_SIZE];
    MPI_Gather(my_A, local_array_size, MPI_INT, final_array, local_array_size, MPI_INT, 0, MPI_COMM_WORLD);
    
    
    // Print the sorted data
    /*
	if (rank == 0) {
		print_array(final_array, ARRAY_SIZE);
		
		// Stop the timer
		end = clock();

		// Calculate the elapsed time in seconds
		elapsed = (double)(end - start) / (CLOCKS_PER_SEC / 1000);
		printf("Elapsed time: %f miliseconds\n", elapsed);
    }
    */
    
    MPI_Finalize();
    
    print_array(final_array, ARRAY_SIZE);
		
	// Stop the timer
	end = clock();

	// Calculate the elapsed time in seconds
	elapsed = (double)(end - start) / (CLOCKS_PER_SEC / 1000);
	printf("Elapsed time: %f miliseconds\n", elapsed);
    
    return 0;
}
