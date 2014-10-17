#pragma once

#ifndef PST_MAP_H__
#define PST_MAP_H__

#include "tree.h"
#include "detail.h"
#include <functional>
#include <initializer_list>

namespace pst
{
  namespace map
  {
    template <typename KeyT, typename ValT, typename Compare = std::less<KeyT>, typename Alloc = std::allocator<ValT> >
    struct rb_map
    {
      typedef KeyT key_type;
      typedef ValT mapped_type;
      typedef std::pair<key_type, mapped_type> value_type;
      typedef pst::tree::rb_tree<value_type,
                                 detail::pair_first_less<Compare>,
                                 Alloc> rb_tree_type;

      rb_map() {}
      rb_map(rb_tree_type t) : tree(std::move(t)) {}

      static rb_map empty_map() { return rb_map(); }

      static rb_map from(std::initializer_list<value_type> lst) {
        return from(begin(lst), end(lst));
      }

      template <typename It>
      static rb_map from(It begin, It end) {
        return empty_map().insert(std::move(begin), std::move(end));
      }

      bool empty() const { return tree.empty(); }

      rb_map insert(key_type k, mapped_type v) const {
        return tree.insert(std::make_pair(std::move(k), std::move(v)));
      }

      template <typename It>
      rb_map insert(It begin, It end) const
      {
        rb_map temp = *this;

        while (begin != end) {
          temp = temp.insert(begin->first, begin->second);
          ++begin;
        }

        return temp;
      }

      template <typename It>
      rb_map erase(It begin, It end) const
      {
        rb_map temp = *this;

        while (begin != end) {
          temp = temp.erase(*begin++);
        }

        return temp;
      }

      value_type const* find(key_type const& key) const {
        return tree.find(std::make_pair(key, mapped_type()));
      }

      rb_map erase(key_type const& k) const {
        return tree.erase(std::make_pair(k, mapped_type()));
      }

    private:
      rb_tree_type tree;
    };
  }
}

#endif // PST_MAP_H__
