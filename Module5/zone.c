
#define _CRT_SECURE_NO_WARNINGS

#include "zone.h"
#include <string.h>
#include <assert.h>

#define HUNK_SENTINAL  0x1df001ed

typedef struct  
{
    int     MemorySentinal;
    int     MemorySize;
    char    Name[8];
} hunk_t;

// hunk stuff
char* HunkBase;
int   HunkSize;
int   HunkLowOffset;
int   HunkHighOffset;

// zone stuff
#define ZONESIZE 0xc000

void Memory_Init(void *Buffer, int BufferSize)
{
    HunkBase = Buffer;
    HunkSize = BufferSize;
    HunkLowOffset = 0;
    HunkHighOffset = 0;

    // allocate 48k on Hunk for Zone space
    //void* ZoneSpace = Hunk_AllocName(ZONESIZE, "zone");
}

void Hunk_Check(void)
{
    for (hunk_t* Header = (hunk_t*)HunkBase; (char*)Header != (HunkBase + HunkLowOffset); )
    {
        if (Header->MemorySentinal != HUNK_SENTINAL)
            assert(0);
        if (Header->MemorySize < 16 || (char*)Header - HunkBase + Header->MemorySize > HunkSize)
            assert(0);
        Header = (hunk_t*)((char*)Header + Header->MemorySize);
    }
}

void *Hunk_Alloc(int RequestSize)
{
    return Hunk_AllocName(RequestSize, "unknown");
}

void* Hunk_AllocName(int RequestSize, char* RequestName)
{
    int TotalSize = sizeof(hunk_t) + ((RequestSize + 15)& ~15);

    hunk_t *Header;

    Header = (hunk_t*)(HunkBase + HunkLowOffset);
    HunkLowOffset += TotalSize;

    memset(Header, 0, TotalSize);

    Header->MemorySize = TotalSize;
    Header->MemorySentinal = HUNK_SENTINAL;
    strncpy(Header->Name, RequestName, 8);

    return (void*)(Header+1);
}

void Hunk_FreeToLowMark(int Mark)
{
    memset(HunkBase + Mark, 0, HunkLowOffset - Mark);
    HunkLowOffset = Mark;
}

void* Hunk_HighAllocName(int RequestSize, char* RequestName)
{
    int TotalSize = sizeof(hunk_t) + ((RequestSize + 15)& ~15);

    hunk_t *Header;

    HunkHighOffset += TotalSize;

    Header = (hunk_t*)(HunkBase + HunkSize - HunkHighOffset);

    memset(Header, 0, TotalSize);
    
    Header->MemorySize = TotalSize;
    Header->MemorySentinal = HUNK_SENTINAL;
    strncpy(Header->Name, RequestName, 8);

    return (void*)(Header + 1);
}

void Hunk_FreeToHighMark(int Mark)
{
    memset(HunkBase + HunkSize - HunkHighOffset, 0, HunkHighOffset - Mark);
    HunkHighOffset = Mark;
}