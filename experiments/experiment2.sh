#!/bin/bash

# Clear the contents of the results
#> results/experiment1.txt

echo "Starting the experiment 2" | tee -a results/experiment2.txt



output=$(../build-release/Sort --random 8 --poly_degree 119)    
    
output="$output, inputs: 8"
    
echo "$output" | tee -a results/experiment2.txt

output=$(../build-release/Sort --random 16 --poly_degree 119)

output="$output, inputs: 16"

echo "$output" | tee -a results/experiment2.txt

output=$(../build-release/Sort --random 32 --poly_degree 119)

output="$output, inputs: 32"

echo "$output" | tee -a results/experiment2.txt

output=$(../build-release/Sort --random 64 --poly_degree 119)

output="$output, inputs: 64"

echo "$output" | tee -a results/experiment2.txt

output=$(../build-release/Sort --random 128 --poly_degree 119)

output="$output, inputs: 128"

echo "$output" | tee -a results/experiment2.txt

output=$(../build-release/Sort --random 256 --poly_degree 119)

output="$output, inputs: 256"

echo "$output" | tee -a results/experiment2.txt

output=$(../build-release/Sort --random 512 --poly_degree 119)

output="$output, inputs: 512"

echo "$output" | tee -a results/experiment2.txt

output=$(../build-release/Sort --random 1024 --poly_degree 119)

output="$output, inputs: 1024"

echo "$output" | tee -a results/experiment2.txt