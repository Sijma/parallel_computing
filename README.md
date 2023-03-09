# Parallel Sorting Algorithm using Odd-Even Transposition Sort

This is an implementation of the Odd-Even Transposition Sort parallel sorting algorithm using MPI. This algorithm can sort an array of positive integers in parallel, with the array being split across multiple processes using MPI.

## Getting Started

To get started with this project, simply clone this repository to your local machine.

### Prerequisites

In order to run this project, you must have MPI installed on your system. 


### Installation

1. Clone the repository to your local machine.
```
git clone https://github.com/Sijma/parallel_computing
```

2. Compile the C program using mpicc
```
mpicc assignment1.c -o assignment1
```

## Usage

The `assignment1` program takes one argument: the size of the array to sort. It can be run using the following command:
```
mpirun -np <process_amount> ./assignment1 <array_size>
```  
  
    
      
        
        
To run the program 10 times and get the average runtime, use the `run_experiment.sh` script. This script takes two arguments: the number of processes to use and the size of the array to sort. It can be run using the following command:
```
./run_experiment.sh <process_amount> <array_size>
```
`run_experiment.sh` Assumes the output executable is named assignment1.
