#include<cstdio>
#include <stdlib.h>
#include "objstack.h"
#include "yaep.h"

static void *
test_parse_alloc (int size)
{
  return malloc (size);
}

static void
test_parse_free (void *mem)
{
  free (mem);
}

/* Printing syntax error. */
static void
test_syntax_error (int err_tok_num, void *err_tok_attr,
		   int start_ignored_tok_num, void *start_ignored_tok_attr,
		   int start_recovered_tok_num, void *start_recovered_tok_attr)
{
  if (start_ignored_tok_num < 0)
    fprintf (stderr, "Syntax error on token %d\n", err_tok_num);
  else
    fprintf
      (stderr,
       "Syntax error on token %d:ignore %d tokens starting with token = %d\n",
       err_tok_num, start_recovered_tok_num - start_ignored_tok_num,
       start_ignored_tok_num);
}

/* The following variable is the current number of next input
   token. */
static int ntok;

/* The following function imported by YAEP (see comments in the interface file). */
static int
test_read_token (void **attr)
{
  const char input [] ="a+((a*a)+a)";

  ntok++;
  *attr = NULL;
  if (ntok < sizeof (input))
    return input [ntok - 1];
  else
    return -1;
}

static const char *description =
"\n"
"E : V '+' V                 # add  1 (0 2)\n"
"  | V '*' V                 # mult 1 (0 2)\n"
"  | V '+' '(' V '*' V ')'   # madd 1 (0 3 5)\n"
"  | '(' V '*' V ')' '+' V   # madd 1 (6 1 3)\n"
"  ;\n"
"V : 'a'                     # 0\n"
"  | '(' E ')'               # 1\n"
"  ;\n"
  ;

int
main (int argc, char **argv)
{
  yaep *e;
  struct yaep_tree_node *root;
  int ambiguous_p;

  e = new yaep ();
  if (e == NULL)
    {
      fprintf (stderr, "yaep::yaep: No memory\n");
      exit (1);
    }
  ntok = 0;
  e->set_one_parse_flag (1);
  e->set_cost_flag (1);
  if (argc > 1)
    e->set_lookahead_level (atoi (argv [1]));
  if (argc > 2)
    e->set_debug_level (atoi (argv [2]));
  else
    e->set_debug_level (3);
  if (argc > 3)
    e->set_error_recovery_flag (atoi (argv [3]));
  if (argc > 4)
    e->set_one_parse_flag (atoi (argv [4]));
  if (e->parse_grammar (1, description) != 0)
    {
      fprintf (stderr, "%s\n", e->error_message ());
      exit (1);
    }
  if (e->parse (test_read_token, test_syntax_error, test_parse_alloc,
		test_parse_free, &root, &ambiguous_p))
    {
      fprintf (stderr, "yaep::parse: %s\n", e->error_message ());
      exit (1);
    }
  if (!ambiguous_p)
    {
      fprintf (stderr, "It should be ambigous grammar\n");
      exit (1);
    }
  fprintf (stderr, "cost = %d\n", root->val.anode.cost);
  delete e;
  exit (0);
}
