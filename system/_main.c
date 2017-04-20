/*  main.c  - main */

#include <xinu.h>
#include <stdio.h>

mutex_t lock = FALSE;

void    printchar(char c)
{
    mutex_lock(&lock);
    int i;
    for (i=0; i<10; i++)
        kprintf("%c", c);
    mutex_unlock(&lock);
}

int main(uint32 argc, uint32 *argv)
{
    //priority of process is input as the 3rd argument of create()
    ready(create((void*) printchar, INITSTK, 15, "P1", 1, 'A'), FALSE);
    ready(create((void*) printchar, INITSTK, 15, "P2", 1, 'B'), FALSE);
    // ready(create((void*) printchar, INITSTK, 10, "P3", 1, 'C'), FALSE);
    // ready(create((void*) printchar, INITSTK, 5, "P4", 1, 'D'), FALSE);

    return 0;
}
