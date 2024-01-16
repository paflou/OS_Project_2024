#!/bin/bash

THREADS=12

# ANSI color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m' # No color

# create an array for PIDs (Process IDs)
pids=()

# create an array for commands
commands=()

# make the files
make
echo -e "${GREEN}----------------------------------------${NC}"

# Define the programs to run with their respective commands
commands+=("./bin/integral_mc_seq")
commands+=("./bin/integral_mc_shm $THREADS")
commands+=("./bin/integral_mc_shm_sem $THREADS")

# run the files in the background and store their PIDs
for cmd in "${commands[@]}"
do
    echo -e "${BLUE}Starting $cmd${NC}"
    $cmd > "${cmd// /_}.out" & # Redirect output to a file and run in the background
    pids+=("$!") # Store the PID of the background process
done
echo -e "${GREEN}----------------------------------------${NC}"

# Start a timer
start_time=$SECONDS
all_done=0

# Timer and check loop
while [ $all_done -eq 0 ]; do
    all_done=1
    for pid in "${pids[@]}"; do
        if kill -0 $pid 2>/dev/null; then
            all_done=0
            break
        fi
    done
    elapsed=$((SECONDS - start_time))
    echo -ne "\r${RED}Elapsed time: ${elapsed} seconds${NC}"
    sleep 1
done

# Print the final elapsed time
elapsed=$((SECONDS - start_time))
echo -e "\r${YELLOW}Total elapsed time: ${elapsed} seconds${NC}"

echo -e "${GREEN}----------------------------------------${NC}"

# Print the results
for cmd in "${commands[@]}"
do
    echo -e "${GREEN}Results of $cmd:${NC}"
    cat "${cmd// /_}.out" # Display the content of the output file
    echo
done

# remove the files
make clean
