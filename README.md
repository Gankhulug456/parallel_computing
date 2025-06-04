Parallel Computing Projects
OpenMP & MPI Labs

Overview
A set of parallel computing projects built using C, OpenMP, and MPI. These projects explore thread-level and process-level parallelism through hands-on implementations of classic algorithms and parallel programming patterns.

🔧 Setup
Language: C (GCC 9.3)
Platform: Ubuntu 20.04, 8-core Intel CPU / Linux MPI Cluster
Tools: OpenMP 4.5, OpenMPI 4.0

🧵 OpenMP Prime Finder
A multi-threaded implementation of the Sieve of Eratosthenes to compute all prime numbers in a given range 
[
𝑀
,
𝑁
]
[M,N] using OpenMP. The program demonstrates near-linear speedup on an 8-core CPU.

Key Features
Compiled with gcc -fopenmp

Each thread uses a private buffer (prime_loc) to avoid false sharing

Primes from threads are merged using a #pragma omp critical section

Optimized inner loop by limiting divisors to ≤ √x

Parallel Strategy
c
Copy
Edit
#pragma omp parallel
{
    // Each thread has private buffer
    #pragma omp for
    for (int i = M; i <= N; ++i) {
        // Sieve logic using private prime_loc
    }
    #pragma omp critical
    {
        // Merge results into global prime list
    }
}
Challenges & Optimizations
Avoided false sharing with per-thread local buffers

Minimized contention with a small #pragma omp critical region

Limited divisor checks to √x to improve time complexity

📊 MPI Histogram Builder
Generates a histogram from randomly generated floating-point numbers in 
[
0
,
20
)
[0,20) using MPI for parallel data distribution and reduction.

Key Features
Built for a 4-node Linux cluster using OpenMPI

Rank 0 generates N random floats and distributes them across ranks

Each rank computes a local histogram, then reduces to a global one using MPI_Reduce

Parallel Strategy
Non-root ranks receive data via MPI_Send; root rank copies locally

Each rank computes a local bin count

Final result is obtained via:

c
Copy
Edit
MPI_Reduce(local_bins, bins, num_bins, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
Challenges & Optimizations
Handled uneven data splits: first N % P ranks receive one extra value

Used MPI_Barrier and MPI_Wtime to measure only the parallel section

Verified correctness by comparing with a serial version

💡 Notes on Performance
OpenMP Prime Finder:
For small input sizes, adding threads may hurt performance due to overhead from spawning threads and contention in #pragma omp critical. This matches the expected non-linear speedup discussed in class.

MPI Histogram Builder:
For large input sizes, more ranks improve performance by dividing the workload. This demonstrates good scalability, consistent with lecture material on parallel efficiency.

📁 Project Structure
Copy
Edit
.
├── openmp_prime_finder.c
├── mpi_histogram_builder.c
├── Makefile
├── README.md
🌐 Portfolio
Interested in more projects like this? Check out my website for additional work in systems programming, web dev, and interactive media.
