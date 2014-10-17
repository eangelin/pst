#pragma once

#ifndef PST_LIST_H__
#define PST_LIST_H__

#include <memory>

namespace pst
{
  namespace list
  {
    template <typename T, typename Alloc = std::allocator<T> >
    struct slist
    {
      typedef T value_type;
      typedef Alloc allocator_type;

      slist(slist&& other) : impl(std::move(other.impl)) {}
      slist(slist const& other) : impl(other.impl) {}

      slist(T val, slist const& tail)
        : impl(std::allocate_shared<impl_t>(impl_allocator(), std::move(val), std::move(tail)))
      {}

      slist& operator=(slist other) {
        swap(other);
        return *this;
      }

      void swap(slist& other) {
        using std::swap;
        swap(impl, other.impl);            
      }

      friend void swap(slist& lhs, slist& rhs) {
        lhs.swap(rhs);
      }

      friend T const& car(slist const& lst) { return lst.impl->val; }
      friend slist cdr(slist const& lst) { return lst.impl->tail; }
      friend bool empty(slist const& lst) { return !lst.impl; }

      static slist empty_slist() { return slist(); }

    private:
      slist() {}

      struct impl_t
      {
        impl_t(T val, slist const tail)
          : val(std::move(val)),
            tail(std::move(tail))
        {}

        T const val;
        slist const tail;

      private:
        impl_t(impl_t const&);
        impl_t& operator=(impl_t const&);
      };

      typedef typename allocator_type::template rebind<impl_t>::other impl_allocator;

      std::shared_ptr<impl_t const> impl;
    };

    template <typename T> slist<T> empty_slist() { return slist<T>::empty_slist(); }

    template <typename T, typename U>
    slist<U> cons(T&& value, slist<U> tail = empty_slist<U>()) {
      return slist<U>(std::forward<T>(value), std::move(tail));
    }

    template <typename T> slist<T> append(slist<T> const& llst, slist<T> const& rlst) {
      return empty(llst) ? rlst : cons(car(llst), append(cdr(llst), rlst));
    }

    template <typename T>
    slist<T> update(slist<T> const& lst, std::size_t index, T&& new_value) {
      return index == 0 ?
        cons(std::forward<T>(new_value), cdr(lst)) :
        cons(car(lst), update(cdr(lst), index - 1, std::forward<T>(new_value)));
    }

    template <typename T>
    slist<slist<T> > suffixes(slist<T> const& lst) {
      return empty(lst) ? cons(lst, empty_slist<slist<T>>()) : cons(lst, suffixes(cdr(lst)));
    }
  }
}

#endif // PST_LIST_H__

