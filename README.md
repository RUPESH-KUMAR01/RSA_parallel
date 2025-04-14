# Implementation of RSA Algorithm with Parallel Processing

This project provides three implementations of the RSA encryption algorithm:

- **Serial Implementation**: A straightforward, single-threaded version.
- **OpenMP Parallelization**: Utilizes OpenMP to parallelize the encryption process across multiple threads.
- **MPI Parallelization**: Employs MPI to distribute the encryption workload across multiple processes, potentially on different machines.

## Table of Contents

- [Overview](#overview)
- [Prerequisites](#prerequisites)
- [Compilation](#compilation)
- [Usage](#usage)
- [Input Format](#input-format)
- [Output](#output)
- [Performance Evaluation](#performance-evaluation)
- [License](#license)

## Overview

RSA (Rivest–Shamir–Adleman) is a widely-used public-key cryptosystem for secure data transmission. The encryption and decryption processes involve modular exponentiation, which can be computationally intensive for large keys. This project explores parallelization techniques to improve the performance of RSA encryption:

- **Serial Version**: Serves as a baseline for performance comparison.
- **OpenMP Version**: Parallelizes the modular exponentiation step using multiple threads on a shared-memory system.
- **MPI Version**: Distributes the encryption tasks across multiple processes, which can run on distributed-memory systems.

## Prerequisites

To compile and run the programs, ensure you have the following installed:

- **C++ Compiler**: Supports C++11 or later.
- **OpenMP**: For the OpenMP version.
- **MPI Library**: Such as MPICH or OpenMPI, for the MPI version.

## Compilation

Use the provided `Makefile` to compile all versions:

```bash
make
```


This will generate the following executables:

- `rsa_serial`
- `rsa_openmp`
- `rsa_mpi`

Alternatively, compile each version manually:

- **Serial Version**:

  ```bash
  g++ -o rsa_serial rsa_serial.cpp
  ```

- **OpenMP Version**:

  ```bash
  g++ -fopenmp -o rsa_openmp rsa_openmp.cpp
  ```

- **MPI Version**:

  ```bash
  mpic++ -o rsa_mpi rsa_mpi.cpp
  ```

## Usage

Each executable requires an input file containing the plaintext message and RSA parameters.

- **Serial Version**:

  ```bash
  ./rsa_serial input.txt
  ```

- **OpenMP Version**:

  ```bash
  ./rsa_openmp input.txt <num_threads>
  ```

  Replace `<num_threads>` with the desired number of threads.

- **MPI Version**:

  ```bash
  mpirun -np <num_processes> ./rsa_mpi input.txt
  ```

  Replace `<num_processes>` with the number of processes to use.

## Input Format

The input file (`input.txt`) should be structured as follows:

```

<p> <q>
<message>
```


- `<p>` and `<q>`: Two prime numbers used to generate the RSA keys.
- `<message>`: The plaintext message to be encrypted, represented as a string of digits.

**Example**:

```

61 53
1234567890
```


## Output

Each program will output:

- The generated RSA keys (public and private).
- The encrypted message.
- The decrypted message (to verify correctness).
- The execution time for the encryption and decryption processes.

## Performance Evaluation

By comparing the execution times of the serial, OpenMP, and MPI versions, you can evaluate the performance gains achieved through parallelization. This can help in understanding the scalability and efficiency of parallel computing techniques applied to cryptographic algorithms.

## License

This project is open-source and available under the [MIT License](LICENSE).

---
