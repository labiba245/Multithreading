#include <string>
#include <list>
#include <iostream>
#include "allocation.h"

#define EXIT_SUCCESS 0

using std::cout;
using std::endl;

std::list<Chunk> freed, allocated;

/*lockFreed is to allow mutual dexclusive access to Free list and lockAllocated 
is to allow the same for Allocated list
//for max concurrency, i have two locks for allocated list and 
free list so that multiple processes can use them simultaneously. */
pthread_mutex_t lockFreed, lockAllocated;
/*lockreadcountFreed is to access readcountFeree and 
lockreadcountAllocated is to access readcountAllocated */
pthread_mutex_t lockreadcountFreed, lockreadcountAllocated;

//Initially readcounts are zero
//two readcount locks to control their concurrency separately
int readcountFreed = 0;
int readcountAllocated = 0;

int option;

void initialiseLocks()
{
    // initialise the locks
    pthread_mutex_init(&lockFreed, NULL);
    pthread_mutex_init(&lockAllocated, NULL);
    pthread_mutex_init(&lockreadcountFreed, NULL);
    pthread_mutex_init(&lockreadcountAllocated, NULL);
}

void lockForReadingAllocatedList()
{
    pthread_mutex_trylock(&lockreadcountAllocated);
    readcountAllocated++;
    // checking if it is the first reader
    if (readcountAllocated == 1)
    {
        /* acquiring the lock so that by the time allocated 
        list is being read, it should not be available for updates */
        pthread_mutex_trylock(&lockAllocated);
    }
    /* so that other processors can also join the reading but only one 
    process can update the readcount at a time */
    pthread_mutex_unlock(&lockreadcountAllocated);
}

void unlockForReadingAllocatedList()
{
    //putting a lock so that only one process can modify readcountAllocated at a time
    pthread_mutex_trylock(&lockreadcountAllocated);
    readcountAllocated--;
    // checking if it is the last reader
    if (readcountAllocated == 0)
    {
        //releasing the lock on allocated list to be available for writing/modification
        pthread_mutex_unlock(&lockAllocated);
    }
    //releasing the lock so that readcountAllocated be accessed by other processors
    pthread_mutex_unlock(&lockreadcountAllocated);
}

void lockForReadingFreedList()
{
    pthread_mutex_trylock(&lockreadcountFreed);
    readcountFreed++;
    if (readcountFreed == 1)
    {
        pthread_mutex_trylock(&lockFreed);
    }
    pthread_mutex_unlock(&lockreadcountFreed);
}
void unlockForReadingFreedList()
{
    pthread_mutex_trylock(&lockreadcountFreed);
    readcountFreed--;
    if (readcountFreed == 0)
    {
        pthread_mutex_unlock(&lockFreed);
    }
    pthread_mutex_unlock(&lockreadcountFreed);
}

void *alloc(void *arg)
{
    size_t *argReceived = (size_t *)arg;
    size_t chunkSize = (size_t)*argReceived;
    void *ptr = NULL;
    bool found = false;
    Chunk chunkFound;
    int n = 0;

    /* putting an exclusive lock on free list for the following code, 
    even though it is just reading from the list
    since two processes should not access the list at the same time 
    for choosing the next chunk where the 
    new block of data has to be stored */

    pthread_mutex_trylock(&lockFreed);
    /* This lock will be relaesed after storing the data in 
    the selected chunk and removing the chunk from free list */

    if (option == FF)
    {
        /*traversing the linked list using an iterator
        till a sufficiently big memory block is found */
        for (std::list<Chunk>::iterator chunk = freed.begin(); chunk != freed.end();
             ++chunk)
        {
            n++;
            if (chunk->chunkSize >= chunkSize)
            {
                found = true;
                chunkFound = *chunk;
                break;
            }
        }
    }
    else if (option == BF)
    {
        int i = 0;
        size_t minBalSize = chunkSize;

        /*traversing the linked list using an iterator
        till a sufficiently big memory block is found */
        for (std::list<Chunk>::iterator chunk = freed.begin();
             chunk != freed.end(); ++chunk)
        {
            i++; //counting the nodes

            /* if the current chunk is greater than the required size and
            the difference is less than the minimum difference, we update the chunk found */
            if (chunk->chunkSize >= chunkSize &&
                (chunk->chunkSize - chunkSize) <= minBalSize)
            {
                chunkFound = *chunk;
                n = i;
                //updating the minumum difference
                minBalSize = chunk->chunkSize - chunkSize;
            }
        }
        //a chunk is found only if the balance is less than the required size
        if (minBalSize < chunkSize)
        {
            found = true;
        }
    }
    else
    {
        int i = 0;
        size_t maxChunkSize = 0;
        for (std::list<Chunk>::iterator chunk = freed.begin();
             chunk != freed.end(); ++chunk)
        {
            i++;
            if (chunk->chunkSize >= maxChunkSize)
            {
                chunkFound = *chunk;
                n = i;
                maxChunkSize = chunk->chunkSize;
            }
        }
        if (maxChunkSize >= chunkSize)
        {
            found = true;
        }
    }
    /*if a matching chunk is found in the freed linked list, 
    memory is allocated from that
    here we can have 2 cases: either the chunk is used as a whole 
    or further divided into two chunks */
    if (found)
    {

        /*Putting an exclusive lock on allocated lust since that need to be modified
        and only one process should be allowed to do so at  a time */
        pthread_mutex_trylock(&lockAllocated);
        //chunk is being used as a whole because sizes are same
        if (chunkFound.chunkSize == chunkSize)
            allocated.push_back(chunkFound);
        else
        {
            /* we create two partitions of the chunk, one is added to the 
            allocated list and the remaining portion is added to the freed list */
            Chunk c1 = {chunkFound.baseAddress, chunkSize};
            Chunk c2 = {(char *)chunkFound.baseAddress + chunkSize,
                        chunkFound.chunkSize - chunkSize};
            allocated.push_back(c1);
            freed.push_back(c2);
        }

        std::list<Chunk>::iterator chunkIterator = freed.begin();

        //to remove the selected chunk from the freed linked list
        while (--n)
        {
            chunkIterator++;
        }

        freed.erase(chunkIterator);

        /* since the operation is complete , we can release the locks on both 
        allocated list and free list, allowing other processes to use the same */
        pthread_mutex_unlock(&lockAllocated);
        pthread_mutex_unlock(&lockFreed);
        pthread_exit(chunkFound.baseAddress);
        // return chunkFound.baseAddress;
    }

    /*in case no chunk is found from the freed list,
    fresh memory is allocated from RAM */
    else
    {
        ptr = sbrk(0);
        sbrk(chunkSize);
        Chunk chk = {ptr, chunkSize};
        // locking the allocatied list to add a new chunk
        pthread_mutex_trylock(&lockAllocated);

        allocated.push_back(chk);
        // to release the lock after vadding the new chunk
        pthread_mutex_unlock(&lockAllocated);
        //     return ptr;
        pthread_exit(ptr);
    }
}
/*first check if the given address is in the allocated list, 
deallocate it and add it to the freed list
in case the address is not found, generate a fatal error
*/
void *dealloc(void *arg)
{
    void *chunk = arg;
    bool found = false;
    Chunk chunkToDeallocate;
    //to find the poition of the chunk to be deleted from the allocated list
    int positionOfChunkToDeallocate = 0;

    /* Putting a shared lock for the for loop which is just reading 
    from the allocated list to allow the operation to be performed
    by multiple readers at the same time */

    lockForReadingAllocatedList();
    /*critical section: since it is under shared lock, multiple readers 
    can read the allocated list, but no process can update it */
    for (std::list<Chunk>::iterator allocatedChunk = allocated.begin();
         allocatedChunk != allocated.end(); ++allocatedChunk)
    {
        ++positionOfChunkToDeallocate;
        if (allocatedChunk->baseAddress == chunk)
        {
            found = true;
            chunkToDeallocate = *allocatedChunk;
            break;
        }
    }
    //end of reader section

    unlockForReadingAllocatedList();
    if (found)
    {
        //Putting an exclusivde lock on both the lists to update exclusively
        pthread_mutex_trylock(&lockFreed);
        pthread_mutex_trylock(&lockAllocated);

        freed.push_back(chunkToDeallocate);
        std::list<Chunk>::iterator chunkIterator = allocated.begin();
        while (--positionOfChunkToDeallocate)
        {
            chunkIterator++;
        }

        allocated.erase(chunkIterator);
        //releasing the locks after the lists are updated
        pthread_mutex_unlock(&lockFreed);
        pthread_mutex_unlock(&lockAllocated);
    }
    else
    {
        cout << "Fatal error: can not deallocate a memory which was never allocated.\n";
        abort();
    }
    return EXIT_SUCCESS;
}

void *showStatus(void *arg)
{
    int totalAllocatedChunks = 0;
    int totalFreedChunks = 0;

    size_t totalAllocatedMem = 0;

    lockForReadingAllocatedList();

    for (std::list<Chunk>::iterator chunk = allocated.begin();
         chunk != allocated.end(); ++chunk)
    {

        totalAllocatedMem += chunk->chunkSize;
        totalAllocatedChunks++;
    }

    unlockForReadingAllocatedList();

    lockForReadingFreedList();
    for (std::list<Chunk>::iterator chunk = freed.begin(); chunk != freed.end(); ++chunk)
    {
        totalAllocatedMem += chunk->chunkSize;
        totalFreedChunks++;
    }
    cout << "**********Status Summary**********" << endl;
    cout << "Total Allocated Memory: " << totalAllocatedMem
         << "\nTotal Allocated Chunks: " << totalAllocatedChunks
         << "\nTotal Freed Chunks: " << totalFreedChunks;
    cout << "\nFragmentation: " << freed.size() << endl;
    unlockForReadingFreedList();
    return EXIT_SUCCESS;
}
