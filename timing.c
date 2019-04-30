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

int get_cache_line_size()
{
  long sum = 0;
  int buffSize = kb*kb*kb/4;
  int arrTimes[80] = {0};
  int* arr4 = (int*)malloc(kb*kb*kb);
  struct timespec t1, t2;
  int timeSum = 0;
  int p;
  k = 0;

	int spacing = 0;
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


	arrTimes[k] = (timeSum / reps);
	timeSum = 0;
	sum = 0;

	//******************TEST SPACING NEXT
	free(arr4);
        k++;
	for (i = 1; i <= 256; i = i *2) 
        {                             // sets spacing between accesses to increasing powers of 2 
		                      // note that these are integers 
		                      //so when spacing is 8 for example, its taking jumps of 32 bytes


	arr4 = (int*)malloc(kb*kb*kb);//create fresh array for each spacing

	sum = 0;
	spacing = i;

	for (j = 0; j <reps; j++) { //basically traverses the array for reps steps of spacing increments

	clock_gettime(CLOCK_MONOTONIC, & t1);

	sum += arr4[((j)*spacing )% buffSize];

	clock_gettime(CLOCK_MONOTONIC, & t2);
	// nullTime = 0
	timeSum += elapsed_ns( & t1, & t2);

	}

	arrTimes[k] = (timeSum / reps);// stores average time in an array for future calculation
	k++;
	timeSum = 0;

	free(arr4);
     }
	//p


	printf("sum =  %d\n", sum);
	// ************** this bit compares runtime of spacing i and i+1 and finds largest jumps 
	// ************saved as cliff variable.
	int gap ;
	int cliff;
	for (i = 0; i < 9; i++) {
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
	return cliff; //Cliff was spacing by integer, so need to multiply by 4 to get amount of bytes.

}

//int mainmem_access_time()
//{
  
  
	


//}


int main(int argc, char** argv)
{
  printf("BEGINNING MEMORY ANALYSIS\n");
  printf("--------------------------\n");

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





  printf("-------------------------\n");
  printf("ENDING MEMORY ANALYSIS");

  


  
  
  return 0;
}