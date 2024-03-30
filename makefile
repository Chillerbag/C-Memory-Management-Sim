
allocate: main.o processManager.o linkedList.o genericMemoryManaging.o roundRobin.o memoryManagerInfinite.o memoryManagerContiguous.o memoryManagerPaged.o memoryManagerVirtual.o
	gcc -Wall -o allocate main.o processManager.o linkedList.o genericMemoryManaging.o roundRobin.o memoryManagerInfinite.o memoryManagerContiguous.o memoryManagerPaged.o memoryManagerVirtual.o

main.o: main.c linkedList.h genericMemoryManaging.h processManager.h
	gcc -Wall -c main.c 

processManager.o: processManager.c processManager.h linkedList.h genericMemoryManaging.h roundRobin.h
	gcc -Wall -c processManager.c 

linkedList.o: linkedList.c linkedList.h 
	gcc -Wall -c linkedList.c 
	
genericMemoryManaging.o: genericMemoryManaging.c genericMemoryManaging.h memoryManagerInfinite.h memoryManagerContiguous.h  memoryManagerPaged.h  memoryManagerVirtual.h 
	gcc -Wall -c genericMemoryManaging.c 

memoryManagerInfinite.o: memoryManagerInfinite.c memoryManagerInfinite.h 
	gcc -Wall -c memoryManagerInfinite.c 

memoryManagerContiguous.o: memoryManagerContiguous.c memoryManagerContiguous.h 
	gcc -Wall -c memoryManagerContiguous.c 

memoryManagerPaged.o: memoryManagerPaged.c memoryManagerPaged.h 
	gcc -Wall -c memoryManagerPaged.c 

memoryManagerVirtual.o: memoryManagerVirtual.c memoryManagerVirtual.h 
	gcc -Wall -c memoryManagerVirtual.c 

roundRobin.o: roundRobin.c roundRobin.h 
	gcc -Wall -c roundRobin.c 
	

clean:
	rm -f *.o allocate