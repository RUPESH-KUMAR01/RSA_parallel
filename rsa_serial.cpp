#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <chrono>

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
        if (exponent % 2 == 1)  // if exponent is odd
            result = (result * base) % mod;

        base = (base * base) % mod;
        exponent = exponent / 2;
    }
    return result;
}

int main(int argc, char *argv[]) {
    vector<unsigned int> message;
    vector<unsigned long long> encryptedMessage;

    if (argc != 2) {
        cout << "Usage: ./<executable> <input_file>\n";
        return -1;
    }

    ifstream fInput(argv[1]);
    if (!fInput) {
        cout << "Error opening the input file.\n";
        return -1;
    }

    unsigned int iP, iQ, iD, iE, iN, iTotientN;
    fInput >> iP >> iQ;

    unsigned int num;
    while (fInput >> num) {
        message.push_back(num);
    }
    fInput.close();

    int messageSize = message.size();
    encryptedMessage.resize(messageSize);

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

    auto start_enc = high_resolution_clock::now();

    for (i = 0; i < messageSize; i++) {
        encryptedMessage[i] = exp(message[i], iE, iN);
    }

    auto end_enc = high_resolution_clock::now();
    auto encryption_time = duration_cast<microseconds>(end_enc - start_enc).count();

    ofstream fEncrypted("encrypted_output.txt");
    for (i = 0; i < messageSize; i++) {
        fEncrypted << encryptedMessage[i] << "\n";
    }
    fEncrypted.close();
    cout << "Encryption complete! Output saved to 'encrypted_output.txt'.\n";
    cout << "Encryption Time: " << encryption_time << " µs\n";

    cout << "\n------ Decrypting Message -------\n";
    cout << "D: " << iD << "\nN: " << iN << endl;

    auto start_dec = high_resolution_clock::now();

    for (i = 0; i < messageSize; i++) {
        message[i] = exp(encryptedMessage[i], iD, iN);
    }

    auto end_dec = high_resolution_clock::now();
    auto decryption_time = duration_cast<microseconds>(end_dec - start_dec).count();

    ofstream fDecrypted("decrypted_output.txt");
    for (i = 0; i < messageSize; i++) {
        fDecrypted << message[i] << "\n";
    }
    fDecrypted.close();
    cout << "Decryption complete! Output saved to 'decrypted_output.txt'.\n";
    cout << "Decryption Time: " << decryption_time << " µs\n";

    cout << "\nTotal Execution Time: " << encryption_time + decryption_time << " µs\n";

    return 0;
}
