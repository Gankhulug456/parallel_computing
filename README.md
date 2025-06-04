# Parallel Computing Projects

A collection of parallel computing labs built with **C**, **OpenMP**, and **MPI**, exploring thread- and process-level parallelism. Projects include a multi-threaded prime number generator and an MPI-based histogram builder.

---

## Overview

- **Language**: C (GCC 9.3)  
- **Platform**: Ubuntu 20.04, 8-core Intel CPU / Linux MPI Cluster  
- **Tools**: OpenMP 4.5, OpenMPI 4.0  

---

## OpenMP Prime Finder

A multi-threaded Sieve of Eratosthenes that finds all prime numbers in a given range \[M, N\] using OpenMP.

### Implementation Details

- **Compilation**: `gcc -fopenmp prime_finder.c -o prime_finder`
- **Thread Strategy**:  
  Each thread maintains a private list of found primes (`prime_loc`).  
  Threads iterate over candidates using `#pragma omp for`, then merge results into a global array inside a `#pragma omp critical` section.

### Optimization Notes

- Avoids false sharing by isolating per-thread writes to local buffers
- Uses `#pragma omp parallel` and `#pragma omp for` for workload distribution
- Optimized the sieve loop: each x is tested only against divisors ≤ √x

---

## 📊 MPI Histogram Builder

Generates a histogram of randomly generated floats in \[0, 20) using distributed computing with MPI.

### Implementation Details

- **Compilation**: `mpicc histogram_builder.c -o histogram_builder`
- **Execution**: `mpirun -np 4 ./histogram_builder`
- **Parallel Strategy**:
  - Rank 0 generates N random floats in \[0, 20)
  - Data is split across ranks using `MPI_Send` (or kept locally by rank 0)
  - Each rank builds a local histogram
  - Global result is computed with:

    ```c
    MPI_Reduce(local_bins, bins, num_bins, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    ```

### Optimization Notes

- Handles uneven splits by giving the first (N mod P) ranks one extra element
- Uses `MPI_Barrier` and `MPI_Wtime` to time only the parallel section
- Verified against a serial version for correctness

---

## 📈 Performance Discussion

- **OpenMP Prime Finder**:  
  For small problem sizes, adding more threads may reduce performance due to thread creation and synchronization overhead. This matches expected behavior discussed in lecture materials.

- **MPI Histogram Builder**:  
  For large inputs, distributing the workload improves performance and showcases effective scalability and parallel speed-up.

---

## 🌐 See More

If you’re interested in seeing more of my work, visit my [website](https://ganaa.work/).

---



