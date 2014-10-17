#pragma once

#ifndef PST_TREE_IO_H__
#define PST_TREE_IO_H__

#include "tree.h"
#include <iostream>
#include <string>

namespace pst
{
  namespace tree
  {
    namespace detail
    {
      template <typename T, typename L, typename E>
      void dump(std::ostream& os, bs_tree<T, L, E> const& t, int indent)
      {
        std::string spc(static_cast<std::string::size_type>(indent), ' ');
        if (!empty(t)) {
          os << spc << keyval(t) << std::endl;
          dump(os, left(t), indent + 2);
          dump(os, right(t), indent + 2);
        }
        else {
          os << spc << "<null>" << std::endl;
        }
      }

      template <typename T, typename L, typename E>
      void dump(std::ostream& os, rb_tree<T, L, E> const& t, int indent)
      {
        std::string spc(static_cast<std::string::size_type>(indent), ' ');
        if (!empty(t)) {
          os << spc << std::string(color(t) == RED ? "red: " : "black: ") << keyval(t) << std::endl;
          dump(os, left(t), indent + 2);
          dump(os, right(t), indent + 2);
        }
        else {
          os << spc << "<null>" << std::endl;
        }
      }
    }

    template <typename T, typename L, typename E>
    void dump(std::ostream& os, bs_tree<T, L, E> const& t) {
      detail::dump(os, t, 0);
    }

    template <typename T, typename L, typename E>
    void dump(std::ostream& os, rb_tree<T, L, E> const& t) {
      detail::dump(os, t, 0);
    }
  }
}

#endif // PST_TREE_IO_H__
