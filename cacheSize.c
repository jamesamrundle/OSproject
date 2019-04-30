#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

int kb = 1024; 
int reps = 1000000000;
struct timespec t1, t2;

int nullTime;
int i, k;

long double diff_time(struct timespec * t1, struct timespec * t2) {
  // accessTime = (((endAccess.tv_sec - startAccess.tv_sec) * SECONDS_PER_NS) + (endAccess.tv_nsec - startAccess.tv_nsec)) / (NUM_ACCESSES);

  return t2 -> tv_nsec - t1 -> tv_nsec + (t2 -> tv_sec - t1 -> tv_sec) * 1000000000L;
}

int main(int argc, char * argv[]) {
    srand(time(NULL));  // Seed random function

   
  
  for(i=kb*4; i < kb*kb; i = i*2){
   
   int arrSize = (i) / sizeof(int);
    int * arr1 = (int*)malloc(arrSize*sizeof(int));
    

    for(k=0; k < arrSize; k ++){
        arr1[k] = k; 
    }
    for(k=0; k < arrSize; k ++){
        int x1,x2;
        int swap;
        x1 = rand() % arrSize;
        x2 = rand() % arrSize;
        swap = arr1[x1];
        arr1[x1] = arr1[x2];
        arr1[x2] = swap;
        // printf(" x1 = %d x2 = %d\n",x1,x2);
    } 
    
    int nextInt=0;
    k=0;
    long double accessTime = 0;

    clock_gettime(CLOCK_MONOTONIC, &t1); //start clock
    for(k = 0; k < reps; k++){
        nextInt=arr1[nextInt];
    }
    clock_gettime(CLOCK_MONOTONIC, &t2); //end clock
    //calculate the time elapsed in ns per access


    // accessTime = (((endAccess.tv_sec - startAccess.tv_sec) * SECONDS_PER_NS) + (endAccess.tv_nsec - startAccess.tv_nsec)) / (NUM_ACCESSES);
    accessTime = diff_time( & t1, & t2)/ reps;

    printf("Access time: %Lf for size %ld\n", accessTime, i);

    free(arr1);
  }  

}
