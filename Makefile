all: alpha_nDCG.o
	g++ -O3 -Wall alpha_nDCG.o -o a.out

alpha_nDCG.o: alpha_nDCG.cpp
	g++ -O3 -Wall -c alpha_nDCG.cpp -o alpha_nDCG.o

clean:
	rm *.o a.out