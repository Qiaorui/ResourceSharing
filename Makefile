all: main.exe

main.exe: main.o Sap.o Preflow.o
	g++ -o main.exe main.o Sap.o Preflow.o

main.o: main.cpp
	g++ -c main.cpp
	
Sap.o: Sap.cpp
	g++ -c Sap.cpp

Preflow.o: Preflow.cpp
	g++ -c Preflow.cpp

test:
	./main.exe -sap -file input.in

test_benchmark_sap:
	./main.exe -sap -benchmark < files.in

test_benchmark_pre:
	./main.exe -pre -benchmark < files.in

test_small_sap:
	./main.exe -sap -dir smallTest

test_small_pre:
	./main.exe -pre -dir -smallTest

clean : 
	rm -f *.o main.exe *~
