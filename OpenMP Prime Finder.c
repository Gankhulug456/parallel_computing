#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define RANGE 20.0

void check_correctness(float *, int *, int, int);

int main(int argc, char *argv[])
{

	double start, end, local_time, elapsed;
	int i; // loop index
	int num_items = 0;
	int num_bins = 0;
	float *data; // array containing data items
	int *bins;	 // the histogram itself
	int myrank;

	if (argc != 3)
	{
		printf("You need to enter two items (in the following orders):\n");
		printf("Number of data items: [1, 10000000000]\n");
		printf("Number of bins: positive integer\n");
		return 1;
	}

	num_items = atoi(argv[1]);
	num_bins = atoi(argv[2]);

	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	// Only process 0 generates the data.
	if (myrank == 0)
	{

		// Allocating the arrays containing the data items and the bins
		if (!(data = (float *)calloc(num_items, sizeof(float))))
		{
			printf("Error allocating data items array\n");
			exit(1);
		}

		if (!(bins = (int *)calloc(num_bins, sizeof(int))))
		{
			printf("Error allocating array of bins\n");
			exit(1);
		}

		/* Now, fill up the data items */
		srand(time(NULL));
		for (i = 0; i < num_items; i++)
			data[i] = ((float)rand() / (float)(RAND_MAX)) * (RANGE - 0.1);

	} // end if

	MPI_Barrier(MPI_COMM_WORLD);
	start = MPI_Wtime();

	/* TODO:  code to be timed is here. This is your main MPI code.
			  Feel free to create any functions you want or add any vaariables you want.	*/
	/* Your code must do the following:
	- Distribute the data array among the processes.
	- Each process will do a local histogram for the data it has received. This includes process 0 too.
	- Local histograms are combined to form the final global histogram. You need to think a bit about this.

	*/
	/* TODO:  code to be timed is here. This is your main MPI code.
			  Feel free to create any functions you want or add any variables you want.	*/
	/* Your code must do the following:
	- Distribute the data array among the processes.
	- Each process will do a local histogram for the data it has received. This includes process 0 too.
	- Local histograms are combined to form the final global histogram. You need to think a bit about this.
	*/

	int processes;
	MPI_Comm_size(MPI_COMM_WORLD, &processes);

	int items = num_items / processes;
	int remainder = num_items % processes;
	int local = items + (myrank < remainder ? 1 : 0);
	float *local_data = (float *)malloc(local * sizeof(float));
	int *local_bins = (int *)calloc(num_bins, sizeof(int));

	if (myrank == 0)
	{
		for (int dest = 1; dest < processes; dest++)
		{
			int send_count = items + (dest < remainder ? 1 : 0);
			MPI_Send(&data[dest * items + (dest < remainder ? dest : remainder)], send_count, MPI_FLOAT, dest, 0, MPI_COMM_WORLD);
		}
		for (int i = 0; i < local; i++)
		{
			local_data[i] = data[i];
		}
	}
	else
	{
		MPI_Recv(local_data, local, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}

	float bin_range = RANGE / num_bins;
	for (int i = 0; i < local; i++)
	{
		int bin = (int)(local_data[i] / bin_range);
		local_bins[bin]++;
	}
	MPI_Reduce(local_bins, bins, num_bins, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	/* Do not modify the code from here till end of the main() function: */
	end = MPI_Wtime();
	local_time = end - start;
	/* Assuming process 0 will end up having the time elapsed to be measured. You can change that if you want. */
	MPI_Reduce(&local_time, &elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
	if (myrank == 0)
	{
		check_correctness(data, bins, num_items, num_bins);
		printf("Time Taken: %lf\n", local_time);
		free(data);
		free(bins);
	}

	MPI_Finalize();

	return 0;
}

/*** Do not modify the following function **/
void check_correctness(float *data, int *bins, int num_items, int num_bins)
{

	int i;
	int *refbins;
	float binrange;

	binrange = (float)(RANGE) / (float)(num_bins);

	if (!(refbins = (int *)calloc(num_bins, sizeof(int))))
	{
		printf("Error allocating array of refbins\n");
		exit(1);
	}

	for (i = 0; i < num_items; i++)
		refbins[(int)(data[i] / binrange)]++;

	for (i = 0; i < num_bins; i++)
		if (bins[i] != refbins[i])
			printf("Mistmatch in element %d of the histogram.... your item = %d ... the correct is %d\n", i, bins[i], refbins[i]);
}

/*******************************************/