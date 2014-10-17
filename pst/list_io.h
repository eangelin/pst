#pragma once

#ifndef PST_LIST_IO_H__
#define PST_LIST_IO_H__

#include "list.h"

namespace pst
{
  namespace list
  {
    namespace detail
    {
      template <typename OStreamT, typename T>
      OStreamT& do_ostream_slist(OStreamT& os, slist<T> const& lst, bool first)
      {
        if (empty(lst))
          return os;

        if (!first)
          os << ", ";

        os << car(lst);

        return do_ostream_slist(os, cdr(lst), false);
      }
    }

    template <typename OStreamT, typename T>
    OStreamT& operator<<(OStreamT& os, slist<T> const& lst) {
      return detail::do_ostream_slist(os, lst, true);
    }
  }
}

#endif // PST_LIST_IO_H__
