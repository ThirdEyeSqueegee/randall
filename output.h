#include <stdbool.h>

bool writebytes (unsigned long long x, int nbytes);

void output (unsigned long long (*rand64) (void), char *output_arg,
             long long nbytes, int wordsize, int output_errno);
