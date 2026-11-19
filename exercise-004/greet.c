#include <stdio.h> // wird fuer putc benötigt
#include "greet.h" // vorwaertsdeklaration von gruss

void gruss(void)
{
    putc(ZEICHEN, stdout);
    putc('\n', stdout);
}