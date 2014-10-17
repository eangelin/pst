#include "maps.h"
#include <pst/map.h>
#include <cassert>
#include <string>

void test_rb_map()
{
  auto m = pst::map::rb_map<std::string, int>::empty_map();
  assert(m.empty());

  auto m2 = m.insert("three", 3);
  assert(m2.find("three"));
  assert(m2.find("three")->second == 3);

  auto m3 = m2.erase("three");
  assert(!m3.find("three"));

  assert(m3.empty());

  auto m4 = pst::map::rb_map<std::string, int>::from({
      {"one", 1},
      {"two", 2},
      {"three", 3}
    });

  assert(m4.find("one")->second == 1);
  assert(m4.find("two")->second == 2);
  assert(m4.find("three")->second == 3);

  auto vals = { "two", "three" };
  m4 = m4.erase(begin(vals), end(vals));

  assert(m4.find("one")->second == 1);
  assert(!m4.find("two"));
  assert(!m4.find("three"));

  m4 = m4.insert("one", 111);

  assert(m4.find("one")->second == 111);

  m4 = m4.erase("unknown");

  assert(m4.find("one")->second == 111);
}

