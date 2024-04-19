#!/bin/bash

# Clear the contents of the results
#> results/experiment1.txt

echo "Starting the experiment 1" | tee -a results/experiment1.txt

###### FROM 20 TO 27 #####

#output=$(../build-release/Sort --random 8 --poly_degree 20)    
    
#output="$output, degree: 20"
    
#echo "$output" | tee -a results/experiment1.txt

#for ((d = 20; d <= 27; d++)); do

    # Each execution is tested 10 times

    # The first generates the context

#    for ((i = 0; i < 10; i++)); do

#        output=$(../build-release/Sort --random 8 --poly_degree $d --load_context)
    
#        output="$output, degree: $d"
    
#        echo "$output" | tee -a results/experiment1.txt

#    done

#    echo "Degree $d done" | tee -a results/experiment1.txt

#done

###### FROM 28 TO 59 #####

# I recreate the context because d > 27 requires a deeper circuit

#output=$(../build-release/Sort --random 8 --poly_degree 28)    
    
#output="$output, degree: 28"
    
#echo "$output" | tee -a results/experiment1.txt

#for ((d = 28; d <= 59; d++)); do

    # Each execution is tested 10 times

    # The first generates the context

#    for ((i = 0; i < 10; i++)); do

#        output=$(../build-release/Sort --random 8 --poly_degree $d --load_context)
    
#        output="$output, degree: $d"
    
#        echo "$output" | tee -a results/experiment1.txt

#    done

#    echo "Degree $d done" | tee -a results/experiment1.txt

#done

###### FROM 60 TO 130 #####

# I recreate the context because d > 59 requires a deeper circuit


#output=$(../build-release/Sort --random 8 --poly_degree 60)    
    
#output="$output, degree: 60"
    
#echo "$output" | tee -a results/experiment1.txt

#for ((d = 60; d <= 130; d++)); do

for ((d = 89; d <= 130; d++)); do

    # Each execution is tested 10 times

    # The first generates the context

    for ((i = 0; i < 10; i++)); do

        output=$(../build-release/Sort --random 8 --poly_degree $d --load_context)
    
        output="$output, degree: $d"
    
        echo "$output" | tee -a results/experiment1.txt

    done

    echo "Degree $d done" | tee -a results/experiment1.txt

done