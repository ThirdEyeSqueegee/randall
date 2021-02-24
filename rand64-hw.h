#include <stdbool.h>

struct cpuid cpuid (unsigned int, unsigned int);

_Bool rdrand_supported (void);

void hardware_rand64_init (void);

unsigned long long hardware_rand64 (void);

void hardware_rand64_fini (void);
