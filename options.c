#include "options.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void
parser (int argc, char **argv, struct options *opt)
{
  int c;
  char *endptr;
  opt->input_arg = NULL;
  opt->output_arg = NULL;

  /* Parse command line arguments using getopt.  */
  while ((c = getopt (argc, argv, "i:o:h")) != -1)
    {
      size_t len = strlen (optarg) + 1;
      switch (c)
        {
        case 'i':
          opt->input_flag = true;
          opt->input_arg = (char *)malloc (len);
          memcpy (opt->input_arg, optarg, len);
          if (strcmp (optarg, "rdrand") && strcmp (optarg, "mrand48_r")
              && optarg[0] != '/')
            {
              fprintf (stderr, "Invalid option for -i: %s\n", optarg);
              exit (1);
            }
          break;
        case 'o':
          opt->output_flag = true;
          opt->output_arg = (char *)malloc (len);
          memcpy (opt->output_arg, optarg, len);
          if (strcmp (optarg, "stdio") && !atoi (optarg))
            {
              fprintf (stderr, "Invalid option for -o: %s\n", optarg);
              exit (1);
            }
          break;
        case 'h':
          fprintf (stdout,
                   "usage: ./randall [-i <option>] [-o <option] NBYTES\n");
          exit (0);
        case '?':
          fprintf (stderr, "Unknown option: -%c\n", optopt);
          exit (1);
        }
    }
  if (argv[optind] == NULL)
    {
      fprintf (stderr, "Please enter NBYTES\n");
      exit (1);
    }
  opt->nbytes = strtoll (argv[optind], &endptr, 10);
}
