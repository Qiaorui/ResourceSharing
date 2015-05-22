all: main.exe

main.exe: main.o Sap.o
	g++ -o main.exe main.o Sap.o

main.o: main.cpp
	g++ -c main.cpp
	
Sap.o: Sap.cpp
	g++ -c Sap.cpp

test:
	./main.exe -file input.in

test_dir:
	./main.exe -dir benchmark

test_small:
	./main.exe -dir smallTest

clean : 
	rm -f *.o main.exe
