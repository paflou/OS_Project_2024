/*Pantelis Flouris, 1093507*/
/*Aggelos Menegatos, 1093426*/
/*Chrysafis Koltsakis, 1084671*/
/*Giorgos Amaxopoulos, 1093311*/

#include "functions.h"
#define MAX 256


double start_time;

void RR(FILE *fp, Queue *q, int time_slice);

void sig_handler(){
}

int main(int argc,char **argv)
{
	char buffer[MAX];
	Queue *q = createQueue();
	FILE *fp;
	char* init_d;
	int time_slice;
	signal(SIGALRM, sig_handler);
	signal(SIGCHLD, sig_handler);
	
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

        RR(fp, q, 1000); 
    }
	else if(strcmp(init_d, "RR") == 0){
        printf("Starting RR scheduling...\n");
    	start_time = get_wtime();
        RR(fp, q, 0.001 * time_slice); 
    }
	else {
        printf("Wrong input!\n");
        return 1;
    }
    printf("\nScheduler finished.\n");
	printf("Total time: %.2f seconds.\n\n", get_wtime() - start_time);
    return 0;
}




void RR(FILE *fp, Queue *q, int time_slice){
    int status = 0;
    Node *current_node;

	current_node = Dequeue(q);
    while(current_node->status != EXITED){
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
		else if(current_node->status == STOPPED){
			current_node->status = RUNNING;
			kill(current_node->pid, SIGCONT);
		}

		alarm(time_slice);
		pause();


  		int check = waitpid(current_node->pid, &status, WNOHANG);
        if(check == 0){
            // The process did not finish, so add it back to the end of the queue
		    kill(current_node->pid, SIGSTOP);
            current_node->status = STOPPED;
            Enqueue(q, current_node->value, current_node->pid, current_node->status);
        }
		else {
			double end_time = get_wtime();
	    	double elapsed_time = ((double) (end_time - start_time));
			current_node->status = EXITED;
            Enqueue(q, current_node->value, current_node->pid, current_node->status);
			printf("Program %s finished.\n", current_node->value);
			printf("\tElapsed Time: %.2f secs\n\n", elapsed_time);
		}
	current_node = Dequeue(q);
    }
Enqueue(q, current_node->value, current_node->pid, current_node->status);
}

