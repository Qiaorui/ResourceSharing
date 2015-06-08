all: main.exe

main.exe: main.o Sap.o Isap.o Preflow.o
	g++ -o main.exe main.o Sap.o Isap.o Preflow.o

main.o: main.cpp
	g++ -c main.cpp
	
Sap.o: Sap.cpp
	g++ -c Sap.cpp

Isap.o: Isap.cpp
	g++ -c Isap.cpp

Preflow.o: Preflow.cpp
	g++ -c Preflow.cpp

test:
	./main.exe -file input.in

test_benchmark:
	./main.exe -dir benchmark

test_small:
	./main.exe -dir smallTest

clean : 
	rm -f *.o main.exe *~
