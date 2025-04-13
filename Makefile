build:
	g++ rsa_serial.cpp -std=c++11 -o rsa_serial
	g++ -fopenmp rsa_openmp.cpp -o rsa_openmp -std=c++11
	mpic++ -o rsa_mpi rsa_mpi.cpp
clean:
	rm -f rsa_serial
	rm -f decrypted_output.txt encrypted_output.txt
	rm -f rsa_openmp
	rm -f rsa_mpi