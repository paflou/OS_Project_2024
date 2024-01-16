/*Pantelis Flouris, 1093507*/
/*Aggelos Menegatos, 1093426*/
/*Chrysafis Koltsakis, 1084671*/
/*Giorgos Amaxopoulos, 1093311*/

#include "functions.h"

Queue* RR(FILE *fp, Queue *q,int time_slice);
int io_pending = 0, io_done = 0;
double start_time;

void io_pending_handler(int signo) {
	io_pending = 1;
}
void io_done_handler(int signo) {
	io_done = 1;
}

void sig_handler(int signo) {
}


int main(int argc,char **argv)
{
	signal(SIGCHLD, sig_handler);
	signal(SIGUSR1, io_pending_handler);
	signal(SIGUSR2, io_done_handler);

	char buffer[MAX];
	Queue *q = createQueue();
	FILE *fp;
	char* init_d;
	int time_slice;
	
	if(strcmp(argv[1], "FCFS") == 0){
		init_d = argv[1];
		fp = fopen(argv[2],"r");
    	if(fp == NULL){
        	printf("Could not open file\n");
        	return 2;
    	}
    } else if(strcmp(argv[1], "RR") == 0){
		time_slice = atoi(argv[2]);
	    init_d = argv[1];
		fp = fopen(argv[3],"r");
		if(fp == NULL){
			printf("Could not open file\n");
			return 2;
		}
    } else {
        printf("Wrong input!\n");
        return 1;
    }

    //save the commands in a queue
    while(fgets(buffer, MAX, fp)) {
        char *token = strtok(buffer, "\n");
		Enqueue(q, token, 0, 0);
    }

    if(strcmp(init_d, "FCFS") == 0){
        printf("Starting FCFS scheduling...\n");
    	start_time = get_wtime();
        q = RR(fp, q, 1000); 
    }
	else if(strcmp(init_d, "RR") == 0){
		printf("Starting RR scheduling...\n");
		start_time = get_wtime();
		q = RR(fp, q, 0.001 * time_slice); 
	}
	else {
        printf("Wrong input!\n");
        return 1;
    }
    printf("\nScheduler finished.\n");
	printf("Total time: %.2f seconds.\n", get_wtime() - start_time);
    return 0;

}

Queue *RR(FILE *fp, Queue *q, int time_slice){
    int status = 0;
    Node *current_node;
	Queue *finished = createQueue();
    while(!isNull(q)){
		current_node = Dequeue(q);
        if (current_node->status == NEW) {
            current_node->status = RUNNING;
            current_node->pid = fork();
			if(current_node->pid == 0){
				current_node->pid = getpid();
				current_node->status = RUNNING;
				printf("Executing %s\n", current_node->value);
				execl(current_node->value, "", NULL);
			    perror("execl");
               	exit(0);
   	        }
		}
		else if(current_node->status == STOPPED || current_node->status == IO) {
			current_node->status = RUNNING;
			kill(current_node->pid, SIGCONT);
		}

		sleep(time_slice);
		//stop the process right after the time slice
		kill(current_node->pid, SIGSTOP);

		if(io_done == 1) {
			current_node->status = STOPPED;
			io_done = 0;
		}

		int check = waitpid(current_node->pid, &status, WNOHANG);
        if(check == 0){
            // The process did not finish, so add it back to the end of the queue
			current_node->status = STOPPED;
			if(io_pending == 1) {
				current_node->status = IO;
				io_pending = 0;
			}
            Enqueue(q, current_node->value, current_node->pid, current_node->status);
        }
		else {
			double end_time = get_wtime();
	    	double elapsed_time = ((double) (end_time - start_time));
			current_node->status = EXITED;
            Enqueue(finished, current_node->value, current_node->pid, current_node->status);
			printf("Program %s finished.\n", current_node->value);
			printf("\tElapsed Time: %.2f secs\n\n", elapsed_time);
		}
    }
	return finished;
}

