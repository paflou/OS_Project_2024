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

double get_wtime(void) {
  struct timeval t;
  gettimeofday(&t, NULL);
  return (double)t.tv_sec + (double)t.tv_usec * 1.0e-6;
}

static double f(double x) {
  return sin(cos(x));
}

double *shared_memory_initialization(int processes_num){
  return mmap(NULL,processes_num*sizeof(double),
              PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS,-1,0);
} 

void calculate_integral(unsigned long n,int processes_num, double* memory,int array_slot){
  srand48(array_slot); 
  double res=0;
  for (unsigned long i = 0; i < n; i++) {
    double xi;
    xi = drand48();
    res += f(xi);
  }
  memory[array_slot]=res;
}

double combine_results(int processes_num,double* ptr,   unsigned long n){
  double a = 0.0;
  double b = 1.0;
  const double h = (b-a)/n;

  double result=0;
  for(int i=0;i<processes_num;i++){
    result+=ptr[i]*h;
  }
  return result;
}

int main(int argc, char *argv[]) {
  unsigned long n = 24e7;  // or e8
  const double ref = 0.73864299803689018;
  double res = 0;
  double t0, t1;

  int Processes = 4; //Default number of processes
  unsigned long instances_per_core;


  if (argc == 2) {
    Processes = atol(argv[1]);
    if(Processes<=0){
      printf("Wrong input!\n");
      exit(1);
    } 
  }
  
  instances_per_core= n / Processes;

  double *ptr= shared_memory_initialization(Processes);

  t0=get_wtime();                                               //start

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
  t1=get_wtime();                                              //finish

  printf("Result=%.16f Error=%e Rel.Error=%e Time=%lf seconds\n",
           res, fabs(res-ref), fabs(res-ref)/ref, t1-t0);

  munmap(ptr,Processes*sizeof(double));

  return 0;
}
