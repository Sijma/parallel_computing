#!/bin/bash

process_amount=$1
array_size=$2

sum=0

# Execute the program 10 times
for i in {1..10}; do
    # Store the output of the program in a variable
    output=$(mpirun -np $process_amount ./assignment1 $array_size)
    
    # Add the output to the sum and increment the count
	sum=$(echo $sum + $output | bc)
done

# Calculate the average
average=$(echo $sum / 10 | bc -l)

# Format the average with 2 decimal precision
formatted_average=$(printf "%.2f" $average)

# Print the average
echo "Average: $formatted_average"
