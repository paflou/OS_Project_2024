///////////////////////////////
// Pantelis Flouris 1093507
// Chrysafis Koltsakis
// Georgios Amaxopoulos
// Aggelos Menegatos
//////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>
#include <semaphore.h>

sem_t flag; //Semaphore declaration


// Function declarations

double get_wtime(void);

static double f(double x);

double *shared_memory_initialization();

void calculate_integral(unsigned long n,int processes_num, double* memory,int array_slot);

double combine_results(int processes_num,double* ptr,   unsigned long n);



int main(int argc, char *argv[]) {
  unsigned long n = 24e7;  // or e8
  const double ref = 0.73864299803689018;
  double res = 0;
  double t0, t1;

  int Processes = 4; //Default number of processes
  unsigned long instances_per_core;

  sem_init(&flag,1,1); //semaphore initialization

  if (argc == 2) { //Number of processes to create if not default (4)
    Processes = atol(argv[1]);
    if(Processes<=0){
      printf("Wrong input!\n");
      exit(1);
    } 
  }
  
  instances_per_core= n / Processes;

  double *ptr= shared_memory_initialization(Processes);

  t0=get_wtime();                                               //start the timer

  for (int i = 0; i < Processes; ++i) {
    pid_t child_pid = fork();
    if(child_pid==0){
        calculate_integral(instances_per_core,Processes,ptr,i);
        exit(EXIT_SUCCESS);
      }
      else if (child_pid<0){
        exit(EXIT_FAILURE);
      }
    }
  while(wait(NULL)!=-1);


  res= combine_results(Processes,ptr,n);
  t1=get_wtime();                                              //stop the timer

  printf("Result=%.16f Error=%e Rel.Error=%e Time=%lf seconds\n",
           res, fabs(res-ref), fabs(res-ref)/ref, t1-t0);

  munmap(ptr,sizeof(double)); //Deallocate memory
  sem_destroy(&flag); //destroy semaphore
  return 0;
}


double get_wtime(void) {
  struct timeval t;
  gettimeofday(&t, NULL);
  return (double)t.tv_sec + (double)t.tv_usec * 1.0e-6;
}

static double f(double x) {
  return sin(cos(x));
}

double *shared_memory_initialization(){
  double *ptr= mmap(NULL,sizeof(double),
              PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS,-1,0);
  *ptr=0;
  return ptr;
}

void calculate_integral(unsigned long n,int processes_num, double* memory,int array_slot){
  srand48(array_slot); 
  double res=0;
  for (unsigned long i = 0; i < n; i++) {
    double xi;
    xi = drand48();
    res += f(xi);
  }
  sem_wait(&flag);
  *memory += res;
  sem_close(&flag);
}


double combine_results(int processes_num,double* ptr,   unsigned long n){
  double a = 0.0;
  double b = 1.0;
  const double h = (b-a)/n;
  return *ptr * h;
}