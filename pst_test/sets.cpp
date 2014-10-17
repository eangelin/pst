#include "sets.h"
#include <pst/set.h>
#include <cassert>

void test_rb_set()
{
  auto s = pst::set::rb_set<int>::from({1, 3, 5, 7, 9 });

  assert(!s.empty());

  assert(s.member(5));
  assert(!s.member(4));
  assert(s.member(3));

  auto vals = { 2, 4, 8 };
  s = s.insert(begin(vals), end(vals));
  s = s.insert(6);

  assert(s.member(4));
  assert(s.member(6));

  s = s.erase(6);

  assert(!s.member(6));

  auto vals2 = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

  s = s.erase(begin(vals2), end(vals2));

  assert(!s.member(2));
  assert(s.empty());
}
