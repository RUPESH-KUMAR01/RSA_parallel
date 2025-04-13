all: build run

build:
	g++ -std=c++11 rsa_serial.cpp -o rsa_serial
	g++ -fopenmp -std=c++11 rsa_openmp.cpp -o rsa_openmp
	mpic++ rsa_mpi.cpp -o rsa_mpi

run:
	@echo "Running Serial Version:"
	@./rsa_serial input.txt
	@echo "\nRunning OpenMP Version:"
	@./rsa_openmp input.txt
	@echo "\nRunning MPI Version:"
	@mpiexec -n 8 ./rsa_mpi input.txt

clean:
	rm -f rsa_serial
	rm -f rsa_openmp
	rm -f rsa_mpi
	rm -f encrypted_output.txt
	rm -f decrypted_output.txt
