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
long sum;
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
  for (b = 0; b < kb*kb*kb/4; b++) {
    // clock_gettime(CLOCK_MONOTONIC, & gen);

    // r = ((int) gen.tv_nsec) % 99;
    // r = rand_r( & r) % 100;
    // arr1[b%()] = r;
    // arr2[b] = b;
    // arr3[b] = r;
        arr4[b] = 0;


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

  int buffSize = kb*kb*kb/4;
  FILE * fp, * xx;
  /* open the file for writing*/
  fp = fopen("./output", "w");
  xx = fopen("./DUMP", "w");
  arr4 = (int*)malloc(kb*kb*kb);
  struct timespec t1, t2;
  int nullTime;
   int index;

  for (i = 0; i < 1000000; i++) {
    clock_gettime(CLOCK_MONOTONIC, & t1);
      clock_gettime(CLOCK_MONOTONIC, & t2);

  }
    d = diff_time( & t1, & t2);
    timeSum += d;
  nullTime = timeSum / 1000000;
  timeSum = 0;
  printf("nullTime = %d\n", nullTime);

  // return(0);

  int p;
  k = 0;

  for (p = 0; p < 1; p++) {
    int spacing = 0;
    int reps = 1000000;
    // randArray();
    // floodCache(arr2, xx);
    // floodCache(arr3, xx);
    // floodCache(arr4, xx);


      
    index = 0;
    for (j = 0; j < reps; j++) {
      // fprintf(fp, "arr[ %d ] ", (0 + (j * spacing) % 10000));
      clock_gettime(CLOCK_MONOTONIC, & t1);
        sum += arr4[index];  //((j)*spacing )% buffSize
        
      // fprintf(xx, "\n\n%d\n", sum);
      clock_gettime(CLOCK_MONOTONIC, & t2);
      index = (index + spacing)%buffSize;
      d = diff_time( & t1, & t2);
      timeSum += (d - nullTime);
     
      // fprintf(fp, "%d\n", d);
    }
           

    arrTimes[k] = (timeSum / reps);
    fprintf(fp, ">>>>>>>>>>>>>>>>spacing of %d, avg run time = %d \n", spacing, timeSum / reps);
    timeSum = 0;
    sum = 0;

//******************TEST SPACING NEXT
    free(arr4);
   index = 0;
    for (i = 16; i <= 1024*kb; i = i *2) { // sets spacing between accesses to increasing powers of 2 
                                    // note that these are integers 
                                    //so when spacing is 8 for example, its taking jumps of 32 bytes
     
     
      arr4 = (int*)malloc(kb*kb*kb);//create fresh array for each spacing

      sum = 0;
      spacing = i;
      

      // printf("rand = %d", r);

      clearModes(); // clears modes calculated on each iteration. modes dont seem to be usefull

      // randArray();
      // floodCache(arr2, xx);
      // floodCache(arr3, xx);
      // floodCache(arr4, xx);
      index = 0;
      for (j = 0; j <reps; j++) { //basically traverses the array for reps steps of spacing increments
              
        clock_gettime(CLOCK_MONOTONIC, & t1);
        
        sum += arr4[index];  //((j)*spacing )% buffSize
        
        clock_gettime(CLOCK_MONOTONIC, & t2);
        // nullTime = 0;
         index = (index + spacing)%buffSize;
        d = diff_time( & t1, & t2);
        if(d-nullTime<100){
          // printf("fukya");  increments modes array index 
          modes[d-nullTime] ++;
        }
        timeSum += (d - nullTime);
      
      }
      

      fprintf(fp, ">>>>>>>>>>>>>>>>spacing of %d, avg run time = %d \n", spacing, timeSum / reps);
      fprintf(fp,"%d\n",timeSum);
      
      arrTimes[k] = (timeSum / reps);// stores average time in an array for future calculation
      k++;
      timeSum = 0;
      

      int modeTime = 0;
      for (int z = 0; z < 100; z++) {
          //  fprintf(fp," arrtime[%d] =\n",k,arrTimes[k]);
          if(modes[z]>modeTime){
         modeTime = z;
           }
      }
      printf(" spacing is %d mode is %d\n",spacing,modeTime);
      free(arr4);
    }
    fprintf(fp,"*****");
  } //p


  printf("sum =  %d\n", sum);
  FILE * result;
  result = fopen("./results", "w");
// ************** this bit compares runtime of spacing i and i+1 and finds largest jumps 
      // ************saved as cliff variable.
  int gap ;
  int cliff;
  for (i = 0; i < 8; i++) {
    // fprintf(result, "spacing of %d = %d \n", i, arrTimes[i]);
    printf("spacing of %d = %d \n", i, arrTimes[i]);
  if(arrTimes[i+1] > 0 && arrTimes[i+1]-arrTimes[i] > gap){
     cliff = i;
     gap = arrTimes[i+1]-arrTimes[i];
  }
  }
  /* typically the "cliff" is calculated at 3 or 4 wich corresponds to spacings
   jumps of 8(32b) to 16(64b) and 16(64b) to 32(128b) respectively.
  havent made it user friendly so cross reference with output file 
    i expect the line size to be 64. We can interpret result as :
          1)If jump is at spacing of 8(32b) to 16(64b) that the spacing is consistantly accessing bytes
               that are on the end of lines  that arent prefetched
          2) if jump is at spacing of 16(64b) to 32(128b) perhaps the indexes are accessed in such a way to 
                encourage prefetching at spacing of 16(64b) and the significance of increased access speed isnt
                  fully shown until spacing is further increased*/
  printf("cliff is %d  at %d\n",cliff,arrTimes[cliff]);
  
  fclose(xx);
  fclose(result);
  fclose(fp);

}