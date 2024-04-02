EXE=allocate

$(EXE): main.c linkedList.c linkedList.h processManager.c processManager.h genericMemoryManaging.c genericMemoryManaging.h roundRobin.c roundRobin.h memoryManagerInfinite.c memoryManagerInfinite.h memoryManagerContiguous.c memoryManagerContiguous.h memoryManagerPaged.c memoryManagerPaged.h memoryManagerVirtual.c memoryManagerVirtual.h
	cc -Wall -o $(EXE) main.c linkedList.c processManager.c genericMemoryManaging.c roundRobin.c memoryManagerInfinite.c memoryManagerContiguous.c memoryManagerPaged.c memoryManagerVirtual.c

format:
	clang-format -style=file -i *.c
clean:
	rm -f $(EXE) *.o 