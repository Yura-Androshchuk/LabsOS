#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "buddy.h"

buddy_t* B = 0;
void bInit(buddy_t* buddy)
{
    B = buddy;
    //fills first bytes with a pointer with a 0 symbol
    memset(B, 0, sizeof(buddy_t));
    B->freelist[Order_Max_Size] = B->pool;
}

//TEsts block
void execute1() {
    void* pointer1, * pointer2;
    pointer1 = bMalloc(4); pointer2 = bMalloc(13);
    freeMemory(pointer2);
    freeMemory(pointer1);
    showResult();
}
void execute2() {
    void* pointer1, * pointer2, * pointer3;
    pointer1 = bMalloc(17);
    pointer2 = bMalloc(6);
    pointer3 = bMalloc(4);

    freeMemory(pointer3);
    freeMemory(pointer1);

    showResult();
}


//Main function
int main() {

    buddy_t* buddy = (buddy_t*)malloc(sizeof(buddy_t));
    bInit(buddy);
    execute1();
    printf("Dont free memory for 1 pointer test");
    execute2();
    makeZero();
    free(buddy);
    return 0;
}

pointer bMalloc(int size) {

    int i, order;
    pointer block, b;
    i = 0;
    while (BLOCKSIZE(i) < size + 1)
        i++;
    order = i = (i < Order_Min_Size) ? Order_Min_Size : i;

    for (;; i++) {                  // continue finding while we can't get not null
        if (i > Order_Max_Size)
            return NULL;
        if (B->freelist[i])
            break;
    }

    block = B->freelist[i];         //delete block from the list
    B->freelist[i] = *(pointer*)B->freelist[i];

    while (i-- > order) { //split
        b = _bOff(block, i);
        B->freelist[i] = b;
    }

    *(((uint8_t*)block - 1)) = order;
    return block;
}

void freeMemory(pointer block)
{
    int i; pointer b; pointer* p;

    i = *(((uint8_t*)block - 1)); // get order

    for (;; i++)
    {
        b = _bOff(block, i);  // define b
        p = &(B->freelist[i]);

        while ((*p != NULL) && (*p != b))// find bu in list
            p = (pointer*)*p;

        if (*p != b)  // not found, insert into list
        {
            *(pointer*)block = B->freelist[i];
            B->freelist[i] = block;
            return;
        }
        block = (block < b) ? block : b; // found, merged block starts from the lower one
        *p = *(pointer*)*p; //delete
    }
}
void makeZero()
{
    B = 0;
}

static int findCount(int i)
{

    int count = 0;
    pointer* p = &(B->freelist[i]);

    while (*p != NULL) {
        count++;
        p = (pointer*)*p;
    }
    return count;
}

static int total_free()
{
    int i, bytecount = 0;
    for (i = 0; i <= Order_Max_Size; i++)
    {
        bytecount += findCount(i) * BLOCKSIZE(i);
    }
    return bytecount;
}

static void print_list(int i)
{
    pointer* p = &B->freelist[i];
    while (*p != NULL)
    {
        printf("    0x%08lx, 0x%08lx\n", (uintptr_t)*p, (uintptr_t)*p - (uintptr_t)B->pool);
        p = (pointer*)*p;
    }
}

void showResult()
{
    printf("Current pool size:\n");
    printf("%d", POOLSIZE);
    printf(" bytes\n");

    printf("Start pool point:\n");
    printf("0x%08x\n", (unsigned int)(uintptr_t)B->pool);

    printf("Free memory: %d\n", total_free());
    for (int i = 0; i <= Order_Max_Size; i++) {
        print_list(i);
    }
    printf("\n");
}