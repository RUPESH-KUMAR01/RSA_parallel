#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <chrono>
#include <mpi.h>

using namespace std;
using namespace chrono;

unsigned int gcd(unsigned int a, unsigned int b) {
    while (b) {
        unsigned int x = a % b;
        a = b;
        b = x;
    }
    return a;
}

unsigned long long exp(unsigned long long base, unsigned long long exponent, unsigned long long mod) {
    unsigned long long result = 1;
    base = base % mod;

    while (exponent > 0) {
        if (exponent % 2 == 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exponent /= 2;
    }
    return result;
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    vector<unsigned int> message;
    vector<unsigned long long> encryptedMessage;
    unsigned int iP, iQ, iE, iD, iN, iTotientN;

    if (rank == 0) {
        if (argc != 2) {
            cout << "Usage: mpirun -np <num_procs> ./<executable> <input_file>\n";
            MPI_Abort(MPI_COMM_WORLD, -1);
        }

        ifstream fInput(argv[1]);
        if (!fInput) {
            cerr << "Error opening the input file.\n";
            MPI_Abort(MPI_COMM_WORLD, -1);
        }

        fInput >> iP >> iQ;
        unsigned int num;
        while (fInput >> num) {
            message.push_back(num);
        }
        fInput.close();

        iN = iP * iQ;
        iTotientN = (iP - 1) * (iQ - 1);

        iE = 7;
        while (gcd(iE, iTotientN) != 1) {
            iE++;
        }

        int i = 1;
        while (true) {
            if ((iE * i) % iTotientN == 1) {
                iD = i;
                break;
            }
            i++;
        }

        cout << "P: " << iP << "\nQ: " << iQ << endl;
        cout << "\n------ Encrypting Message -------\n";
        cout << "E: " << iE << "\nN: " << iN << endl;
    }

    // Broadcast public/private key values to all processes
    MPI_Bcast(&iP, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
    MPI_Bcast(&iQ, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
    MPI_Bcast(&iE, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
    MPI_Bcast(&iD, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
    iN = iP * iQ;

    // Broadcast message size
    int messageSize = message.size();
    MPI_Bcast(&messageSize, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Resize vectors in other processes
    if (rank != 0) {
        message.resize(messageSize);
    }

    // Broadcast message to all processes
    MPI_Bcast(message.data(), messageSize, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
    encryptedMessage.resize(messageSize);

    // Start encryption
    auto start_enc = high_resolution_clock::now();

    for (int i = rank; i < messageSize; i += size) {
        encryptedMessage[i] = exp(message[i], iE, iN);
    }

    // Gather results back to root
    MPI_Reduce(rank == 0 ? MPI_IN_PLACE : encryptedMessage.data(),
               encryptedMessage.data(), messageSize, MPI_UNSIGNED_LONG_LONG,
               MPI_SUM, 0, MPI_COMM_WORLD);

    auto end_enc = high_resolution_clock::now();
    int64_t encryption_time;
    if (rank == 0) {
        encryption_time = duration_cast<microseconds>(end_enc - start_enc).count();
        ofstream fEncrypted("encrypted_output.txt");
        for (auto val : encryptedMessage)
            fEncrypted << val << "\n";
        fEncrypted.close();
        cout << "Encryption complete! Output saved to 'encrypted_output.txt'.\n";
        cout << "Encryption Time: " << encryption_time << " µs\n";
        cout << "\n------ Decrypting Message -------\n";
        cout << "D: " << iD << "\nN: " << iN << endl;
    }

    // Start decryption
    auto start_dec = high_resolution_clock::now();

    for (int i = rank; i < messageSize; i += size) {
        message[i] = exp(encryptedMessage[i], iD, iN);
    }

    // Gather decrypted message back to root
    MPI_Reduce(rank == 0 ? MPI_IN_PLACE : message.data(),
               message.data(), messageSize, MPI_UNSIGNED,
               MPI_SUM, 0, MPI_COMM_WORLD);

    auto end_dec = high_resolution_clock::now();

    if (rank == 0) {
        auto decryption_time = duration_cast<microseconds>(end_dec - start_dec).count();
        ofstream fDecrypted("decrypted_output.txt");
        for (auto val : message)
            fDecrypted << val << "\n";
        fDecrypted.close();
        cout << "Decryption complete! Output saved to 'decrypted_output.txt'.\n";
        cout << "Decryption Time: " << decryption_time << " µs\n";
        cout << "\nTotal Execution Time: " << encryption_time + decryption_time << " µs\n";
    }

    MPI_Finalize();
    return 0;
}
