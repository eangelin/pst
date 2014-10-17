#pragma once

#ifndef PST_SET_H__
#define PST_SET_H__

#include "tree.h"
#include <functional>
#include <initializer_list>

namespace pst
{
  namespace set
  {
    template <typename T, typename Compare = std::less<T>, typename Alloc = std::allocator<T> >
    struct rb_set
    {
      typedef T value_type;
      typedef pst::tree::rb_tree<value_type, Compare, Alloc> rb_tree_type;

      rb_set() {}
      rb_set(rb_tree_type t) : tree(std::move(t)) {}

      static rb_set empty_set() { return rb_set(); }

      template <typename U> static rb_set from(std::initializer_list<U> lst) {
        return from(begin(lst), end(lst));
      }

      template <typename It> static rb_set from(It begin, It end) {
        return empty_set().insert(std::move(begin), std::move(end));
      }

      bool empty() const { return tree.empty(); }
      bool member(value_type const& t) const { return !!tree.find(t); }
      rb_set insert(value_type t) const { return tree.insert(std::move(t)); }
      rb_set erase(value_type const& t) const { return tree.erase(t); }

      template <typename It>
      rb_set insert(It begin, It end) const
      {
        rb_set temp = *this;

        while (begin != end) {
          temp = temp.insert(*begin++);
        }

        return temp;
      }

      template <typename It>
      rb_set erase(It begin, It end) const
      {
        rb_set temp = *this;

        while (begin != end) {
          temp = temp.erase(*begin++);
        }

        return temp;
      }

    private:
      rb_tree_type tree;
    };
  }
}

#endif // PST_SET_H__
