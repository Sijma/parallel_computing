#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <string.h>

#include<unistd.h>

#include <stdbool.h>

#define ARRAY_VALUE_CAP 100 // Maximum possible value generated

//TODO: REMOVE
void print_array(unsigned long* arr, int size) {
  printf("[");
  for (int i = 0; i < size; i++) {
    printf("%ld", arr[i]);
    if (i < size - 1) {  // Print a comma after each element except the last one
      printf(", ");
    }
  }
  printf("]\n");
}

// Function to compare two integer arrays
bool compare_arrays(unsigned long* a, unsigned long* b, int size) {
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
void sort_array(unsigned long* array, long size) {
	qsort(array, size, sizeof(unsigned long), compare);
}

void merge_bottom(unsigned long* result, unsigned long* a, unsigned long* b, long size) {
    long i = 0;
    long j = 0;
    long k = 0;

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

void merge_top(unsigned long* result, unsigned long* a, unsigned long* b, long size) {
    long i = size - 1;
    long j = size - 1;
    long k = size - 1;

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
	if(argc != 2){
		printf("Please provide only a single argument, for the size of the array.\n");
	}
	
	char *a = argv[1];
  	long ARRAY_SIZE = atol(a);
	
	clock_t start, end;
    double elapsed;

    // Start the timer
    start = clock();

    int rank, num_procs;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    
    long local_array_size = ARRAY_SIZE/num_procs;
    unsigned long *A = ( unsigned long *)malloc(ARRAY_SIZE * sizeof( unsigned long));
    
    
	if (rank == 0){
		// Seed the random number generator using current time
		srand(time(NULL));
		
		for (long i = 0; i < ARRAY_SIZE; i++) {
			A[i] = rand()%ARRAY_VALUE_CAP;
		}
	}
	
    // Scatter the arrays to the different processes
	unsigned long *my_A = ( unsigned long *)malloc(local_array_size * sizeof( unsigned long));
	unsigned long *receive_A = ( unsigned long *)malloc(local_array_size * sizeof( unsigned long));
    
	MPI_Scatter(A, local_array_size, MPI_UNSIGNED_LONG, my_A, local_array_size, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);
    
	unsigned long *merged_array = ( unsigned long *)malloc(local_array_size * sizeof( unsigned long));

    // Sort the received array using sort_array
    sort_array(my_A, local_array_size);

    int partner;
    
    // Perform the odd-even transposition steps
	for (int phase = 0; phase < num_procs; phase++) {
		partner = compute_partner(phase, rank, num_procs);
		if (partner != -2){
			MPI_Sendrecv(my_A, local_array_size, MPI_UNSIGNED_LONG, partner, 0, receive_A, local_array_size, MPI_UNSIGNED_LONG, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			if (rank < partner){
				merge_bottom(merged_array, my_A, receive_A, local_array_size);
				memcpy(my_A, merged_array, local_array_size*sizeof( unsigned long));
			} else {
				merge_top(merged_array, my_A, receive_A, local_array_size);
				memcpy(my_A, merged_array, local_array_size*sizeof( unsigned long));
			}
		}
    }
    unsigned long *final_array = ( unsigned long *)malloc(ARRAY_SIZE * sizeof( unsigned long));
    
	MPI_Gather(my_A, local_array_size, MPI_UNSIGNED_LONG, final_array, local_array_size, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);
	
	if(rank == 0){
		//print_array(final_array, ARRAY_SIZE);
		// Stop the timer
		end = clock();

		// Calculate the elapsed time in seconds
		elapsed = (double)(end - start) / (CLOCKS_PER_SEC / 1000);
		printf("%.2f\n", elapsed);
    }
    
    MPI_Finalize();
    
    return 0;
}
