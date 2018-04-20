CC = gcc
FLAGS = -Wall 

all: 
	gcc -c testecli.cpp -o testecli.o 
	gcc -c testesrv.cpp -o testesrv.o
	gcc -o testecli testecli.o
	gcc -o testesrv testesrv.o	

clean:
	rm *.o testecli testesrv
