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
<br />

To run the program 10 times and get the average runtime, use the `run_experiment.sh` script. This script takes two arguments: the number of processes to use and the size of the array to sort. It can be run using the following command:

```
./run_experiment.sh <process_amount> <array_size>
```
<br />
`run_experiment.sh` Assumes the output executable is named assignment1.

## Results

The program will output the sorted array and the time taken to sort the array in milliseconds. The `run_experiment.sh` script will output the average time taken to sort the array over 10 runs.

## Acknowledgements

This project was completed as part of a course on parallel computing at Harokopio University.

## Screenshots
### Time to Sort
![Εικόνα2](https://user-images.githubusercontent.com/39009079/224070639-9f1b7317-2640-4cd5-9c42-1310fb37e6a0.png)

### Acceleration vs 1 process
![Εικόνα3](https://user-images.githubusercontent.com/39009079/224070849-bfa87121-b8ad-45ef-a1c4-74c906fd9dd0.png)

### Process efficiency
![Εικόνα4](https://user-images.githubusercontent.com/39009079/224070970-819d5c9b-f801-4501-8f61-c94116ebf7d4.png)
