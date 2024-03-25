
allocate: main.o linkedList.o
	gcc -o allocate main.o linkedList.o 

main.o: main.c linkedList.h
	gcc -c main.c 

linkedList.o: linkedList.c linkedList.h 
	gcc -c linkedList.c 

clean:
	rm -f *.o allocate