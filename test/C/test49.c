#include <stdlib.h>

#include"common.h"

static const char *input = "s";

static const char *description =
"\n"
"P  : 's' opt                # prog (1)\n"
"opt :                       # opt (-)\n"
"opt : 'c'                   # optc (-)"
  ;

main (int argc, char **argv)
{
  test_complex_parse (1, 0, 0, 0, argc, argv);
  exit (0);
}
