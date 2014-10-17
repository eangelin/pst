#pragma once

#ifndef PST_UTIL_H__
#define PST_UTIL_H__

#include <functional>

namespace pst
{
  namespace detail
  {
    template <typename LessT>
    struct pair_first_less
    {
      template <typename FirstT, typename SecondT>
      bool operator()(std::pair<FirstT, SecondT> const& lhs,
                      std::pair<FirstT, SecondT> const& rhs) const
      {
        return LessT()(lhs.first, rhs.first);
      }
    };
  }
}

#endif // PST_UTIL_H__
