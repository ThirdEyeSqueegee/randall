#include <stdbool.h>

struct options
{
  bool input_flag;
  bool output_flag;
  char *input_arg;
  char *output_arg;
  long long nbytes;
};

void parser (int argc, char **argv, struct options *opt);
