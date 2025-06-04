#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int prime_find(int x)
{
   if (x < 2)
      return 0;
   for (int i = 2; i <= (x + 1) / 2; i++)
   {
      if (x % i == 0)
         return 0;
   }
   return 1;
}
int main(int argc, char *argv[])
{
   int M = atoi(argv[1]);
   int N = atoi(argv[2]);
   int t = atoi(argv[3]);
   int num = 0;
   int *prime = malloc((N - M + 1) * sizeof(int));
   double tstart = omp_get_wtime();
   omp_set_num_threads(t);

#pragma omp parallel
   {
      int local_num = 0;
      int *prime_loc = malloc((N - M + 1) * sizeof(int));
#pragma omp for
      for (int i = M; i <= N; i++)
      {
         if (prime_find(i))
         {
            prime_loc[local_num++] = i;
         }
      }
#pragma omp critical
      {
         for (int j = 0; j < local_num; j++)
         {
            prime[num++] = prime_loc[j];
         }
      }
   }

   double ttaken = omp_get_wtime() - tstart;
   char filename[64];
   sprintf(filename, "%d.txt", N);
   FILE *fp = fopen(filename, "w");
   for (int i = 0; i < num; i++)
   {
      fprintf(fp, "%d\n", prime[i]);
   }
   fclose(fp);
   printf("The number of prime numbers found between %d and %d is %d.\n", M, N, num);
   printf("Time taken for the main part: %f\n", ttaken);
   return 0;
}