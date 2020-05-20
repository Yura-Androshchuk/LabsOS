#include <stdint.h>
#define BUDDY_H_
#define Order_Max_Size       11
#define Order_Min_Size       4   
#define POOLSIZE        (1 << Order_Max_Size) 
#define BLOCKSIZE(i)    (1 << (i)) // block size = 2**i

#define _MEMBASE        ((uintptr_t)B->pool)   //block adress
#define _OFFSET(b)      ((uintptr_t)b - _MEMBASE)
#define bOff(b, i)  (_OFFSET(b) ^ (1 << (i)))
#define _bOff(b, i)   ((pointer)( bOff(b, i) + _MEMBASE))
#define Round_Memory(x)       ((x % 4) ? (x / 4 + 1) * 4 : x)
typedef void* pointer;
typedef struct buddy {
    pointer freelist[Order_Max_Size + 2];  // еще один слот для первого блока в пуле
    uint8_t pool[POOLSIZE];
} buddy_t;

//functions
pointer bMalloc(int size);
void freeMemory(pointer block);
void bInit();
void makeZero();
void showResult();

