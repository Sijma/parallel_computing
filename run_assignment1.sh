#!/bin/bash

# Compile the C program using GCC
mpicc assignment1.c -o assignment1

# Execute the compiled program
mpirun -np 6 ./assignment1

