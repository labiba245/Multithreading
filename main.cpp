#include "dataGenerator.h"
#include <sys/resource.h>
#include <cstring>
#include <list>

#define EXIT_SUCCESS 0
#define SMALL_ARRAY_SIZE 10
#define MEDIUM_ARRAY_SIZE 1000
#define LARGE_ARRAY_SIZE 100000
// will change numOfIteration for experimental puporses
#define numOfIteration 20
extern int option;
extern std::list<Chunk> allocated, freed;

using std::cout;
using std::endl;
using std::string;

int main(int argc, char **argv)
{

    if (argc == 1)
        option = FF;
    else
    {
        if (strcmp(argv[1], "BF") == 0)
            option = BF;
        else if (strcmp(argv[1], "WF") == 0)
            option = WF;
        else if (strcmp(argv[1], "FF") == 0)
        {
            option = FF;
        }
        else
        {
            cout << "***INVALID COMMAND***" << endl;
            cout << "./main FF or ./main for first fit" << endl;
            cout << "./main BF for best fit" << endl;
            cout << "./main WF for worst fit" << endl;
            cout << "Please check README.md file" << endl;
            exit(0);
        }
    }
    pthread_t threadDealloc;

    initialiseLocks();

    //small integer arrays
    double memoryUsage1 = 0;
    double userTime1 = 0;
    double systemTime1 = 0;
    double memoryUsageArray1[numOfIteration];
    double uTimeArray1[numOfIteration];
    double sTimeArray1[numOfIteration];

    cout << "\n---------------------------------------------------------------\n";
    cout << "Allocation and Deallocation for small array of integers\n";
    cout << "---------------------------------------------------------------\n";
    for (int i = 0; i < numOfIteration; i++)
    {
        int *smallIntArray1 = (int *)intArray(SMALL_ARRAY_SIZE * 3);
        int *smallIntArray2 = (int *)intArray(SMALL_ARRAY_SIZE * 2);
        int *smallIntArray3 = (int *)arrayOfOddIntegers(SMALL_ARRAY_SIZE * 4);
        int *smallIntArray4 = (int *)intArray(SMALL_ARRAY_SIZE * 5);

        pthread_create(&threadDealloc, NULL, dealloc, smallIntArray2);
        pthread_join(threadDealloc, NULL);

        pthread_create(&threadDealloc, NULL, dealloc, smallIntArray4);
        pthread_join(threadDealloc, NULL);

        pthread_create(&threadDealloc, NULL, dealloc, smallIntArray1);
        pthread_join(threadDealloc, NULL);

        int *smallIntArray5 = (int *)intArray(SMALL_ARRAY_SIZE * 3);

        struct rusage usage1;
        struct rusage r_usage1;
        getrusage(RUSAGE_SELF, &usage1);
        getrusage(RUSAGE_SELF, &r_usage1);

        memoryUsage1 += r_usage1.ru_maxrss;
        memoryUsageArray1[i] = r_usage1.ru_maxrss;
        uTimeArray1[i] = r_usage1.ru_utime.tv_usec;
        sTimeArray1[i] = r_usage1.ru_stime.tv_usec;
        userTime1 += usage1.ru_utime.tv_usec;
        systemTime1 += usage1.ru_stime.tv_usec;

        pthread_create(&threadDealloc, NULL, dealloc, smallIntArray3);
        pthread_join(threadDealloc, NULL);

        pthread_create(&threadDealloc, NULL, dealloc, smallIntArray5);
        pthread_join(threadDealloc, NULL);
        allocated.clear();
        freed.clear();
    }

    cout << "\n**********Memory and Time consumption**********\n";
    cout << "Mem"
         << "\t"
         << "sTime"
         << "\t"
         << "uTime \n";
    for (int j = 0; j < numOfIteration; j++)
    {
        cout << memoryUsageArray1[j] << "\t" << sTimeArray1[j] << "\t" << uTimeArray1[j] << endl;
    }

    cout << "\n\nAverage Memory Usage for small integer array: " << memoryUsage1 / numOfIteration << endl;
    cout << "Average system time: " << systemTime1 / numOfIteration << endl;
    cout << "Average user time: " << userTime1 / numOfIteration << endl;
    cout << "Press enter to continue! ";
    getchar();

    //small double array

    double memoryUsage2 = 0;
    double userTime2 = 0;
    double systemTime2 = 0;
    double memoryUsageArray2[numOfIteration];
    double uTimeArray2[numOfIteration];
    double sTimeArray2[numOfIteration];

    cout << "\n------------------------------------------------------------\n";
    cout << "Allocation and Deallocation for small array of doubles\n";
    cout << "------------------------------------------------------------\n";
    for (int i = 0; i < numOfIteration; i++)
    {
        double *smallDoubleArray1 = (double *)doubleArray(SMALL_ARRAY_SIZE * 3);
        double *smallDoubleArray2 = (double *)doubleArray(SMALL_ARRAY_SIZE * 2);
        double *smallDoubleArray3 = (double *)doubleArray(SMALL_ARRAY_SIZE * 4);
        double *smallDoubleArray4 = (double *)doubleArray(SMALL_ARRAY_SIZE * 5);

        pthread_create(&threadDealloc, NULL, dealloc, smallDoubleArray2);
        pthread_join(threadDealloc, NULL);

        pthread_create(&threadDealloc, NULL, dealloc, smallDoubleArray4);
        pthread_join(threadDealloc, NULL);

        pthread_create(&threadDealloc, NULL, dealloc, smallDoubleArray1);
        pthread_join(threadDealloc, NULL);

        double *smallDoubleArray5 = (double *)doubleArray(SMALL_ARRAY_SIZE * 3);

        struct rusage usage2;
        struct rusage r_usage2;
        getrusage(RUSAGE_SELF, &usage2);
        getrusage(RUSAGE_SELF, &r_usage2);
        memoryUsage2 += r_usage2.ru_maxrss;
        memoryUsageArray2[i] = r_usage2.ru_maxrss;
        uTimeArray2[i] = r_usage2.ru_utime.tv_usec;
        sTimeArray2[i] = r_usage2.ru_stime.tv_usec;
        userTime2 += usage2.ru_utime.tv_usec;
        systemTime2 += usage2.ru_stime.tv_usec;

        pthread_create(&threadDealloc, NULL, dealloc, smallDoubleArray3);
        pthread_join(threadDealloc, NULL);

        pthread_create(&threadDealloc, NULL, dealloc, smallDoubleArray5);
        pthread_join(threadDealloc, NULL);
        allocated.clear();
        freed.clear();
    }

    cout << "\n**********Memory and Time consumption**********\n";
    cout << "Mem"
         << "\t"
         << "sTime"
         << "\t"
         << "uTime \n";
    for (int j = 0; j < numOfIteration; j++)
    {
        cout << memoryUsageArray2[j] << "\t" << sTimeArray2[j] << "\t" << uTimeArray2[j] << endl;
    }

    cout << "\n\nAverage Memory Usage for small double array: " << memoryUsage2 / numOfIteration << endl;
    cout << "Average system time: " << systemTime2 / numOfIteration << endl;
    cout << "Average user time: " << userTime2 / numOfIteration << endl;
    cout << "Press enter to continue! ";
    getchar();

    //small characater array

    double memoryUsage3 = 0;
    double userTime3 = 0;
    double systemTime3 = 0;
    double memoryUsageArray3[numOfIteration];
    double uTimeArray3[numOfIteration];
    double sTimeArray3[numOfIteration];

    cout << "\n--------------------------------------------------------------\n";
    cout << "Allocation and Deallocation for small array of characters\n";
    cout << "--------------------------------------------------------------\n";
    for (int i = 0; i < numOfIteration; i++)
    {
        char *smallCharArray1 = (char *)charArray(SMALL_ARRAY_SIZE * 3);
        char *smallCharArray2 = (char *)charArray(SMALL_ARRAY_SIZE * 2);
        char *smallCharArray3 = (char *)charArray(SMALL_ARRAY_SIZE * 4);
        char *smallCharArray4 = (char *)charArray(SMALL_ARRAY_SIZE * 5);

        pthread_create(&threadDealloc, NULL, dealloc, smallCharArray2);
        pthread_join(threadDealloc, NULL);

        pthread_create(&threadDealloc, NULL, dealloc, smallCharArray4);
        pthread_join(threadDealloc, NULL);

        pthread_create(&threadDealloc, NULL, dealloc, smallCharArray1);
        pthread_join(threadDealloc, NULL);

        char *smallCharArray5 = (char *)charArray(SMALL_ARRAY_SIZE * 3);

        struct rusage usage3;
        struct rusage r_usage3;
        getrusage(RUSAGE_SELF, &usage3);
        getrusage(RUSAGE_SELF, &r_usage3);
        memoryUsage3 += r_usage3.ru_maxrss;
        memoryUsageArray3[i] = r_usage3.ru_maxrss;
        uTimeArray3[i] = r_usage3.ru_utime.tv_usec;
        sTimeArray3[i] = r_usage3.ru_stime.tv_usec;
        userTime3 += usage3.ru_utime.tv_usec;
        systemTime3 += usage3.ru_stime.tv_usec;

        pthread_create(&threadDealloc, NULL, dealloc, smallCharArray3);
        pthread_join(threadDealloc, NULL);

        pthread_create(&threadDealloc, NULL, dealloc, smallCharArray5);
        pthread_join(threadDealloc, NULL);

        allocated.clear();
        freed.clear();
    }
    cout << "Mem"
         << "\t"
         << "sTime"
         << "\t"
         << "uTime \n";
    for (int j = 0; j < numOfIteration; j++)
    {
        cout << memoryUsageArray3[j] << "\t" << sTimeArray3[j] << "\t" << uTimeArray3[j] << endl;
    }

    cout << "\n\nAverage Memory Usage for small character array: " << memoryUsage3 / numOfIteration << endl;
    cout << "Average system time: " << systemTime3 / numOfIteration << endl;
    cout << "Average user time: " << userTime3 / numOfIteration << endl;
    cout << "Press enter to continue! ";
    getchar();

    //small float array

    double memoryUsage4 = 0;
    double userTime4 = 0;
    double systemTime4 = 0;
    double memoryUsageArray4[numOfIteration];
    double uTimeArray4[numOfIteration];
    double sTimeArray4[numOfIteration];

    cout << "\n------------------------------------------------------------\n";
    cout << "Allocation and Deallocation for small array of floats\n";
    cout << "------------------------------------------------------------\n";
    for (int i = 0; i < numOfIteration; i++)
    {
        float *smallFloatArray1 = (float *)floatArray(SMALL_ARRAY_SIZE * 3);
        float *smallFloatArray2 = (float *)floatArray(SMALL_ARRAY_SIZE * 2);
        float *smallFloatArray3 = (float *)floatArray(SMALL_ARRAY_SIZE * 4);
        float *smallFloatArray4 = (float *)floatArray(SMALL_ARRAY_SIZE * 5);

        pthread_create(&threadDealloc, NULL, dealloc, smallFloatArray2);
        pthread_join(threadDealloc, NULL);

        pthread_create(&threadDealloc, NULL, dealloc, smallFloatArray4);
        pthread_join(threadDealloc, NULL);

        pthread_create(&threadDealloc, NULL, dealloc, smallFloatArray1);
        pthread_join(threadDealloc, NULL);

        float *smallFloatArray5 = (float *)floatArray(SMALL_ARRAY_SIZE * 3);

        struct rusage usage4;
        struct rusage r_usage4;
        getrusage(RUSAGE_SELF, &usage4);
        getrusage(RUSAGE_SELF, &r_usage4);
        memoryUsage4 += r_usage4.ru_maxrss;
        memoryUsageArray4[i] = r_usage4.ru_maxrss;
        uTimeArray4[i] = r_usage4.ru_utime.tv_usec;
        sTimeArray4[i] = r_usage4.ru_stime.tv_usec;
        userTime4 += usage4.ru_utime.tv_usec;
        systemTime4 += usage4.ru_stime.tv_usec;

        pthread_create(&threadDealloc, NULL, dealloc, smallFloatArray3);
        pthread_join(threadDealloc, NULL);

        pthread_create(&threadDealloc, NULL, dealloc, smallFloatArray5);
        pthread_join(threadDealloc, NULL);
        allocated.clear();
        freed.clear();
    }
    cout << "Mem"
         << "\t"
         << "sTime"
         << "\t"
         << "uTime \n";
    for (int j = 0; j < numOfIteration; j++)
    {
        cout << memoryUsageArray4[j] << "\t" << sTimeArray4[j] << "\t" << uTimeArray4[j] << endl;
    }

    cout << "\n\nAverage Memory Usage for small float array: " << memoryUsage4 / numOfIteration << endl;
    cout << "Average system time: " << systemTime4 / numOfIteration << endl;
    cout << "Average user time: " << userTime4 / numOfIteration << endl;
    cout << "Press enter to continue! ";
    getchar();

    //small array of different data types

    double memoryUsage5 = 0;
    double userTime5 = 0;
    double systemTime5 = 0;
    double memoryUsageArray5[numOfIteration];
    double uTimeArray5[numOfIteration];
    double sTimeArray5[numOfIteration];

    cout << "\n----------------------------------------------------------------------\n";
    cout << "Allocation and Deallocation for small array of different data types\n";
    cout << "----------------------------------------------------------------------\n";
    for (int i = 0; i < numOfIteration; i++)
    {
        float *smallFloatArray = (float *)floatArray(SMALL_ARRAY_SIZE * 3);
        double *smallDoubleArray = (double *)doubleArray(SMALL_ARRAY_SIZE * 2);
        char *smallCharArray = (char *)charArray(SMALL_ARRAY_SIZE * 4);
        int *smallIntArray = (int *)intArray(SMALL_ARRAY_SIZE * 5);

        pthread_create(&threadDealloc, NULL, dealloc, smallDoubleArray);
        pthread_join(threadDealloc, NULL);

        pthread_create(&threadDealloc, NULL, dealloc, smallIntArray);
        pthread_join(threadDealloc, NULL);

        pthread_create(&threadDealloc, NULL, dealloc, smallFloatArray);
        pthread_join(threadDealloc, NULL);

        float *smallFloatArray6 = (float *)floatArray(SMALL_ARRAY_SIZE * 3);

        struct rusage usage5;
        struct rusage r_usage5;
        getrusage(RUSAGE_SELF, &usage5);
        getrusage(RUSAGE_SELF, &r_usage5);
        memoryUsage5 += r_usage5.ru_maxrss;
        memoryUsageArray5[i] = r_usage5.ru_maxrss;
        uTimeArray5[i] = r_usage5.ru_utime.tv_usec;
        sTimeArray5[i] = r_usage5.ru_stime.tv_usec;
        userTime5 += usage5.ru_utime.tv_usec;
        systemTime5 += usage5.ru_stime.tv_usec;

        pthread_create(&threadDealloc, NULL, dealloc, smallCharArray);
        pthread_join(threadDealloc, NULL);

        pthread_create(&threadDealloc, NULL, dealloc, smallFloatArray6);
        pthread_join(threadDealloc, NULL);
        allocated.clear();
        freed.clear();
    }
    cout << "Mem"
         << "\t"
         << "sTime"
         << "\t"
         << "uTime  \n";
    for (int j = 0; j < numOfIteration; j++)
    {
        cout << memoryUsageArray5[j] << "\t" << sTimeArray5[j] << "\t" << uTimeArray5[j] << endl;
    }

    cout << "\n\nAverage Memory Usage for small  array  of different data types: " << memoryUsage5 / numOfIteration << endl;
    cout << "Average system time: " << systemTime5 / numOfIteration << endl;
    cout << "Average user time: " << userTime5 / numOfIteration << endl;
    cout << "Press enter to continue ";
    getchar();

    //medium integer arrays

    double memoryUsage6 = 0;
    double userTime6 = 0;
    double systemTime6 = 0;
    double memoryUsageArray6[numOfIteration];
    double uTimeArray6[numOfIteration];
    double sTimeArray6[numOfIteration];

    cout << "\n-----------------------------------------------------------------\n";
    cout << "Allocation and Deallocation for medium array of integers\n";
    cout << "-----------------------------------------------------------------\n";
    for (int i = 0; i < numOfIteration; i++)
    {
        int *mediumIntArray1 = (int *)intArray(MEDIUM_ARRAY_SIZE * 3);
        int *mediumIntArray2 = (int *)intArray(MEDIUM_ARRAY_SIZE * 2);
        int *mediumIntArray3 = (int *)arrayOfOddIntegers(MEDIUM_ARRAY_SIZE * 4);
        int *mediumIntArray4 = (int *)intArray(MEDIUM_ARRAY_SIZE * 5);

        pthread_create(&threadDealloc, NULL, dealloc, mediumIntArray2);
        pthread_join(threadDealloc, NULL);

        pthread_create(&threadDealloc, NULL, dealloc, mediumIntArray4);
        pthread_join(threadDealloc, NULL);

        pthread_create(&threadDealloc, NULL, dealloc, mediumIntArray1);
        pthread_join(threadDealloc, NULL);

        int *mediumIntArray5 = (int *)intArray(MEDIUM_ARRAY_SIZE * 3);

        struct rusage usage6;
        struct rusage r_usage6;
        getrusage(RUSAGE_SELF, &usage6);
        getrusage(RUSAGE_SELF, &r_usage6);
        memoryUsage6 += r_usage6.ru_maxrss;
        memoryUsageArray6[i] = r_usage6.ru_maxrss;
        uTimeArray6[i] = r_usage6.ru_utime.tv_usec;
        sTimeArray6[i] = r_usage6.ru_stime.tv_usec;
        userTime6 += usage6.ru_utime.tv_usec;
        systemTime6 += usage6.ru_stime.tv_usec;

        pthread_create(&threadDealloc, NULL, dealloc, mediumIntArray3);
        pthread_join(threadDealloc, NULL);

        pthread_create(&threadDealloc, NULL, dealloc, mediumIntArray5);
        pthread_join(threadDealloc, NULL);
        allocated.clear();
        freed.clear();
    }
    cout << "Mem"
         << "\t"
         << "sTime"
         << "\t"
         << "uTime \n";
    for (int j = 0; j < numOfIteration; j++)
    {
        cout << memoryUsageArray6[j] << "\t" << sTimeArray6[j] << "\t" << uTimeArray6[j] << endl;
    }

    cout << "\n\nAverage Memory Usage  for medium integer array: " << memoryUsage6 / numOfIteration << endl;
    cout << "Average system time: " << systemTime6 / numOfIteration << endl;
    cout << "Average user time: " << userTime6 / numOfIteration << endl;
    cout << "Press enter to continue! ";
    getchar();

    //medium double array

    double memoryUsage7 = 0;
    double userTime7 = 0;
    double systemTime7 = 0;
    double memoryUsageArray7[numOfIteration];
    double uTimeArray7[numOfIteration];
    double sTimeArray7[numOfIteration];

    cout << "\n--------------------------------------------------------------\n";
    cout << "Allocation and Deallocation for medium array of doubles\n";
    cout << "--------------------------------------------------------------\n";
    for (int i = 0; i < numOfIteration; i++)
    {
        double *mediumDoubleArray1 = (double *)doubleArray(MEDIUM_ARRAY_SIZE * 3);
        double *mediumDoubleArray2 = (double *)doubleArray(MEDIUM_ARRAY_SIZE * 2);
        double *mediumDoubleArray3 = (double *)doubleArray(MEDIUM_ARRAY_SIZE * 4);
        double *mediumDoubleArray4 = (double *)doubleArray(MEDIUM_ARRAY_SIZE * 5);

        pthread_create(&threadDealloc, NULL, dealloc, mediumDoubleArray2);
        pthread_join(threadDealloc, NULL);

        pthread_create(&threadDealloc, NULL, dealloc, mediumDoubleArray4);
        pthread_join(threadDealloc, NULL);

        pthread_create(&threadDealloc, NULL, dealloc, mediumDoubleArray1);
        pthread_join(threadDealloc, NULL);

        double *mediumDoubleArray5 = (double *)doubleArray(MEDIUM_ARRAY_SIZE * 3);

        struct rusage usage7;
        struct rusage r_usage7;
        getrusage(RUSAGE_SELF, &usage7);
        getrusage(RUSAGE_SELF, &r_usage7);
        memoryUsage7 += r_usage7.ru_maxrss;
        memoryUsageArray7[i] = r_usage7.ru_maxrss;
        uTimeArray7[i] = r_usage7.ru_utime.tv_usec;
        sTimeArray7[i] = r_usage7.ru_stime.tv_usec;
        userTime7 += usage7.ru_utime.tv_usec;
        systemTime7 += usage7.ru_stime.tv_usec;

        pthread_create(&threadDealloc, NULL, dealloc, mediumDoubleArray3);
        pthread_join(threadDealloc, NULL);

        pthread_create(&threadDealloc, NULL, dealloc, mediumDoubleArray5);
        pthread_join(threadDealloc, NULL);
        allocated.clear();
        freed.clear();
    }
    cout << "Mem"
         << "\t"
         << "sTime"
         << "\t"
         << "uTime  \n";
    for (int j = 0; j < numOfIteration; j++)
    {
        cout << memoryUsageArray7[j] << "\t" << sTimeArray7[j] << "\t" << uTimeArray7[j] << endl;
    }

    cout << "\n\nAverage Memory Usage for medium double array: " << memoryUsage7 / numOfIteration << endl;
    cout << "Average system time: " << systemTime7 / numOfIteration << endl;
    cout << "Average user time: " << userTime7 / numOfIteration << endl;
    cout << "Press enter to continue! ";
    getchar();

    //medium character array

    double memoryUsage8 = 0;
    double userTime8 = 0;
    double systemTime8 = 0;
    double memoryUsageArray8[numOfIteration];
    double uTimeArray8[numOfIteration];
    double sTimeArray8[numOfIteration];

    cout << "\n-------------------------------------------------------------------\n";
    cout << "Allocation and Deallocation for medium array of characters\n";
    cout << "-------------------------------------------------------------------\n";
    for (int i = 0; i < numOfIteration; i++)
    {
        char *mediumCharArray1 = (char *)charArray(MEDIUM_ARRAY_SIZE * 3);
        char *mediumCharArray2 = (char *)charArray(MEDIUM_ARRAY_SIZE * 2);
        char *mediumCharArray3 = (char *)charArray(MEDIUM_ARRAY_SIZE * 4);
        char *mediumCharArray4 = (char *)charArray(MEDIUM_ARRAY_SIZE * 5);

        pthread_create(&threadDealloc, NULL, dealloc, mediumCharArray2);
        pthread_join(threadDealloc, NULL);

        pthread_create(&threadDealloc, NULL, dealloc, mediumCharArray4);
        pthread_join(threadDealloc, NULL);

        pthread_create(&threadDealloc, NULL, dealloc, mediumCharArray1);
        pthread_join(threadDealloc, NULL);

        char *mediumCharArray5 = (char *)charArray(MEDIUM_ARRAY_SIZE * 3);

        struct rusage usage8;
        struct rusage r_usage8;
        getrusage(RUSAGE_SELF, &usage8);
        getrusage(RUSAGE_SELF, &r_usage8);
        memoryUsage8 += r_usage8.ru_maxrss;
        memoryUsageArray8[i] = r_usage8.ru_maxrss;
        uTimeArray8[i] = r_usage8.ru_utime.tv_usec;
        sTimeArray8[i] = r_usage8.ru_stime.tv_usec;
        userTime8 += usage8.ru_utime.tv_usec;
        systemTime8 += usage8.ru_stime.tv_usec;

        pthread_create(&threadDealloc, NULL, dealloc, mediumCharArray3);
        pthread_join(threadDealloc, NULL);

        pthread_create(&threadDealloc, NULL, dealloc, mediumCharArray5);
        pthread_join(threadDealloc, NULL);

        allocated.clear();
        freed.clear();
    }
    cout << "Mem"
         << "\t"
         << "sTime"
         << "\t"
         << "uTime \n";
    for (int j = 0; j < numOfIteration; j++)
    {
        cout << memoryUsageArray8[j] << "\t" << sTimeArray8[j] << "\t" << uTimeArray8[j] << endl;
    }

    cout << "\n\nAverage Memory Usage for medium character array: " << memoryUsage8 / numOfIteration << endl;
    cout << "Average system time: " << systemTime8 / numOfIteration << endl;
    cout << "Average user time: " << userTime8 / numOfIteration << endl;
    cout << "Press enter to continue!";
    getchar();

    //medium float array

    double memoryUsage9 = 0;
    double userTime9 = 0;
    double systemTime9 = 0;
    double memoryUsageArray9[numOfIteration];
    double uTimeArray9[numOfIteration];
    double sTimeArray9[numOfIteration];

    cout << "\n---------------------------------------------------------------\n";
    cout << "Allocation and Deallocation for medium array of floats\n";
    cout << "--------------------------------------------------------------\n";
    for (int i = 0; i < numOfIteration; i++)
    {

        float *mediumFloatArray1 = (float *)floatArray(MEDIUM_ARRAY_SIZE * 3);
        float *mediumFloatArray2 = (float *)floatArray(MEDIUM_ARRAY_SIZE * 2);
        float *mediumFloatArray3 = (float *)floatArray(MEDIUM_ARRAY_SIZE * 4);
        float *mediumFloatArray4 = (float *)floatArray(MEDIUM_ARRAY_SIZE * 5);

        pthread_create(&threadDealloc, NULL, dealloc, mediumFloatArray2);
        pthread_join(threadDealloc, NULL);

        pthread_create(&threadDealloc, NULL, dealloc, mediumFloatArray4);
        pthread_join(threadDealloc, NULL);

        pthread_create(&threadDealloc, NULL, dealloc, mediumFloatArray1);
        pthread_join(threadDealloc, NULL);

        float *mediumFloatArray5 = (float *)floatArray(MEDIUM_ARRAY_SIZE * 3);

        struct rusage usage9;
        struct rusage r_usage9;
        getrusage(RUSAGE_SELF, &usage9);
        getrusage(RUSAGE_SELF, &r_usage9);
        memoryUsage9 += r_usage9.ru_maxrss;
        memoryUsageArray9[i] = r_usage9.ru_maxrss;
        uTimeArray9[i] = r_usage9.ru_utime.tv_usec;
        sTimeArray9[i] = r_usage9.ru_stime.tv_usec;
        userTime9 += usage9.ru_utime.tv_usec;
        systemTime9 += usage9.ru_stime.tv_usec;

        pthread_create(&threadDealloc, NULL, dealloc, mediumFloatArray3);
        pthread_join(threadDealloc, NULL);

        pthread_create(&threadDealloc, NULL, dealloc, mediumFloatArray5);
        pthread_join(threadDealloc, NULL);
        allocated.clear();
        freed.clear();
    }
    cout << "Mem"
         << "\t"
         << "sTime"
         << "\t"
         << "uTime \n";
    for (int j = 0; j < numOfIteration; j++)
    {
        cout << memoryUsageArray9[j] << "\t" << sTimeArray9[j] << "\t" << uTimeArray9[j] << endl;
    }

    cout << "\n\nAverage Memory Usage for medium float array: " << memoryUsage9 / numOfIteration << endl;
    cout << "Average system time: " << systemTime9 / numOfIteration << endl;
    cout << "Average user time: " << userTime9 / numOfIteration << endl;
    cout << "Press enter to continue!";
    getchar();

    //medium array of different types

    double memoryUsage10 = 0;
    double userTime10 = 0;
    double systemTime10 = 0;
    double memoryUsageArray10[numOfIteration];
    double uTimeArray10[numOfIteration];
    double sTimeArray10[numOfIteration];

    cout << "\n----------------------------------------------------------------------\n";
    cout << "Allocation and Deallocation for medium array of different data types\n";
    cout << "----------------------------------------------------------------------\n";
    for (int i = 0; i < numOfIteration; i++)
    {
        float *mediumFloatArray = (float *)floatArray(MEDIUM_ARRAY_SIZE * 3);
        double *mediumDoubleArray = (double *)doubleArray(MEDIUM_ARRAY_SIZE * 2);
        char *mediumCharArray = (char *)charArray(MEDIUM_ARRAY_SIZE * 4);
        int *mediumIntArray = (int *)intArray(MEDIUM_ARRAY_SIZE * 5);

        pthread_create(&threadDealloc, NULL, dealloc, mediumDoubleArray);
        pthread_join(threadDealloc, NULL);

        pthread_create(&threadDealloc, NULL, dealloc, mediumIntArray);
        pthread_join(threadDealloc, NULL);

        pthread_create(&threadDealloc, NULL, dealloc, mediumFloatArray);
        pthread_join(threadDealloc, NULL);

        float *mediumFloatArray6 = (float *)floatArray(MEDIUM_ARRAY_SIZE * 3);

        struct rusage usage10;
        struct rusage r_usage10;
        getrusage(RUSAGE_SELF, &usage10);
        getrusage(RUSAGE_SELF, &r_usage10);
        memoryUsage10 += r_usage10.ru_maxrss;
        memoryUsageArray10[i] = r_usage10.ru_maxrss;
        uTimeArray10[i] = r_usage10.ru_utime.tv_usec;
        sTimeArray10[i] = r_usage10.ru_stime.tv_usec;
        userTime10 += usage10.ru_utime.tv_usec;
        systemTime10 += usage10.ru_stime.tv_usec;

        pthread_create(&threadDealloc, NULL, dealloc, mediumCharArray);
        pthread_join(threadDealloc, NULL);

        pthread_create(&threadDealloc, NULL, dealloc, mediumFloatArray6);
        pthread_join(threadDealloc, NULL);
        allocated.clear();
        freed.clear();
    }
    cout << "Mem"
         << "\t"
         << "sTime"
         << "\t"
         << "uTime \n";
    for (int j = 0; j < numOfIteration; j++)
    {
        cout << memoryUsageArray10[j] << "\t" << sTimeArray10[j] << "\t" << uTimeArray10[j] << endl;
    }

    cout << "\n\nAverage Memory Usage for medium array of different data types: " << memoryUsage10 / numOfIteration << endl;
    cout << "Average system time: " << systemTime10 / numOfIteration << endl;
    cout << "Average user time: " << userTime10 / numOfIteration << endl;
    cout << "Press enter to continue! ";
    getchar();

    //large integer array

    double memoryUsage11 = 0;
    double userTime11 = 0;
    double systemTime11 = 0;
    double memoryUsageArray11[numOfIteration];
    double uTimeArray11[numOfIteration];
    double sTimeArray11[numOfIteration];

    cout << "\n--------------------------------------------------------------------\n";
    cout << "Allocation and Deallocation for large array of integers\n";
    cout << "--------------------------------------------------------------------\n";
    for (int i = 0; i < numOfIteration; i++)
    {
        int *largeIntArray1 = (int *)intArray(LARGE_ARRAY_SIZE * 3);
        int *largeIntArray2 = (int *)intArray(MEDIUM_ARRAY_SIZE * 2);
        int *largeIntArray3 = (int *)arrayOfOddIntegers(LARGE_ARRAY_SIZE * 4);
        int *largeIntArray4 = (int *)intArray(LARGE_ARRAY_SIZE * 5);

        pthread_create(&threadDealloc, NULL, dealloc, largeIntArray2);
        pthread_join(threadDealloc, NULL);

        pthread_create(&threadDealloc, NULL, dealloc, largeIntArray4);
        pthread_join(threadDealloc, NULL);

        pthread_create(&threadDealloc, NULL, dealloc, largeIntArray1);
        pthread_join(threadDealloc, NULL);

        int *largeIntArray5 = (int *)intArray(LARGE_ARRAY_SIZE * 3);

        struct rusage usage11;
        struct rusage r_usage11;
        getrusage(RUSAGE_SELF, &usage11);
        getrusage(RUSAGE_SELF, &r_usage11);
        memoryUsage11 += r_usage11.ru_maxrss;
        memoryUsageArray11[i] = r_usage11.ru_maxrss;
        uTimeArray11[i] = r_usage11.ru_utime.tv_usec;
        sTimeArray11[i] = r_usage11.ru_stime.tv_usec;
        userTime11 += usage11.ru_utime.tv_usec;
        systemTime11 += usage11.ru_stime.tv_usec;

        pthread_create(&threadDealloc, NULL, dealloc, largeIntArray3);
        pthread_join(threadDealloc, NULL);

        pthread_create(&threadDealloc, NULL, dealloc, largeIntArray5);
        pthread_join(threadDealloc, NULL);
        allocated.clear();
        freed.clear();
    }
    cout << "Mem"
         << "\t"
         << "sTime"
         << "\t"
         << "uTime  \n";
    for (int j = 0; j < numOfIteration; j++)
    {
        cout << memoryUsageArray11[j] << "\t" << sTimeArray11[j] << "\t" << uTimeArray11[j] << endl;
    }

    cout << "\n\nAverage Memory Usage for large integer array: " << memoryUsage11 / numOfIteration << endl;
    cout << "Average system time: " << systemTime11 / numOfIteration << endl;
    cout << "Average user time: " << userTime11 / numOfIteration << endl;
    cout << "Press enter to continue! ";
    getchar();

    //large double array

    double memoryUsage12 = 0;
    double userTime12 = 0;
    double systemTime12 = 0;
    double memoryUsageArray12[numOfIteration];
    double uTimeArray12[numOfIteration];
    double sTimeArray12[numOfIteration];

    cout << "\n-----------------------------------------------------------------\n";
    cout << "Allocation and Deallocation for large array of doubles\n";
    cout << "-----------------------------------------------------------------\n";
    for (int i = 0; i < numOfIteration; i++)
    {
        double *largeDoubleArray1 = (double *)doubleArray(LARGE_ARRAY_SIZE * 3);
        double *largeDoubleArray2 = (double *)doubleArray(LARGE_ARRAY_SIZE * 2);
        double *largeDoubleArray3 = (double *)doubleArray(LARGE_ARRAY_SIZE * 4);
        double *largeDoubleArray4 = (double *)doubleArray(LARGE_ARRAY_SIZE * 5);

        pthread_create(&threadDealloc, NULL, dealloc, largeDoubleArray2);
        pthread_join(threadDealloc, NULL);

        pthread_create(&threadDealloc, NULL, dealloc, largeDoubleArray4);
        pthread_join(threadDealloc, NULL);

        pthread_create(&threadDealloc, NULL, dealloc, largeDoubleArray1);
        pthread_join(threadDealloc, NULL);

        double *largeDoubleArray5 = (double *)doubleArray(LARGE_ARRAY_SIZE * 3);

        struct rusage usage12;
        struct rusage r_usage12;
        getrusage(RUSAGE_SELF, &usage12);
        getrusage(RUSAGE_SELF, &r_usage12);
        memoryUsage12 += r_usage12.ru_maxrss;
        memoryUsageArray12[i] = r_usage12.ru_maxrss;
        uTimeArray12[i] = r_usage12.ru_utime.tv_usec;
        sTimeArray12[i] = r_usage12.ru_stime.tv_usec;
        userTime12 += usage12.ru_utime.tv_usec;
        systemTime12 += usage12.ru_stime.tv_usec;

        pthread_create(&threadDealloc, NULL, dealloc, largeDoubleArray3);
        pthread_join(threadDealloc, NULL);

        pthread_create(&threadDealloc, NULL, dealloc, largeDoubleArray5);
        pthread_join(threadDealloc, NULL);
        allocated.clear();
        freed.clear();
    }
    cout << "Mem"
         << "\t"
         << "sTime"
         << "\t"
         << "uTime \n";
    for (int j = 0; j < numOfIteration; j++)
    {
        cout << memoryUsageArray12[j] << "\t" << sTimeArray12[j] << "\t" << uTimeArray12[j] << endl;
    }

    cout << "\n\nAverage Memory Usage for large double array: " << memoryUsage12 / numOfIteration << endl;
    cout << "Average system time: " << systemTime12 / numOfIteration << endl;
    cout << "Average user time: " << userTime12 / numOfIteration << endl;
    cout << "Press enter to continue! ";
    getchar();

    //large char array

    double memoryUsage13 = 0;
    double userTime13 = 0;
    double systemTime13 = 0;
    double memoryUsageArray13[numOfIteration];
    double uTimeArray13[numOfIteration];
    double sTimeArray13[numOfIteration];

    cout << "\n---------------------------------------------------------------------\n";
    cout << "Allocation and Deallocation for large array of characters\n";
    cout << "---------------------------------------------------------------------\n";
    for (int i = 0; i < numOfIteration; i++)
    {
        char *largeCharArray1 = (char *)charArray(LARGE_ARRAY_SIZE * 3);
        char *largeCharArray2 = (char *)charArray(LARGE_ARRAY_SIZE * 2);
        char *largeCharArray3 = (char *)charArray(LARGE_ARRAY_SIZE * 4);
        char *largeCharArray4 = (char *)charArray(LARGE_ARRAY_SIZE * 5);

        pthread_create(&threadDealloc, NULL, dealloc, largeCharArray2);
        pthread_join(threadDealloc, NULL);

        pthread_create(&threadDealloc, NULL, dealloc, largeCharArray4);
        pthread_join(threadDealloc, NULL);

        pthread_create(&threadDealloc, NULL, dealloc, largeCharArray1);
        pthread_join(threadDealloc, NULL);

        char *largeCharArray5 = (char *)charArray(LARGE_ARRAY_SIZE * 3);

        struct rusage usage13;
        struct rusage r_usage13;
        getrusage(RUSAGE_SELF, &usage13);
        getrusage(RUSAGE_SELF, &r_usage13);
        memoryUsage13 += r_usage13.ru_maxrss;
        memoryUsageArray13[i] = r_usage13.ru_maxrss;
        uTimeArray13[i] = r_usage13.ru_utime.tv_usec;
        sTimeArray13[i] = r_usage13.ru_stime.tv_usec;
        userTime13 += usage13.ru_utime.tv_usec;
        systemTime13 += usage13.ru_stime.tv_usec;

        pthread_create(&threadDealloc, NULL, dealloc, largeCharArray3);
        pthread_join(threadDealloc, NULL);

        pthread_create(&threadDealloc, NULL, dealloc, largeCharArray5);
        pthread_join(threadDealloc, NULL);
        allocated.clear();
        freed.clear();
    }
    cout << "Mem"
         << "\t"
         << "sTime"
         << "\t"
         << "uTime \n";
    for (int j = 0; j < numOfIteration; j++)
    {
        cout << memoryUsageArray13[j] << "\t" << sTimeArray13[j] << "\t" << uTimeArray13[j] << endl;
    }

    cout << "\n\nAverage Memory Usage for large character array: " << memoryUsage13 / numOfIteration << endl;
    cout << "Average system time: " << systemTime13 / numOfIteration << endl;
    cout << "Average user time: " << userTime13 / numOfIteration << endl;
    cout << "Press enter to continue! ";
    getchar();

    //large float array

    double memoryUsage14 = 0;
    double userTime14 = 0;
    double systemTime14 = 0;
    double memoryUsageArray14[numOfIteration];
    double uTimeArray14[numOfIteration];
    double sTimeArray14[numOfIteration];

    cout << "\n---------------------------------------------------------------\n";
    cout << "Allocation and Deallocation for large array of floats\n";
    cout << "---------------------------------------------------------------\n";
    for (int i = 0; i < numOfIteration; i++)
    {
        float *largeFloatArray1 = (float *)floatArray(LARGE_ARRAY_SIZE * 3);
        float *largeFloatArray2 = (float *)floatArray(LARGE_ARRAY_SIZE * 2);
        float *largeFloatArray3 = (float *)floatArray(LARGE_ARRAY_SIZE * 4);
        float *largeFloatArray4 = (float *)floatArray(LARGE_ARRAY_SIZE * 5);

        pthread_create(&threadDealloc, NULL, dealloc, largeFloatArray2);
        pthread_join(threadDealloc, NULL);

        pthread_create(&threadDealloc, NULL, dealloc, largeFloatArray4);
        pthread_join(threadDealloc, NULL);

        pthread_create(&threadDealloc, NULL, dealloc, largeFloatArray1);
        pthread_join(threadDealloc, NULL);

        float *largeFloatArray5 = (float *)floatArray(LARGE_ARRAY_SIZE * 3);

        struct rusage usage14;
        struct rusage r_usage14;
        getrusage(RUSAGE_SELF, &usage14);
        getrusage(RUSAGE_SELF, &r_usage14);
        memoryUsage14 += r_usage14.ru_maxrss;
        memoryUsageArray14[i] = r_usage14.ru_maxrss;
        uTimeArray14[i] = r_usage14.ru_utime.tv_usec;
        sTimeArray14[i] = r_usage14.ru_stime.tv_usec;
        userTime14 += usage14.ru_utime.tv_usec;
        systemTime14 += usage14.ru_stime.tv_usec;

        pthread_create(&threadDealloc, NULL, dealloc, largeFloatArray3);
        pthread_join(threadDealloc, NULL);

        pthread_create(&threadDealloc, NULL, dealloc, largeFloatArray5);
        pthread_join(threadDealloc, NULL);
        allocated.clear();
        freed.clear();
    }
    cout << "Mem"
         << "\t"
         << "sTime"
         << "\t"
         << "uTime \n";
    for (int j = 0; j < numOfIteration; j++)
    {
        cout << memoryUsageArray14[j] << "\t" << sTimeArray14[j] << "\t" << uTimeArray14[j] << endl;
    }

    cout << "\n\nAverage Memory Usage for large float array: " << memoryUsage14 / numOfIteration << endl;
    cout << "Average system time: " << systemTime14 / numOfIteration << endl;
    cout << "Average user time: " << userTime14 / numOfIteration << endl;
    cout << "Press enter to continue! ";
    getchar();

    //large array of different data types

    double memoryUsage15 = 0;
    double userTime15 = 0;
    double systemTime15 = 0;
    double memoryUsageArray15[numOfIteration];
    double uTimeArray15[numOfIteration];
    double sTimeArray15[numOfIteration];

    cout << "\n----------------------------------------------------------------------\n";
    cout << "Allocation and Deallocation for large array of different data types\n";
    cout << "----------------------------------------------------------------------\n";
    for (int i = 0; i < numOfIteration; i++)
    {
        float *largeFloatArray = (float *)floatArray(LARGE_ARRAY_SIZE * 3);
        double *largeDoubleArray = (double *)doubleArray(LARGE_ARRAY_SIZE * 2);
        char *largeCharArray = (char *)charArray(LARGE_ARRAY_SIZE * 4);
        int *largeIntArray = (int *)intArray(LARGE_ARRAY_SIZE * 5);

        pthread_create(&threadDealloc, NULL, dealloc, largeDoubleArray);
        pthread_join(threadDealloc, NULL);

        pthread_create(&threadDealloc, NULL, dealloc, largeIntArray);
        pthread_join(threadDealloc, NULL);

        pthread_create(&threadDealloc, NULL, dealloc, largeFloatArray);
        pthread_join(threadDealloc, NULL);

        float *largeFloatArray6 = (float *)floatArray(LARGE_ARRAY_SIZE * 3);

        struct rusage usage15;
        struct rusage r_usage15;
        getrusage(RUSAGE_SELF, &usage15);
        getrusage(RUSAGE_SELF, &r_usage15);
        memoryUsage15 += r_usage15.ru_maxrss;
        memoryUsageArray15[i] = r_usage15.ru_maxrss;
        uTimeArray15[i] = r_usage15.ru_utime.tv_usec;
        sTimeArray15[i] = r_usage15.ru_stime.tv_usec;
        userTime15 += usage15.ru_utime.tv_usec;
        systemTime15 += usage15.ru_stime.tv_usec;

        pthread_create(&threadDealloc, NULL, dealloc, largeCharArray);
        pthread_join(threadDealloc, NULL);

        pthread_create(&threadDealloc, NULL, dealloc, largeFloatArray6);
        pthread_join(threadDealloc, NULL);
        allocated.clear();
        freed.clear();
    }
    cout << "Mem"
         << "\t"
         << "sTime"
         << "\t"
         << "uTime \n";
    for (int j = 0; j < numOfIteration; j++)
    {
        cout << memoryUsageArray15[j] << "\t" << sTimeArray15[j] << "\t" << uTimeArray15[j] << endl;
    }

    cout << "\n\nAverage Memory Usage for large array of different types: " << memoryUsage15 / numOfIteration << endl;
    cout << "Average system time: " << systemTime15 / numOfIteration << endl;
    cout << "Average user time: " << userTime15 / numOfIteration << endl;

    return EXIT_SUCCESS;
}
