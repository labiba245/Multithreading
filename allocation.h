#ifndef ALLOCATION_H
#define ALLOCATION_H

#include <unistd.h>
#include <cstdlib>
#include <stddef.h>
#include <pthread.h>

#define FF 1
#define BF 2
#define WF 3

struct Chunk
{
    void *baseAddress;
    size_t chunkSize;
};

void *alloc(void *arg);

void *dealloc(void *arg);

void *showStatus(void *arg);

void lockForReadingAllocatedList();

void unlockForReadingAllocatedList();

void lockForReadingFreedList();

void unlockForReadingFreedList();

void initialiseLocks();

#endif
