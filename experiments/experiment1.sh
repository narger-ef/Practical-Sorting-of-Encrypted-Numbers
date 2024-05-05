#!/bin/bash

# Clear the contents of the results
#> results/experiment1.txt

echo "Starting the experiment 1" | tee -a results/experiment1.txt

###### FROM 20 TO 27 #####

output=$(../build-release/Sort --random 8 --poly_degree 20 )    
    
output="$output, degree: 20"
    
echo "$output" | tee -a results/experiment1.txt

for ((d = 20; d <= 28; d++)); do

    # Each execution is tested 10 times

    # The first generates the context

    for ((i = 0; i < 3; i++)); do

        output=$(../build-release/Sort --random 8 --poly_degree $d --load_context)
    
        output="$output, degree: $d"
    
        echo "$output" | tee -a results/experiment1.txt

    done

    echo "Degree $d done" | tee -a results/experiment1.txt

done

###### FROM 28 TO 59 #####

# I recreate the context because d > 28 requires a deeper circuit

output=$(../build-release/Sort --random 8 --poly_degree 29 )    
    
output="$output, degree: 28"
    
echo "$output" | tee -a results/experiment1.txt

for ((d = 29; d <= 60; d++)); do

    # Each execution is tested 10 times

    # The first generates the context

    for ((i = 0; i < 3; i++)); do

        output=$(../build-release/Sort --random 8 --poly_degree $d --load_context )
    
        output="$output, degree: $d"
    
        echo "$output" | tee -a results/experiment1.txt

    done

    echo "Degree $d done" | tee -a results/experiment1.txt

done

###### FROM 60 TO 130 #####

# I recreate the context because d > 60 requires a deeper circuit


output=$(../build-release/Sort --random 8 --poly_degree 61 )    
    
output="$output, degree: 60"
    
echo "$output" | tee -a results/experiment1.txt

for ((d = 61; d <= 90; d++)); do

    # Each execution is tested 10 times

    # The first generates the context

    for ((i = 0; i < 3; i++)); do

        output=$(../build-release/Sort --random 8 --poly_degree $d --load_context )
    
        output="$output, degree: $d"
    
        echo "$output" | tee -a results/experiment1.txt

    done

    echo "Degree $d done" | tee -a results/experiment1.txt

done