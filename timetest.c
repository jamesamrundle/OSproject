#include <stdio.h>
#include <stdlib.h>  // for exit(), rand(), strtol()
#include <pthread.h>
#include <time.h>    // for nanosleep()
#include <errno.h>   // for EINTR error check in millisleep()
#include <stddef.h>
#include <stdio.h>
#include <sys/sysinfo.h>
#include <math.h>


float stdsum , standardDeviation = 0.0;;

int timeSum;

long diff_time(struct timespec *t1, struct timespec *t2)
{
return t2->tv_nsec - t1->tv_nsec + (t2->tv_sec - t1->tv_sec) * 1000000000L;
}

void sumTimes(float data){
       stdsum += data;
}

void calculateSD(float data)
{
   float mean;

    int i;

  
       
   

    mean = stdsum/10000;

 
        standardDeviation += pow(data - mean, 2);

}

int main(int argc, char *argv[])
{

FILE * fp , *xx;
   /* open the file for writing*/
   fp = fopen ("./output","w");
     xx = fopen ("./DUMP","w");



struct timespec t1, t2;
int i , j ,k , r;
int d;
int sum;

char pete[10000] = {0};
for(i = 0;i<10000;i++){
  pete[i] = rand()%60;
}

for (i = 1; i < 100; i++) {
  fprintf (fp, "**************    %d     *****************  %d  ***********\n",i,i);
for(k = 1;k<50;k++){
  
  r =(rand()%10000);
fprintf(fp,"r== %d",r);

for (j = 0; j < 10; j++) {
  fprintf(fp,"arr[ %d ] ",(r +(j*i)%10000));
    clock_gettime(CLOCK_MONOTONIC, &t1);
sum =+ pete[(r +(j*i)%10000)];
clock_gettime(CLOCK_MONOTONIC, &t2);
printf("%d\n",sum);
d = diff_time(&t1, &t2);
timeSum += d;
fprintf (fp, "%d\n",d);
// sleep(1);
}
  fprintf (fp, ">>>>>>>>>>>>>>>>spacing of %d, run time = %d \n",i,timeSum);
timeSum = 0;
sum = 0;
}
}
fclose(xx);
fclose(fp);

// int addr_cnt = 0;
//   int each;
//   int rflag;
//  fp = fopen ("./output","r");

//   if (fp != NULL) {

//       printf("hi\n");
//     rflag =fscanf(fp, "%d\n",&each);
//           addr_cnt++;
//     // printf("%x",each);
//     calculateSD(each);

//     while (rflag != EOF) {
		  
//       addr_cnt++;
//         // printf("%d",addr_cnt);
//       rflag = fscanf(fp, "%d\n",&each);
//         calculateSD(each);

//     }

   
        	
    
//   } else addr_cnt = -1;

//   fclose(fp);

//     float finalStDev = sqrt(standardDeviation/10000);
// printf("final %f \n",finalStDev);

 


}
