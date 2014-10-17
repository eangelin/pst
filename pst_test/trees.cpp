#include "trees.h"
#include "rand.h"
#include "tracer.h"
#include <pst/tree.h>
#include <pst/tree_iterator.h>
#include <pst/tree_io.h>
#include <pst/tree_sane.h>
#include <set>
#include <map>
//#include <boost/pool/pool_alloc.hpp>
#include <allocators>
#include <cmath>
#include <vector>
#include <numeric>
#include <algorithm>
#include <iostream>

namespace
{
  template <typename IntTree>
  void tree_rand_test()
  {
    auto t = IntTree::empty_tree();
    std::set<int> ints;

    for (int i = 0; i < 1000; ++i)
    {
      int r = rand_int();

      auto prev = t;

      ints.insert(r);
      t = t.insert(r);

      assert(ints.size() == t.size());
      assert(check(t));
    }

    for (auto i = begin(ints); i != end(ints); ++i)
    {
      auto tv = t.find(*i); tv;
      assert(tv);
      assert(*tv == *i);
    }
  }

  template <typename IntTree>
  void tree_eq_insert_test()
  {
    auto t = IntTree::empty_tree();
    t = t.insert(1);
    t = t.insert(1);

    t.find(1);
  }

  template <typename IntTree>
  void tree_simple_erase_test()
  {
    auto t = IntTree::empty_tree();

    t = t.insert(3);
    t = t.erase(3);

    assert(t.empty());

    t = t.insert(3);
    t = t.insert(5);

    t = t.erase(3);

    assert(!t.find(3));
    assert(t.find(5));
  }

  template <typename IntTree>
  void tree_rand_erase_test()
  {
    auto t = IntTree::empty_tree();

    std::set<int> ints;

    for (int i = 0; i < 1000; ++i)
    {
      int r = rand_int();
      ints.insert(r);
      t = t.insert(r);
    }

    for (int i = 0; i < 2000; ++i)
    {
      auto prev = t;
      int r = rand_int();
      ints.erase(r);
      t = t.erase(r);

      assert(ints.size() == t.size());
      assert(check(t));
    }
  }

  template <typename IntTree>
  void tree_insert_perf_test(int n)
  {
    auto t = IntTree::empty_tree();

    for (int i = 0; i < n; ++i)
    {
      auto r = rand_int();
      typename IntTree::key_value_type v = r;
      t = t.insert(std::move(v));
    }
  }

  template <typename IntTree>
  void tree_insert_erase_perf_test(int n)
  {
    auto t = IntTree::empty_tree();

    for (int i = 0; i < n; ++i)
    {
      t = t.insert(rand_int());
    }

    for (int i = 0; i < n; ++i)
    {
      t = t.erase(rand_int());
    }
  }

  template <class T, class A = std::allocator<T> > struct mk_rb_tree_type {
    typedef pst::tree::rb_tree<T, std::less<T>, A> type;  
  };

  template <class T, class A = std::allocator<T> > struct mk_bs_tree_type {
    typedef pst::tree::bs_tree<T, std::less<T>, A> type;  
  };

  void test_rb_tree_depth()
  {
    auto t = pst::tree::rb_tree<int>::empty_tree();

    for (int i = 0; i < 2000; ++i)
    {
      t = t.insert(rand_int());
      auto d = t.depth();
      auto bd = t.bdepth();
      assert(d < 2 * bd);
      auto e = 1 + 2 * std::log2(t.size());
      assert(d <= e);
    }
  }

  template <typename IntTree>
  void iterate_tree()
  {
    using namespace pst::tree;

    auto t = IntTree::empty_tree();
    std::vector<int> nums;
    nums.resize(1000);
    std::iota(begin(nums), end(nums), 0);
    std::random_shuffle(begin(nums), end(nums));

    for (auto i : nums)
      t = t.insert(i);

    std::sort(begin(nums), end(nums));

    tree_iterator<IntTree> tbegin(t), tend;

    auto mr = std::mismatch(begin(nums), end(nums),
                            tbegin);

    assert(mr.first == end(nums));    
  }
}

void test_bs_tree()
{
  using namespace pst::tree;
  tree_rand_test<bs_tree<int>>();
  tree_eq_insert_test<bs_tree<int>>();
  tree_rand_erase_test<rb_tree<int>>();
  iterate_bs_tree();
}

void test_rb_tree()
{
  using namespace pst::tree;

  tree_rand_test<rb_tree<int>>();
  tree_eq_insert_test<bs_tree<int>>();
  tree_rand_erase_test<rb_tree<int>>();
  test_rb_tree_depth();
  iterate_rb_tree();
}

void trace_rb_tree()
{
  using namespace pst::tree;

  tree_rand_erase_test<rb_tree<tracer<int>>>();
  tree_simple_erase_test<rb_tree<tracer<int>>>();
  tree_rand_test<rb_tree<tracer<int>>>();

  tracer<int>::report(std::cout);
}

void time_bs_tree()
{
  tree_insert_perf_test<mk_bs_tree_type<tracer<int>, std::allocator<int> >::type>(1000000);
}

void time_rb_tree()
{
  tree_insert_perf_test<mk_rb_tree_type<tracer<int>, std::allocator<int> >::type>(1000000);
  //tree_insert_perf_test<mk_rb_tree_type<int, boost::fast_pool_allocator<int> >::type>(1000000);

  tree_insert_erase_perf_test<mk_rb_tree_type<int, std::allocator<int> >::type>(1000000);
  //tree_insert_erase_perf_test<mk_rb_tree_type<int, boost::fast_pool_allocator<int> >::type>(10000000);
}

void iterate_bs_tree()
{
  using namespace pst::tree;
  iterate_tree<bs_tree<int> >();
}

void iterate_rb_tree()
{
  using namespace pst::tree;
  iterate_tree<rb_tree<int> >();  
}

