/* Federico Poli Dicembre 2010 */

#include "basic.h"

int clamp(int val, int min, int max)
{
    if (val>max) return max;
    if (val<min) return min;
    return val;
}

// Stampa un errore con una sintassi tipo printf() ed esci
void error(const char* msg, ...)
{
    va_list args;
    va_start(args,msg);
    vfprintf(stderr,msg,args);
    va_end(args);
    exit(-1);
}
void error(char* msg, ...)
{
    va_list args;
    va_start(args,msg);
    vfprintf(stderr,msg,args);
    va_end(args);
    exit(-1);
}

