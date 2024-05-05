#!/bin/bash

# Clear the contents of the results
#> results/experiment1.txt

echo "Starting the experiment 2" | tee -a results/experiment2.txt

# n = 8 
output=$(../build-release/Sort --random 8 --poly_degree 120 --toy_parameters)    
    
output="$output, NOTHING - inputs: 8"
    
echo "$output" | tee -a results/experiment2.txt

output=$(../build-release/Sort --random 8 --poly_degree 120 --error_correction --toy_parameters)    
    
output="$output, ERRORCORR - inputs: 8"
    
echo "$output" | tee -a results/experiment2.txt

output=$(../build-release/Sort --random 8 --poly_degree 120 --scaling_factor 1.1 --toy_parameters)    
    
output="$output, SCALING - inputs: 8"
    
echo "$output" | tee -a results/experiment2.txt


# n = 16

output=$(../build-release/Sort --random 16 --poly_degree 120 --toy_parameters)    
    
output="$output, NOTHING - inputs: 16"
    
echo "$output" | tee -a results/experiment2.txt

output=$(../build-release/Sort --random 16 --poly_degree 120 --error_correction --toy_parameters)    
    
output="$output, ERRORCORR - inputs: 16"
    
echo "$output" | tee -a results/experiment2.txt

output=$(../build-release/Sort --random 16 --poly_degree 120 --scaling_factor 1.1 --toy_parameters)    
    
output="$output, SCALING - inputs: 16"
    
echo "$output" | tee -a results/experiment2.txt

# n = 32

output=$(../build-release/Sort --random 32 --poly_degree 120 --toy_parameters)    
    
output="$output, NOTHING - inputs: 32"
    
echo "$output" | tee -a results/experiment2.txt

output=$(../build-release/Sort --random 32 --poly_degree 120 --error_correction --toy_parameters)    
    
output="$output, ERRORCORR - inputs: 32"
    
echo "$output" | tee -a results/experiment2.txt

output=$(../build-release/Sort --random 32 --poly_degree 120 --scaling_factor 1.1 --toy_parameters)    
    
output="$output, SCALING - inputs: 32"
    
echo "$output" | tee -a results/experiment2.txt

# n = 64

output=$(../build-release/Sort --random 64 --poly_degree 120 --toy_parameters)    
    
output="$output, NOTHING - inputs: 64"
    
echo "$output" | tee -a results/experiment2.txt

output=$(../build-release/Sort --random 64 --poly_degree 120 --error_correction --toy_parameters)    
    
output="$output, ERRORCORR - inputs: 64"
    
echo "$output" | tee -a results/experiment2.txt

output=$(../build-release/Sort --random 64 --poly_degree 120 --scaling_factor 1.1 --toy_parameters)    
    
output="$output, SCALING - inputs: 64"
    
echo "$output" | tee -a results/experiment2.txt

# n = 128

output=$(../build-release/Sort --random 128 --poly_degree 120 --toy_parameters)    
    
output="$output, NOTHING - inputs: 128"
    
echo "$output" | tee -a results/experiment2.txt

output=$(../build-release/Sort --random 128 --poly_degree 120 --error_correction --toy_parameters)    
    
output="$output, ERRORCORR - inputs: 128"
    
echo "$output" | tee -a results/experiment2.txt

output=$(../build-release/Sort --random 128 --poly_degree 120 --scaling_factor 1.1 --toy_parameters)    
    
output="$output, SCALING - inputs: 128"
    
echo "$output" | tee -a results/experiment2.txt

# n = 256 

output=$(../build-release/Sort --random 256 --poly_degree 120 --toy_parameters)    
    
output="$output, NOTHING - inputs: 256"
    
echo "$output" | tee -a results/experiment2.txt

output=$(../build-release/Sort --random 256 --poly_degree 120 --error_correction --toy_parameters)    
    
output="$output, ERRORCORR - inputs: 256"
    
echo "$output" | tee -a results/experiment2.txt

output=$(../build-release/Sort --random 256 --poly_degree 120 --scaling_factor 1.1 --toy_parameters)    
    
output="$output, SCALING - inputs: 256"
    
echo "$output" | tee -a results/experiment2.txt

# n = 512

output=$(../build-release/Sort --random 512 --poly_degree 120 --toy_parameters)    
    
output="$output, NOTHING - inputs: 512"
    
echo "$output" | tee -a results/experiment2.txt

output=$(../build-release/Sort --random 512 --poly_degree 120 --error_correction --toy_parameters)    
    
output="$output, ERRORCORR - inputs: 512"
    
echo "$output" | tee -a results/experiment2.txt

output=$(../build-release/Sort --random 512 --poly_degree 120 --scaling_factor 1.1 --toy_parameters)    
    
output="$output, SCALING - inputs: 512"
    
echo "$output" | tee -a results/experiment2.txt