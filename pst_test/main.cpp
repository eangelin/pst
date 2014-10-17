#include "trees.h"
#include "lists.h"
#include "sets.h"
#include "maps.h"
#include "rand.h"

int main(int /*argc*/, char* /*argv*/ [])
{
  init_rand();

  test_slist();
  test_bs_tree();
  test_rb_tree();
  test_rb_set();
  test_rb_map();
  iterate_bs_tree();
  iterate_rb_tree();

  return 0;
}
