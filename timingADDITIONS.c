#include <stdio.h>
#include <stdlib.h>
#include <time.h>    // for clock_gettime()
#include <math.h>
#define NUMITS   1000000
#define FREQSIZE     100


float stdsum, standardDeviation = 0.0;;
const int kb = 1024;
// const int buffSize = 8192;
int timeTotal, timeSum = 0;

int i, j, k, r;
int d;

int comparator(const void *p, const void *q)  
{ 
  int l = *((int *) p);
  int r = *((int *) q);
  return (l - r); 
}

void rdtscl(unsigned long long *ll)
{
  unsigned int lo, hi;
  __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));                        
  *ll = ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );  
}

long elapsed_ns(struct timespec* t1, struct timespec* t2)
{
  long seconds = t2->tv_sec - t1->tv_sec; 
  long ns = t2->tv_nsec - t1->tv_nsec; 
    
  if (ns < 0) { // clock underflow 
    --seconds;
    ns += 1000000000; 
  }

  return seconds*1000000000 + ns;
}

unsigned long get_cache_access_time()
{
  struct timespec t1, t2;
  unsigned long x = 0;
  unsigned long y = 0;
  unsigned long* results = malloc(sizeof(unsigned long) * NUMITS);
  unsigned long freq[FREQSIZE];
  unsigned long null_time, assign_time;
  unsigned int i;
  unsigned long max;
  unsigned long mode;
  int size = sizeof(results) / sizeof(results[0]);

  //clock_getres(CLOCK_MONOTONIC, &t1); TIMING RESOLUTION 1ns
  

  //Eliminating overhead involved in using clock_gettime
  for (i = 0; i < NUMITS; i++) {
    clock_gettime(CLOCK_MONOTONIC, &t1);
    clock_gettime(CLOCK_MONOTONIC, &t2);
    results[i] = elapsed_ns(&t1, &t2);
  }

  for (i = 0; i < FREQSIZE; i++) {
    freq[i] = 0;
  }

  for (i = 0; i < NUMITS; i++) {
    if (results[i] < FREQSIZE) {
      freq[results[i]]++;
    }
  }

  max = 0;
  mode = 0;
  for (i = 0; i < FREQSIZE; i++) {
    if (freq[i] > max) {
      max = freq[i];
      mode = i;
    }
  }
  null_time = mode;
  //Finish determining overhead involved in using clock_gettime


  //Begin testing access time to data
  //stored in results[] by assigning values from it to x.
  //Most accesses from results[] will be from the cache,
  //so it is safe to say that the mode will reflect
  //accesses from the cache
  for (i = 0; i < NUMITS; i++) {
    clock_gettime(CLOCK_MONOTONIC, &t1);
    x += results[i];//Overhead involved but has minimal effect on reference time
    clock_gettime(CLOCK_MONOTONIC, &t2);
    results[i] = elapsed_ns(&t1, &t2);
  }

  for (i = 0; i < FREQSIZE; i++) {
    freq[i] = 0;
  }

  for (i = 0; i < NUMITS; i++) {
    if (results[i] < FREQSIZE) {
      freq[results[i]]++;
    }
  }

  //Getting the mode time for an assignment statement
  max = 0;
  mode = 0;
  for (i = 0; i < FREQSIZE; i++) {
    if (freq[i] > max) {
      max = freq[i];
      mode = i;
    }
  }

  assign_time = mode - null_time;

  return assign_time;


}

unsigned long get_ram_access_time(){
  struct timespec t1, t2;
  unsigned long x = 0;
  unsigned long y = 0;
  unsigned long* results = malloc(sizeof(unsigned long) * NUMITS);
  unsigned long freq[FREQSIZE];
  unsigned long null_time, assign_time;
  unsigned int i;
  unsigned long max;
  unsigned long mode;
  int size = sizeof(results) / sizeof(results[0]);

  //clock_getres(CLOCK_MONOTONIC, &t1); TIMING RESOLUTION 1ns
  

  //Eliminating overhead involved in using clock_gettime
  for (i = 0; i < NUMITS; i++) {
    clock_gettime(CLOCK_MONOTONIC, &t1);
    clock_gettime(CLOCK_MONOTONIC, &t2);
    results[i] = elapsed_ns(&t1, &t2);
  }

  for (i = 0; i < FREQSIZE; i++) {
    freq[i] = 0;
  }

  for (i = 0; i < NUMITS; i++) {
    if (results[i] < FREQSIZE) {
      freq[results[i]]++;
    }
  }

  max = 0;
  mode = 0;
  for (i = 0; i < FREQSIZE; i++) {
    if (freq[i] > max) {
      max = freq[i];
      mode = i;
    }
  }
  null_time = mode;
  //Finish determining overhead involved in using clock_gettime


  //Begin testing access time to data
  //stored in results[] by assigning values from it to x.
  //Most accesses from results[] will be from the cache,
  //so it is safe to say that the mode will reflect
  //accesses from the cache
  const int kb =1024;
  int* bigArr = (int*)malloc(kb*kb*kb);//create fresh array for each spacing
  int arrSize = (kb*kb*kb) /4;
  int spacing = 1024;
  int sum  = 0;
  int index = 0;
  for (i = 0; i < NUMITS; i++) {
    clock_gettime(CLOCK_MONOTONIC, &t1);
    sum += bigArr[index];//Overhead involved but has minimal effect on reference time
    clock_gettime(CLOCK_MONOTONIC, &t2);
    
    index = (index + spacing)%arrSize;

    results[i] = elapsed_ns(&t1, &t2);
  }
  free(bigArr);

  for (i = 0; i < FREQSIZE; i++) {
    freq[i] = 0;
  }

  for (i = 0; i < NUMITS; i++) {
    if (results[i] < FREQSIZE) {
      freq[results[i]]++;
    }
  }

  //Getting the mode time for an assignment statement
  max = 0;
  mode = 0;
  for (i = 0; i < FREQSIZE; i++) {
    if (freq[i] > max) {
      max = freq[i];
      mode = i;
    }
  }

  assign_time = mode - null_time;

  return(assign_time);
}

int get_cache_line_size()
{
  long sum = 0;
  int buffSize = kb*kb*kb/4;
  
  int maxSpacing = 64;
  int numLoops = log2(maxSpacing)+1;//+1 for the 1st 0 spacing
  int* arr4 = (int*)malloc(kb*kb*kb);
  int* arrTimes = (int*)malloc(numLoops*4);
  struct timespec t1, t2;
  int timeSum = 0;
  int p;
  k = 0;

	int spacing = 0; //initial spacing 0 means constantly hitting arr[0]
	int reps = 1000000;   

	for (j = 0; j < reps; j++) {
	// fprintf(fp, "arr[ %d ] ", (0 + (j * spacing) % 10000));
	clock_gettime(CLOCK_MONOTONIC, & t1);
	sum += arr4[((j)*spacing )% buffSize];

	// fprintf(xx, "\n\n%d\n", sum);
	clock_gettime(CLOCK_MONOTONIC, & t2);

	timeSum += elapsed_ns( & t1, & t2);

	// fprintf(fp, "%d\n", d);
	}

  free(arr4);
	arrTimes[k] = (timeSum / reps);

  
	//******************TEST SPACING NEXT
  
  
  k++;
	for (i = 1; i <= maxSpacing; i = i *2){  // sets spacing between accesses to increasing powers of 2 
		                                  // note that these are integers 
		                              //so when spacing is 8 for example, its taking jumps of 32 bytes
                                  
	arr4 = (int*)malloc(kb*kb*kb);//create fresh array for each spacing
  timeSum = 0;
	sum = 0;
	spacing = i;

	for (j = 0; j <reps; j++) { //basically traverses the array for reps steps of spacing increments

	clock_gettime(CLOCK_MONOTONIC, & t1);

	sum += arr4[((j)*spacing )% buffSize];

	clock_gettime(CLOCK_MONOTONIC, & t2);
	// nullTime = 0
	timeSum += elapsed_ns( & t1, & t2);

	}
  // printf("spacing of %d = %d \n", spacing, timeSum/reps);

	arrTimes[k] = (timeSum / reps);// stores average time in an array for future calculation
	k++;

	free(arr4);
     }
	//p


	printf("sum =  %d\n", sum);
	// ************** this bit compares runtime of spacing i and i+1 and finds largest jumps 
	// ************saved as cliff variable.
	int gap ;
	int cliff;
  sum = 0;
	for (int x = 0; x <=numLoops; x++) {
    sum+= arrTimes[x];
    int byteSpace;
    if( x == 0) byteSpace = 0;
    else byteSpace = pow(2,x-1); // calculates bytes that correspond do the entry in arrTimes

    
	  // printf("spacing of %d bytes = %d \n", byteSpace, arrTimes[x]);
    
            // if(x > 4){
            // int rightDiff = arrTimes[x+1] - arrTimes[x];   //**I want this to work but its not RELIABLE!
            // int leftDiff = arrTimes[x] - arrTimes[x-1];    // **and for some reason the cliff section doesnt execute correctly o.O
            
            // if((rightDiff >= 1.8*leftDiff)&&(rightDiff < 2.6*leftDiff))
            //      printf("\t\t\t\t\t%d might be spacing \n",byteSpace);
            // }

	  if(arrTimes[x+1] > 0 && arrTimes[x+1]-arrTimes[x] > gap){
	   cliff = byteSpace;
	   gap = arrTimes[x+1]-arrTimes[x];
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
	printf("returning = %d\n",cliff);
  return cliff; 

}

//int mainmem_access_time()
//{
  
// unsigned long get_cache_size(){

//   int kb = 1024; 
//   int reps = 1000000000;
//   struct timespec t1, t2;
//   long double accessTime = 0;

//   int nullTime;
//   int i, k;

//   for(i=kb*4; i < kb*kb; i = i*2){
   
//    int arrSize = (i) / sizeof(int);
//     int * arr1 = (int*)malloc(arrSize*sizeof(int));
    

//     for(k=0; k < arrSize; k ++){
//         arr1[k] = k; 
//     }
//     for(k=0; k < arrSize; k ++){
//         int x1,x2;
//         int swap;
//         x1 = rand() % arrSize;
//         x2 = rand() % arrSize;
//         swap = arr1[x1];
//         arr1[x1] = arr1[x2];
//         arr1[x2] = swap;
//         // printf(" x1 = %d x2 = %d\n",x1,x2);
//     } 
    
//     int nextInt=0;
//     k=0;

//     clock_gettime(CLOCK_MONOTONIC, &t1); //start clock
//     for(k = 0; k < reps; k++){
//         nextInt=arr1[nextInt];
//     }
//     clock_gettime(CLOCK_MONOTONIC, &t2); //end clock
//     //calculate the time elapsed in ns per access


//     // accessTime = (((endAccess.tv_sec - startAccess.tv_sec) * SECONDS_PER_NS) + (endAccess.tv_nsec - startAccess.tv_nsec)) / (NUM_ACCESSES);
//     accessTime = elapsed_ns( & t1, & t2)/ reps;

//     printf("Access time: %Lf for size %ld\n", accessTime, i);

//     free(arr1);
//   }  
//   return accessTime;
// }
	


//}


int main(int argc, char** argv)
{
  printf("BEGINNING MEMORY ANALYSIS\n");
  printf("--------------------------\n");

  // printf("Cache size = %d bytes\n",get_cache_size());

  printf("Cache line size: %d\n", get_cache_line_size());

  unsigned long cache_access_time = get_cache_access_time();
  if (cache_access_time < 1)
  {

    printf("Cache reference time: <1ns\n");

  }
  else
  {

    printf("Cache reference time: %lu\n", cache_access_time);

  }


    printf("Main Memory reference time: %lu\n", get_ram_access_time());


  printf("-------------------------\n");
  printf("ENDING MEMORY ANALYSIS");

  


  
  
  return 0;
}
