#pragma once

/*
------Top of Memory------ -

high hunk allocations

< -- - high hunk reset point held by vid

    video buffer

    z buffer

    surface cache

    < -- - high hunk used

    cachable memory

    < -- - low hunk used

    client and server low hunk allocations

    < --low hunk reset point held by host

    startup hunk allocations

    Zone block

    ---- - Bottom of Memory---- -
*/

void Memory_Init(void *Buffer, int BufferSize);

void* Hunk_AllocName(int RequestSize, char* RequestName);
