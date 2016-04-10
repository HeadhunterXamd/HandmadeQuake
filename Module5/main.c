#include <stdlib.h>

#include "zone.h"

#define ALLOCSIZE 16*1024*1024

int main()
{
    void* MemoryBlock = malloc(ALLOCSIZE);

    Memory_Init(MemoryBlock, ALLOCSIZE);

    int* MyIntPtr = (int*)Hunk_AllocName(4, "temp");
    *MyIntPtr = 4;

    return 0;
}

