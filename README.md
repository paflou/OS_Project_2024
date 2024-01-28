# Project in Operating Systems class in the University of Patras (2023-2024).


## Project 1
### Project 1 Part 1 is a simple bash script that .


### Project 1 Part 2 is a demonstration of the differences in speed of the calculation of the Monte Carlo Integration between single process, multi process and multi process using a single shared memory slot and semaphores.

To run, simply execute the ./run.sh file after running Make (**on a linux system**).
Default number of processes is 12, but you can edit that in the run script.

## Project 2
### Project 2 is a simulation of how Round Robin and FCFS work.
FCFS is created as an instance of RR with a massive time slice.
To run, execute Make in the 'work' folder, then in the sheduler folder. That way you generate the executables plus the simulation tasks.
There is also a run.sh file that runs every sheduler with different workloads.

The two files (sheduler.c, sheduler_io.c) both implement the two schedulers, but the latter also accounts for I/O operations.



