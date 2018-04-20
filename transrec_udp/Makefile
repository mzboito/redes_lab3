CC = gcc
FLAGS = -Wall 

all: rec trans

rec: rec.c
	$(CC) rec.c -lc -o rec $(FLAGS) 

trans: trans.c 
	$(CC) trans.c -lc -o trans $(FLAGS) 

clean:
	rm rec trans 
