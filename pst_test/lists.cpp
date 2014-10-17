#include "lists.h"
#include <pst/list.h>

void test_slist()
{
  auto sl = pst::list::cons<int, int>(1);
  auto e = pst::list::empty_slist<int>();
  auto lst = cons(1, cons(2, cons(3, e)));
  auto lst2 = append(lst, lst);
  auto lst3 = update(lst2, 2, 4711);
  auto lst4 = suffixes(lst2);
}
