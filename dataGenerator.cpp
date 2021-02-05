#include "dataGenerator.h"
#include <iostream>
#include <string.h>

using std::cout;
using std::string;

pthread_t threadAlloc;

int *intArray(int size)
{
    size_t sizeRequiredForInt = size * sizeof(int);
    pthread_create(&threadAlloc, NULL, alloc, &sizeRequiredForInt);
    void *valueReturnedFromThreadAlloc;
    pthread_join(threadAlloc, &valueReturnedFromThreadAlloc);
    int *addressAllocated = (int *)valueReturnedFromThreadAlloc;

    for (int i = 0; i < size; i++)
    {
        addressAllocated[i] = i + 1;
    }
    return addressAllocated;
}
double *doubleArray(int size)
{
    size_t sizeRequiredForDouble = size * sizeof(double);
    pthread_create(&threadAlloc, NULL, alloc, &sizeRequiredForDouble);
    void *valueReturnedFromThreadAlloc;
    pthread_join(threadAlloc, &valueReturnedFromThreadAlloc);
    double *addressAllocated = (double *)valueReturnedFromThreadAlloc;

    for (int i = 0; i < size; i++)
    {
        addressAllocated[i] = i + 1;
    }
    return addressAllocated;
}

int *arrayOfOddIntegers(int size)
{
    size_t sizeRequiredForOddInt = size * sizeof(int);
    pthread_create(&threadAlloc, NULL, alloc, &sizeRequiredForOddInt);
    void *valueReturnedFromThreadAlloc;
    pthread_join(threadAlloc, &valueReturnedFromThreadAlloc);
    int *addressAllocated = (int *)valueReturnedFromThreadAlloc;

    for (int i = 0; i < size; i++)
    {
        addressAllocated[i] = 2 * i + 1;
    }
    return addressAllocated;
}

char *charArray(int size)
{
    size_t sizeRequiredForChar = size * sizeof(char);
    pthread_create(&threadAlloc, NULL, alloc, &sizeRequiredForChar);
    void *valueReturnedFromThreadAlloc;
    pthread_join(threadAlloc, &valueReturnedFromThreadAlloc);
    char *addressAllocated = (char *)valueReturnedFromThreadAlloc;

    for (int i = 0; i < size; i++)
    {
        addressAllocated[i] = 'X';
    }
    return addressAllocated;
}

float *floatArray(int size)
{
    size_t sizeRequiredForFloat = size * sizeof(float);
    pthread_create(&threadAlloc, NULL, alloc, &sizeRequiredForFloat);
    void *valueReturnedFromThreadAlloc;
    pthread_join(threadAlloc, &valueReturnedFromThreadAlloc);
    float *addressAllocated = (float *)valueReturnedFromThreadAlloc;

    for (int i = 0; i < size; i++)
    {
        addressAllocated[i] = i + 1;
    }
    return addressAllocated;
}
