/* Generate N bytes of random output.  */

/* When generating output this program uses the x86-64 RDRAND
   instruction if available to generate random numbers, falling back
   on /dev/random and stdio otherwise.

   This program is not portable.  Compile it with gcc -mrdrnd for a
   x86-64 machine.

   Copyright 2015, 2017, 2020 Paul Eggert

   This program is free software: you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include "mrand_rand64.h"
#include "options.h"
#include "output.h"
#include "rand64-hw.h"
#include "rand64-sw.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Main program, which outputs N bytes of random data.  */
int
main (int argc, char **argv)
{
  /* Initialize options struct and parse command line args.  */
  struct options *opt = malloc (sizeof (struct options));
  parser (argc, argv, opt);

  /* Initialize the default random number generator.  */
  void (*initialize) (void) = hardware_rand64_init;
  unsigned long long (*rand64) (void) = hardware_rand64;
  void (*finalize) (void) = hardware_rand64_fini;

  /* Check command line arguments and throw errors/initialize modules.  */
  if (opt->input_flag && opt->input_arg != NULL)
    {
      if (!strcmp (opt->input_arg, "rdrand") && !rdrand_supported ())
        {
          fprintf (stderr, "rdrand not supported\n");
          return 1;
        }
      if (!strcmp (opt->input_arg, "mrand48_r"))
        {
          {
            initialize = mrand_rand64_init;
            rand64 = mrand_rand64;
            finalize = mrand_rand64_fini;
          }
        }
      if (opt->input_arg[0] == '/')
        {
          void (*init_sw) (char *path) = software_rand64_init;
          rand64 = software_rand64;
          finalize = software_rand64_fini;

          init_sw (opt->input_arg);
        }
    }

  initialize ();

  int wordsize = sizeof rand64 ();
  int output_errno = 0;

  /* Handle output.  */
  output (rand64, opt->output_arg, opt->nbytes, wordsize, output_errno);

  finalize ();

  if (opt->input_arg != NULL)
    free (opt->input_arg);
  if (opt->output_arg != NULL)
    free (opt->output_arg);
  free (opt);

  return 0;
}
