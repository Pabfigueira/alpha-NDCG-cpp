all: alpha_nDCG.o
	g++ -std=c++11 -O3 -Wall alpha_nDCG.o -o a.out

alpha_nDCG.o: alpha_nDCG.cpp
	g++ -std=c++11 -O3 -Wall -c alpha_nDCG.cpp -o alpha_nDCG.o

clean:
	rm *.o a.out
