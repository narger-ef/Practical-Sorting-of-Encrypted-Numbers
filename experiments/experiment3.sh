#!/bin/bash

# Clear the contents of the results
#> results/experiment1.txt

echo "Starting the experiment 3" | tee -a results/experiment3.txt

# d = 495 - error correction

output=$(../build-release/Sort --random 1024 --poly_degree 496 --error_correction --toy_parameters) #19.46GB RAM

output="$output, inputs: 1024"

echo "$output" | tee -a results/experiment3.txt

output=$(../build-release/Sort --random 2048 --poly_degree 496 --error_correction --toy_parameters)

output="$output, inputs: 2048"

echo "$output" | tee -a results/experiment3.txt

output=$(../build-release/Sort --random 4096 --poly_degree 496 --error_correction --toy_parameters)

output="$output, inputs: 4096"

# d = 1007 - error correction

output=$(../build-release/Sort --random 1024 --poly_degree 1008 --error_correction --toy_parameters)

output="$output, inputs: 1024"

echo "$output" | tee -a results/experiment3.txt

output=$(../build-release/Sort --random 2048 --poly_degree 1008 --error_correction --toy_parameters)

output="$output, inputs: 2048"

echo "$output" | tee -a results/experiment3.txt

output=$(../build-release/Sort --random 4096 --poly_degree 1008 --error_correction --toy_parameters)

output="$output, inputs: 4096"

echo "$output" | tee -a results/experiment3.txt

echo "Now with scaling" | tee -a results/experiment3.txt

# d = 495 - scaling

output=$(../build-release/Sort --random 1024 --poly_degree 496 --scaling_factor 1.1 --toy_parameters)

output="$output, inputs: 1024"

echo "$output" | tee -a results/experiment3.txt

output=$(../build-release/Sort --random 2048 --poly_degree 496 --scaling_factor 1.1 --toy_parameters)

output="$output, inputs: 2048"

echo "$output" | tee -a results/experiment3.txt

output=$(../build-release/Sort --random 4096 --poly_degree 496 --scaling_factor 1.1 --toy_parameters)

output="$output, inputs: 4096"

echo "$output" | tee -a results/experiment3.txt

# d = 1007 - scaling

output=$(../build-release/Sort --random 1024 --poly_degree 1008 --scaling_factor 1.1 --toy_parameters)

output="$output, inputs: 1024"

echo "$output" | tee -a results/experiment3.txt

output=$(../build-release/Sort --random 2048 --poly_degree 1008 --scaling_factor 1.1 --toy_parameters)

output="$output, inputs: 2048"

echo "$output" | tee -a results/experiment3.txt

output=$(../build-release/Sort --random 4096 --poly_degree 1008 --scaling_factor 1.1 --toy_parameters)

output="$output, inputs: 4096"

echo "$output" | tee -a results/experiment3.txt