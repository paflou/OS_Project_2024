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
#include <time.h>
#include <unistd.h> 
#include <pthread.h> 
#include <semaphore.h>

typedef struct{
  double* memory;
  unsigned long instances;
  int thread_num;
} Thread_input;

Thread_input createThread(double* memIn,unsigned long instancesIn,int thread_numIn){
  Thread_input newThread;
  newThread.memory=memIn;
  newThread.instances=instancesIn;
  newThread.thread_num=thread_numIn;
  return newThread;
}

sem_t flag;

void* calculate_integral(void* arg){
  Thread_input *input = (Thread_input *)arg;
  srand48(input->thread_num);
  double res=0;
  for (unsigned long i = 0; i < input->instances; i++) {
    double xi;
    xi = drand48();
    res +=  sin(cos(xi));;
  }

  sem_wait(&flag);
  *input->memory += res;
  sem_post(&flag);
}

double get_wtime(void) {
  struct timeval t;
  gettimeofday(&t, NULL);
  return (double)t.tv_sec + (double)t.tv_usec * 1.0e-6;
}

double combine_results(int processes_num,double* ptr,   unsigned long n){
  double a = 0.0;
  double b = 1.0;
  const double h = (b-a)/n;

  return *ptr*h;
}

int main(int argc, char *argv[]) {
  unsigned long n = 24e7;  // or e8
  const double ref = 0.73864299803689018;
  double res = 0;
  double t0, t1;
  int thread_num = 4; //Default number of threads
  unsigned long instances_per_thread;

  sem_init(&flag,0,1);

  double* ptr=mmap(NULL,sizeof(double),
              PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS,-1,0);
  *ptr=0; //initialize memory to 0


  if (argc == 2) {
    thread_num = atol(argv[1]);
    if(thread_num<=0){
      printf("Wrong input!\n");
      exit(1);
    } 
  }
  pthread_t thread[thread_num];
  instances_per_thread= n / thread_num;

  t0=get_wtime();                                               //start

  for (int i = 0; i < thread_num; ++i) {
    Thread_input input = createThread(ptr,instances_per_thread,i);
    pthread_create(&thread[i],NULL,calculate_integral,&input);
    pthread_join(thread[i],NULL);
  }

  res= combine_results(thread_num,ptr,n);
  t1=get_wtime();                                              //finish

  printf("Result=%.16f Error=%e Rel.Error=%e Time=%lf seconds\n\n",
           res, fabs(res-ref), fabs(res-ref)/ref, t1-t0);

  sem_destroy(&flag);
  return 0;
}
