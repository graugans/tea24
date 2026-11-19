#include <stdio.h>
#include "greet.h"

static int our_bss;

int main(void)
{
    putc(ZEICHEN, stdout);
    putc('\n', stdout);
    return 0;
}