#include <stdio.h>

#include <stdlib.h>  // for exit(), rand(), strtol()

#include <pthread.h>

#include <time.h>    // for nanosleep()

#include <errno.h>   // for EINTR error check in millisleep()

#include <stddef.h>

#include <stdio.h>

#include <sys/sysinfo.h>

#include <math.h>


float stdsum, standardDeviation = 0.0;;
const int kb = 1024;
// const int buffSize = 8192;
int timeTotal, timeSum = 0;

int i, j, k, r;
int d;
int sum;
struct timespec gen;
int arr1[8192] = { // int elements in 32kb array
  0
};
int arr2[10000] = {
  0
};

int arr3[10000] = {
  0
};
int * arr4;
int arrTimes[80] = {
  0
};

int modes[100] = {
  0
};


void clearModes(){
  for (int z = 0; z < 100; z++) {
    modes[z] = 0;
  }
  
}

void randArray() {
  int b;
  for (b = 0; b < 10000; b++) {
    clock_gettime(CLOCK_MONOTONIC, & gen);

    r = ((int) gen.tv_nsec) % 99;
    r = rand_r( & r) % 100;
    // arr1[b%()] = r;
    arr2[b] = b;
    arr3[b] = r;
        arr4[b] = r;


    // printf("ARRRRRR %d\n",arr1[b]);
  }
}

long diff_time(struct timespec * t1, struct timespec * t2) {
  return t2 -> tv_nsec - t1 -> tv_nsec + (t2 -> tv_sec - t1 -> tv_sec) * 1000000000L;
}

void floodCache(int * x, FILE * file) {
  int z;
  for (int z = 0; z < 10000; z++) {
    fprintf(file, "%d", ( * x) + z);
  }
}

int main(int argc, char * argv[]) {

  int buffSize = kb*kb*kb;
  FILE * fp, * xx;
  /* open the file for writing*/
  fp = fopen("./output", "w");
  xx = fopen("./DUMP", "w");
  arr4 = (int*)malloc(kb*kb*kb);
  struct timespec t1, t2;
  int nullTime;

  for (i = 0; i < 100000; i++) {
    clock_gettime(CLOCK_MONOTONIC, & t1);
    clock_gettime(CLOCK_MONOTONIC, & t2);
    d = diff_time( & t1, & t2);
    timeSum += d;
  }
  nullTime = timeSum / 1000000;
  timeSum = 0;
  printf("nullTime = %d\n", nullTime);

  // return(0);

  int p;
  k = 0;

  for (p = 0; p < 1; p++) {
    int spacing = 0;
    int reps = 1000;
    randArray();
    floodCache(arr2, xx);
    floodCache(arr3, xx);
    // floodCache(arr4, xx);



    for (j = 0; j < reps; j++) {
      // fprintf(fp, "arr[ %d ] ", (0 + (j * spacing) % 10000));

      clock_gettime(CLOCK_MONOTONIC, & t1);
      sum = sum + arr4[(0 + (j * spacing) % (buffSize))];
      clock_gettime(CLOCK_MONOTONIC, & t2);

      // fprintf(xx, "\n\n%d\n", sum);

      d = diff_time( & t1, & t2);
      timeSum += (d - nullTime);

      // fprintf(fp, "%d\n", d);
    }

    arrTimes[k] = (timeSum / reps);
    fprintf(fp, ">>>>>>>>>>>>>>>>spacing of %d, avg run time = %d \n", spacing, timeSum / reps);
    timeSum = 0;
    sum = 0;

//******************TEST SPACING NEXT


    for (i = 2; i <= 64; i = i * 2) {
      

     

      sum = 0;
      spacing = i;
      r = 0; //rand() % 10000;

      // printf("rand = %d", r);

      clearModes();

      randArray();
      floodCache(arr2, xx);
      floodCache(arr3, xx);
      // floodCache(arr4, xx);
      for (j = reps; j >0; j--) {
        clock_gettime(CLOCK_MONOTONIC, & t1);

        
      //  arr1[(j-1)*spacing % buffSize] = arr1[(j)*spacing % buffSize];
        // sum += arr1[(j-2)*spacing % buffSize];
        // sum += arr1[(j-3)*spacing % buffSize];
        sum += arr4[((j)*spacing )% buffSize];
        clock_gettime(CLOCK_MONOTONIC, & t2);

          // if(j%10 == 0) 
          // printf( "\n\n%d\n", sum);
          // printf("spacing of %d ,accessed %d then %d \n",spacing,(j-1)*spacing % buffSize,(r + (j * spacing) % (buffSize)));
        d = diff_time( & t1, & t2);
        if(d-nullTime<100){
          modes[d-nullTime] ++;
        }
        timeSum += (d - nullTime);
        
            // printf("*%d*%d\n", p, d);
        // sleep(1);
      }
      // printf("%d",sum);
      fprintf(fp, ">>>>>>>>>>>>>>>>spacing of %d, avg run time = %d \n", spacing, timeSum / reps);
      
      
      arrTimes[k] = (timeSum / reps);
      k++;
      timeSum = 0;
      
  //     for (int z = 0; z < 100; z++) {
  //   // fprintf(result,"hits of  %d = %d \n", i, modes[i] );
  //   printf("hits of  %d = %d \n", z, modes[z]);
  // }
      int modeTime = 0;
      for (int z = 0; z < 100; z++) {
          //  fprintf(fp," arrtime[%d] =\n",k,arrTimes[k]);
          if(modes[z]>modeTime){
          modeTime = z;
           }
      }
      printf(" spacing is %d mode is %d\n",spacing,modeTime);
    }
  } //p


  printf("sum =  %d\n", sum);
  FILE * result;
  result = fopen("./results", "w");

  for (i = 0; i < 10; i++) {
    fprintf(result, "spacing of %d = %d \n", i, arrTimes[i]);
    printf("spacing of %d = %d \n", i, arrTimes[i]);
  }
  
  fclose(xx);
  fclose(result);
  fclose(fp);

}