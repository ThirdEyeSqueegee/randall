#include <stdlib.h>
#include <time.h>

struct drand48_data *buffer;

void
mrand_rand64_init (void)
{
  buffer = malloc (sizeof (struct drand48_data));
  srand48_r (time (NULL), buffer);
}

unsigned long long
mrand_rand64 (void)
{
  long int p;
  mrand48_r (buffer, &p);
  long int q;
  mrand48_r (buffer, &q);
  unsigned long long int x = ((unsigned long long int)p << 32) | q;
  return x;
}

void
mrand_rand64_fini (void)
{
  free (buffer);
}
