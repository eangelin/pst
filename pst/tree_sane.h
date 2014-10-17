#pragma once

#ifndef PST_TREE_SANE_H__
#define PST_TREE_SANE_H__

#include "tree.h"

namespace pst
{
  namespace tree
  {
    namespace detail
    {
      template <typename T, typename L, typename E>
      bool do_check(rb_tree<T, L, E> const& t)
      {
        if (t.empty())
        {
          return true;
        }

        auto l = t.left(), r = t.right();

        if (t.color() == RED && (l.color() == RED || r.color() == RED))
        {
          return false;
        }

        if (!l.empty() && !rb_tree<T, L, E>::key_less(l.keyval(), t.keyval()))
        {
          return false;
        }

        if (!r.empty() && !rb_tree<T, L, E>::key_less(t.keyval(), r.keyval()))
        {
          return false;
        }

        if (l.bdepth() != r.bdepth())
        {
          return false;
        }

        return do_check(l) && do_check(r);
      }
    }

    template <typename T, typename L, typename E>
    bool check(bs_tree<T, L, E> const& t)
    {
      if (t.empty())
        return true;

      auto l = t.left(), r = t.right();

      if (!l.empty() && !bs_tree<T, L, E>::key_less(l.keyval(), t.keyval()))
      {
        return false;
      }

      if (!r.empty() && !bs_tree<T, L, E>::key_less(t.keyval(), r.keyval()))
      {
        return false;
      }

      return check(l) && check(r);
    }

    template <typename T, typename L, typename E>
    bool check(rb_tree<T, L, E> const& t)
    {
      return t.color() == BLACK && detail::do_check(t);
    }
  }
}

#endif // PST_TREE_SANE_H__
