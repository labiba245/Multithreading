#ifndef DATA_GENERATOR_H
#define DATA_GENERATOR_H

#include "allocation.h"
#include <string>
#include <iostream>

using std::string;

int *intArray(int size);

double *doubleArray(int size);

int *arrayOfOddIntegers(int size);

char *charArray(int size);

float *floatArray(int size);

#endif