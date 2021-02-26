#include "output.h"
#include "mrand_rand64.h"
#include "rand64-hw.h"
#include "rand64-sw.h"
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

bool
writebytes (unsigned long long (*rand64) (void), char *output_arg, int nbytes)
{
  /* Write N bytes at a time using the write() syscall.  */
  if (output_arg != NULL && strcmp (output_arg, "stdio"))
    {
      int total_written = 0;
      int required_to_write = nbytes;
      int bufsize = atoi (output_arg);
      char *buffer = (char *)malloc (bufsize * sizeof (char));

      do
        {
          unsigned long long x = rand64 ();
          int index = 0;
          if (total_written + bufsize > required_to_write)
            bufsize = required_to_write - total_written;
          while (x > 0 && index < bufsize)
            {
              buffer[index++] = x;
              x >>= CHAR_BIT;
            }
          if (index == bufsize)
            {
              total_written += write (STDOUT_FILENO, (void *)buffer, bufsize);
            }
        }
      while (total_written < required_to_write);

      free (buffer);

      return true;
    }
  /* Write using putchar.  */
  else
    {
      unsigned long long x = rand64 ();
      do
        {
          if (putchar (x) < 0)
            return false;
          x >>= CHAR_BIT;
          nbytes--;
        }
      while (0 < nbytes);

      return true;
    }
  return false;
}

void
output (unsigned long long (*rand64) (void), char *output_arg,
        long long nbytes, int wordsize, int output_errno)
{
  do
    {
      int outbytes = nbytes < wordsize ? nbytes : wordsize;
      if (!writebytes (rand64, output_arg, outbytes))
        {
          output_errno = errno;
          break;
        }
      nbytes -= outbytes;
    }
  while (0 < nbytes);

  if (fclose (stdout) != 0)
    output_errno = errno;

  if (output_errno)
    {
      errno = output_errno;
      perror ("output");
    }
}
