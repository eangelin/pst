#pragma once

#ifndef PST_TREE_H__
#define PST_TREE_H__

#include <memory> // allocators
#include <functional> // less, equal_to
#include <algorithm> // swap
#include <cassert> // assert...
#include <cstddef> // size_t

namespace pst
{
  namespace tree
  {
    template <typename Derived,
              typename KeyValT,
              typename Compare = std::less<KeyValT>,
              typename ImplData = std::tuple<>,
              typename Alloc = std::allocator<KeyValT> >
    struct bst_base
    {
      typedef Derived tree_type;
      typedef KeyValT key_value_type;
      typedef Compare key_compare;
      typedef Alloc allocator_type;
      typedef ImplData impl_data_type;

      friend bool operator==(tree_type const& lhs, tree_type const& rhs) {
        return lhs.impl == rhs.impl;
      }

      friend bool operator!=(tree_type const& lhs, tree_type const& rhs) {
        return !(lhs == rhs);
      }

      static tree_type empty_tree() { return tree_type(); }

      bool empty() const
      {
        return !impl;
      }

      key_value_type const& keyval() const { return impl->val; }
      key_value_type keyval_copy() const { return impl->val; }

      std::size_t size() const {
        return empty() ? 0 : (1 + left().size() + right().size());
      }

      std::size_t depth() const
      {
        return empty() ? 0 : (1 + std::max(left().depth(), right().depth()));
      }

      key_value_type const* find(typename key_value_type const& kv) const
      {
        if (empty()) {
          return 0;
        }
        else if (key_less(kv, keyval())) {
          return left().find(kv);
        }
        else if (key_less(keyval(), kv)) {
          return right().find(kv);
        }
        else {
          return &keyval();
        }
      }

      key_value_type const* find_min() const
      {
        if (empty()) {
          return 0;
        }
        else if (left().empty()) {
          return &keyval();
        }
        else {
          return left().find_min();
        }
      }

      key_value_type const* find_max() const
      {
        if (empty()) {
          return 0;
        }
        else if (right().empty()) {
          return &keyval();
        }
        else {
          return right().find_max();
        }
      }

      tree_type const& left() const { return impl->left; }
      tree_type const& right() const { return impl->right; }

      static bool key_less(key_value_type const& lhs, key_value_type const& rhs) { return key_compare()(lhs, rhs); }

    protected:
      bst_base() {}
      bst_base(bst_base&& other) : impl(std::move(other.impl)) {}
      bst_base(bst_base const& other) : impl(other.impl) {}

      bst_base& operator=(bst_base&& other) {
        impl = std::move(other.impl);
        return *this;
      }

      bst_base& operator=(bst_base const& other) {
        impl = other.impl;
        return *this;
      }

      struct impl_t
      {
        impl_t(key_value_type&& keyval, tree_type&& left, tree_type&& right, impl_data_type impl_data = impl_data_type())
          : val(std::move(keyval)),
            left(std::move(left)),
            right(std::move(right)),
            impl_data(std::move(impl_data))
        {}

        impl_t(key_value_type&& keyval)
          : val(std::move(keyval))
        {}

        impl_t(key_value_type&& keyval, impl_data_type impl_data)
          : val(std::move(keyval)),
            impl_data(std::move(impl_data))
        {}

        key_value_type const val;
        tree_type const left, right;
        impl_data_type const impl_data;

      private:
        impl_t(impl_t const&);
        impl_t& operator=(impl_t const&);
      };

      typedef typename allocator_type::template rebind<impl_t>::other impl_allocator;

      bst_base(key_value_type&& keyval)
        : impl(std::allocate_shared<impl_t>(impl_allocator(), std::move(keyval)))
      {}

      bst_base(key_value_type&& keyval, impl_data_type impl_data)
        : impl(std::allocate_shared<impl_t>(impl_allocator(), std::move(keyval), std::move(impl_data)))
      {}

      bst_base(key_value_type&& keyval, tree_type&& left, tree_type&& right, impl_data_type impl_data = impl_data_type())
        : impl(std::allocate_shared<impl_t>(impl_allocator(), std::move(keyval), std::move(left), std::move(right), std::move(impl_data)))
      {}

      ~bst_base() {}

      impl_data_type const& impl_data() const { return impl->impl_data; }

      static tree_type with_left(tree_type const& orig, tree_type&& left) {
        return tree_type(orig.keyval_copy(), std::move(left), tree_type(orig.right()), orig.impl_data());
      }

      static tree_type with_right(tree_type const& orig, tree_type&& right) {
        return tree_type(orig.keyval_copy(), tree_type(orig.left()), std::move(right), orig.impl_data());
      }

      static tree_type with_left_right(tree_type const& orig, tree_type&& left, tree_type&& right) {
        return tree_type(orig.keyval_copy(), std::move(left), std::move(right), orig.impl_data());
      }

      static tree_type with_impl(tree_type const& orig, impl_data_type impl_data) {
        return tree_type(orig.keyval_copy(), tree_type(orig.left()), tree_type(orig.right()), std::move(impl_data));
      }

      static tree_type with_left_right_impl_data(tree_type const& orig, tree_type&& left, tree_type&& right, impl_data_type impl_data) {
        return tree_type(orig.keyval_copy(), std::move(left), std::move(right), std::move(impl_data));
      }

      static tree_type with_keyval(tree_type const& orig, key_value_type&& keyval) {
        return tree_type(std::move(keyval), tree_type(orig.left()), tree_type(orig.right()), orig.impl_data());
      }

      static tree_type with_left_right_keyval(tree_type const& orig, tree_type&& left, tree_type&& right, key_value_type&& keyval) {
        return tree_type(std::move(keyval), std::move(left), std::move(right), orig.impl_data());
      }

      std::shared_ptr<impl_t const> impl;
    };

    template <typename T,
              typename Compare = std::less<T>,
              typename Alloc = std::allocator<T> >
    struct bs_tree : public bst_base<bs_tree<T, Compare, Alloc>, T, Compare, std::tuple<>, Alloc>
    {
      bs_tree() {}
      bs_tree(bs_tree&& other) : bst_base(std::move(other)) {}
      bs_tree(bs_tree const& other) : bst_base(other) {}

      bs_tree insert(key_value_type&& v) const {
        return insert_impl(std::move(v));
      }

      bs_tree insert(key_value_type const& v) const {
        return insert_impl(std::move(key_value_type(v)));
      }

      bs_tree erase(key_value_type const& v) const
      {
        if (empty()) {
          return *this;
        }
        else if (key_less(v, keyval())) {
          return bs_tree(keyval(), left().erase(v), right());
        }
        else if (key_less(keyval(), v)) {
          return bs_tree(keyval(), left(), right().erase(v));
        }
        else {
          if (left().empty()) {
            return right();
          }
          else if (right().empty()) {
            return left();
          }
          else {
            auto pred = left().find_max();
            return bs_tree(*pred, left().erase(*pred), right());
          }
        }
      }

    protected:
      bs_tree insert_impl(key_value_type&& v) const
      {
        if (empty()) {
          return bs_tree(std::move(v));
        }
        else if (key_less(v, keyval())) {
          return bs_tree(keyval_copy(), left().insert(std::move(v)), bs_tree(right()));
        }
        else if (key_less(keyval(), v)) {
          return bs_tree(keyval_copy(), bs_tree(left()), right().insert(std::move(v)));
        }
        else {
          return bs_tree(std::move(v), bs_tree(left()), bs_tree(right()));
        }
      }

    protected:
      bs_tree(key_value_type&& keyval)
        : bst_base(std::move(keyval))
      {}

      bs_tree(key_value_type&& keyval, bs_tree&& l, bs_tree&& r)
        : bst_base(std::move(keyval), std::move(l), std::move(r))
      {}
    };

    /**************************************************************/

    enum rb_color_t { BLACK, RED };

    template <typename T,
              typename LessT = std::less<T>,
              typename Alloc = std::allocator<T> >
    struct rb_tree : public bst_base<rb_tree<T, LessT, Alloc>, T, LessT, rb_color_t, Alloc>
    {
      friend bst_base;

      rb_tree() {}
      rb_tree(rb_tree&& other) : bst_base(std::move(other)) {}
      rb_tree(rb_tree const& other) : bst_base(other) {}

      rb_color_t color() const { return impl ? impl->impl_data : BLACK; }

      std::size_t bdepth() const
      {
        return empty() ? 1 : (left().bdepth() + (color() == BLACK ? 1 : 0));
      }

      rb_tree insert(key_value_type&& v) const
      {
        auto with_insert = insert_impl<lhs_ops>(std::move(v), BLACK);
        return with_insert.second.color() == BLACK ? with_insert.second : with_color(with_insert.second, BLACK);
      }

      rb_tree insert(key_value_type const& v) const
      {
        auto with_insert = insert_impl<lhs_ops>(std::move(key_value_type(v)), BLACK);
        return with_insert.second.color() == BLACK ? with_insert.second : with_color(with_insert.second, BLACK);
      }

      rb_tree erase(key_value_type const& v) const
      {
        auto with_erase = erase_impl(v);
        return with_erase.second.color() == BLACK ? with_erase.second : with_color(with_erase.second, BLACK);
      }

    private:
      enum rb_shape_t { BBB, BBR, BRB, BRR, RBB, RBR, RRB, RRR }; /* RRR can't happen as we change one subtree at a time */

      static rb_shape_t rb_mk_shape(rb_color_t parent, rb_color_t left, rb_color_t right) {
        return static_cast<rb_shape_t>(4 * parent + 2 * left + right);
      }

      struct rhs_ops
      {
        static rb_shape_t shape_RRB() { return RRB; }
        static rb_shape_t shape_RBR() { return RBR; }

        static rb_shape_t rb_mk_shape(rb_color_t parent, rb_color_t left, rb_color_t right) {
          return rb_tree::rb_mk_shape(parent, left, right);
        }

        static bool key_less(typename rb_tree::key_value_type const& lhs, typename rb_tree::key_value_type const& rhs) {
          return rb_tree::key_less(lhs, rhs);
        }
        
        static rb_tree with_left(rb_tree const& orig, rb_tree left) {
          return rb_tree::with_left(orig, std::move(left));
        }

        static rb_tree with_right(rb_tree const& orig, rb_tree right) {
          return rb_tree::with_right(orig, std::move(right));
        }

        static rb_tree with_left_right(rb_tree const& orig, rb_tree left, rb_tree right) {
          return rb_tree::with_left_right(orig, std::move(left), std::move(right));
        }

        static rb_tree const& left(rb_tree const& t) {
          return t.left();
        }

        static rb_tree const& right(rb_tree const& t) {
          return t.right();
        }

        static rb_tree with_left_right_color(rb_tree const& t, rb_tree left, rb_tree right, rb_color_t color) {
          return rb_tree::with_left_right_color(t, std::move(left), std::move(right), color);
        }
      };

      struct lhs_ops
      {
        static rb_shape_t shape_RRB() { return RBR; }
        static rb_shape_t shape_RBR() { return RRB; }

        static rb_shape_t rb_mk_shape(rb_color_t parent, rb_color_t left, rb_color_t right) {
          return rb_tree::rb_mk_shape(parent, right, left);
        }

        static bool key_less(typename rb_tree::key_value_type const& lhs, typename rb_tree::key_value_type const& rhs) {
          return rb_tree::key_less(rhs, lhs);
        }

        static rb_tree with_left(rb_tree const& orig, rb_tree left) {
          return rb_tree::with_right(orig, std::move(left));
        }

        static rb_tree with_right(rb_tree const& orig, rb_tree right) {
          return rb_tree::with_left(orig, std::move(right));
        }

        static rb_tree with_left_right(rb_tree const& orig, rb_tree left, rb_tree right) {
          return rb_tree::with_left_right(orig, std::move(right), std::move(left));
        }

        static rb_tree const& left(rb_tree const& t) {
          return t.right();
        }

        static rb_tree const& right(rb_tree const& t) {
          return t.left();
        }

        static rb_tree with_left_right_color(rb_tree const& t, rb_tree left, rb_tree right, rb_color_t color) {
          return rb_tree::with_left_right_color(t, std::move(right), std::move(left), color);
        }
      };

      template <typename some_hs> struct other_hs;
      template <> struct other_hs<rhs_ops> { typedef lhs_ops type; };
      template <> struct other_hs<lhs_ops> { typedef rhs_ops type; };

      typedef std::pair<int, rb_tree> deltatree_t;

      deltatree_t erase_impl(key_value_type const& v) const
      {
        if (empty()) {
          return std::make_pair(0, *this);
        }
        else if (color() == BLACK) {
          return black_erase_impl(v);
        }
        else {
          return red_erase_impl(v);
        }
      }

      template <typename Ops>
      static deltatree_t black_fixup_right_impl(rb_tree const& parent/*_with_new_right*/)
      {
        assert(parent.color() == BLACK);
        assert(Ops::right(parent).color() == BLACK);

        rb_tree const& left = Ops::left(parent);

        if (left.color() == RED)
        {
          rb_tree const& left_r = Ops::right(left);
          rb_tree const& left_r_r = Ops::right(left_r);

          if (left_r_r.color() == BLACK)
          {
            // (P{B} (L{R} LL{B} (LR{B} LRL{?} LRR{B})) R{B}) -> (L{B} LL{B} (LR{B} LRL{?} (P{R} LRR{B} R{B})))
            return std::make_pair(0,
                                  Ops::with_left_right_color(left,
                                                             Ops::left(left),
                                                             Ops::with_right(left_r,
                                                                             Ops::with_left_right_color(parent,
                                                                                                        left_r_r,
                                                                                                        Ops::right(parent),
                                                                                                        RED)),
                                                             BLACK));
          }
          else
          {
            // (P{B} (L{R} LL{B} (LR{B} LRL{?} LRR{R})) R{B}) -> (L{B} LL{B} (LRR{B} (LR{B} LRL{?} LRRL{B}) (P{B} (LRRR{B}, R{B}))))
            assert(left_r_r.color() == RED);
            return std::make_pair(0,
                                  Ops::with_left_right_color(left,
                                                             Ops::left(left),
                                                             Ops::with_left_right(left_r_r,
                                                                                  Ops::with_right(left_r,
                                                                                                  Ops::left(left_r_r)),
                                                                                  Ops::with_left(parent,
                                                                                                 Ops::right(left_r_r))),
                                                             BLACK));
          }
        }
        else
        {
          assert(left.color() == BLACK);

          rb_tree const& left_l = Ops::left(left);
          rb_tree const& left_r = Ops::right(left);

          if (left_l.color() == RED)
          {
            // (P{B} (L{B} LL{R} LR{?}) R{B}) -> (L{B} LL{B} (P{B} LR{?} R{B}))
            return std::make_pair(0,
                                  Ops::with_left_right(left,
                                                       with_color(left_l, BLACK),
                                                       Ops::with_left(parent,
                                                                      left_r)));
          }
          else if (left_r.color() == RED)
          {
            // (P{B} (L{B} LL{B} LR{R}) R{B}) -> (LR{B} (L{B} LL{B} LRL{B})) (P{B} LRR{B} R{B})
            assert(left_l.color() == BLACK);
            return std::make_pair(0,
                                  Ops::with_left_right_color(left_r,
                                                             Ops::with_right(left,
                                                                             Ops::left(left_r)),
                                                             Ops::with_left(parent,
                                                                            Ops::right(left_r)),
                                                             BLACK));
          }
          else
          {
            // (P{B} (L{B} LL{B} LR{B}) R{B}) -> (P{B} (L{R} LL{B} LR{B}) R{B})
            // still -1
            assert(left_l.color() == BLACK);
            assert(left_r.color() == BLACK);
            return std::make_pair(-1,
                                  Ops::with_left(parent,
                                                 with_color(left, RED)));
          }
        }
      }

      static deltatree_t black_fixup_left(rb_tree const& new_parent) {
        return black_fixup_right_impl<lhs_ops>(new_parent);
      }

      template <typename Ops>
      deltatree_t black_erase_right_impl(key_value_type const& v) const
      {
        auto new_right = Ops::right(*this).erase_impl(v);

        if (new_right.first == 0)
        {
          return std::make_pair(0, Ops::with_right(*this,
                                                   std::move(new_right.second)));
        }
        else
        {
          return black_fixup_right_impl<Ops>(Ops::with_right(*this,
                                                             std::move(new_right.second)));
        }
      }

      deltatree_t black_erase_impl(key_value_type const& v) const
      {
        assert(color() == BLACK);

        if (key_less(v, keyval()))
        {
          return black_erase_right_impl<lhs_ops>(v);
        }
        else if (key_less(keyval(), v))
        {
          return black_erase_right_impl<rhs_ops>(v);
        }
        else
        {
          if (left().empty())
          {
            if (right().empty())
            {
              return std::make_pair(-1, empty_tree());
            }
            else
            {
              // Since empty(left), right must be a red leaf. No change in black depth.
              return std::make_pair(0, with_color(right(), BLACK));
            }
          }
          else if (right().empty())
          {
            assert(!left().empty());
            // Since empty(right), left must be a red leaf. No change in black depth.
            return std::make_pair(0, with_color(left(), BLACK));
          }
          else
          {
            assert(!left().empty() && !right().empty());
            auto predecessor_kv = left().find_max();
            auto new_left = left().erase_impl(*predecessor_kv);
            auto relabeled_t = with_left_right_keyval(*this, std::move(new_left.second), rb_tree(right()), std::move(key_value_type(*predecessor_kv)));
            if (new_left.first == 0)
            {
              return std::make_pair(0, std::move(relabeled_t));
            }
            else
            {
              return black_fixup_left(relabeled_t);
            }
          }
        }
      }

      template <typename Ops>
      static deltatree_t red_fixup_right_impl(rb_tree const& parent/*_with_new_right*/)
      {
        assert(parent.color() == RED);
        assert(Ops::left(parent).color() == BLACK);
        assert(Ops::right(parent).color() == BLACK);

        rb_tree const& left = Ops::left(parent);
        rb_tree const& left_l = Ops::left(left);
        rb_tree const& left_r = Ops::right(left);

        if (left_l.color() == RED)
        {
          // (P{R} (L{B} LL{R} LR{?}) R{B}) -> (L{R} LL{B} (P{B} LR{?} R{B}))
          return std::make_pair(0,
                                Ops::with_left_right_color(left,
                                                           with_color(left_l, BLACK),
                                                           Ops::with_left_right_color(parent,
                                                                                      left_r,
                                                                                      Ops::right(parent),
                                                                                      BLACK),
                                                           RED));
        }
        else if (left_r.color() == RED)
        {
          // (P{R} (L{B} LL{?} LR{R}) R{B}) -> (LR{R} (L{B} LL{?} LRL{B}) (P{B} LRR{B} R{B}))
          return std::make_pair(0,
                                Ops::with_left_right(left_r,
                                                     Ops::with_right(left,
                                                                     Ops::left(left_r)),
                                                     Ops::with_left_right_color(parent,
                                                                                Ops::right(left_r),
                                                                                Ops::right(parent),
                                                                                BLACK)));
        }
        else
        {
          // (P{R} (L{B} LL{B} LR{B}) R{B}) -> (P{B} L{R} R{B})
          return std::make_pair(0,
                                Ops::with_left_right_color(parent,
                                                           with_color(left, RED),
                                                           Ops::right(parent),
                                                           BLACK));
        }
      }

      static deltatree_t red_fixup_left(rb_tree const& new_parent) {
        return red_fixup_right_impl<lhs_ops>(new_parent);
      }

      template <typename Ops>
      deltatree_t red_erase_right_impl(key_value_type const& v) const
      {
        auto new_right = Ops::right(*this).erase_impl(v);

        if (new_right.first == 0)
        {
          return std::make_pair(0, Ops::with_right(*this,
                                                   std::move(new_right.second)));
        }
        else
        {
          return red_fixup_right_impl<Ops>(Ops::with_right(*this,
                                                           std::move(new_right.second)));
        }
      }

      deltatree_t red_erase_impl(key_value_type const& v) const
      {
        if (key_less(v, keyval()))
        {
          return red_erase_right_impl<lhs_ops>(v);
        }
        else if (key_less(keyval(), v))
        {
          return red_erase_right_impl<rhs_ops>(v);
        }
        else
        {
          // deleting t
          if (left().empty())
          {
            assert(right().empty());
            return std::make_pair(0, empty_tree());
          }
          else
          {
            assert(!right().empty());
            auto predecessor_kv = left().find_max();
            auto new_left = left().erase_impl(*predecessor_kv);
            auto relabeled_t = with_left_right_keyval(*this, std::move(new_left.second), rb_tree(right()), std::move(key_value_type(*predecessor_kv)));
            if (new_left.first == 0)
            {
              return std::make_pair(0, std::move(relabeled_t));
            }
            else
            {
              return red_fixup_left(relabeled_t);
            }
          }
        }
      }

      typedef std::pair<rb_shape_t, rb_tree> shapetree_t;

      static rb_color_t rb_shape_parent(rb_shape_t s)
      {
        return static_cast<rb_color_t>(!!(s & 4));
      }

      static rb_color_t rb_shape_left(rb_shape_t s)
      {
        return static_cast<rb_color_t(!!(s & 2));
      }

      static rb_color_t rb_shape_right(rb_shape_t s)
      {
        return static_cast<rb_color_t(!!(s & 1));
      }

      static bool rb_check_shape(rb_shape_t s, rb_tree const& t)
      {
        if (t.empty())
          return s == BBB;

        return rb_shape_parent(s) == color() && rb_shape_left(s) == left().color() && rb_shape_right(s) == rigtht().color();
      }

      static shapetree_t shape_checked(shapetree_t st)
      {
        assert(rb_check_shape(st.first, st.second));
        return st;
      }

      template <typename Ops>
      shapetree_t insert_impl(key_value_type&& v, rb_color_t sibling_color) const
      {
        if (empty())
        {
          return std::make_pair(RBB,
                                rb_tree(std::move(v), RED));
        }

        // paranoid? use shape_checked(..._insert_impl(...)) below.

        if (color() == BLACK)
        {
          return black_insert_impl(std::move(v));
        }
        else
        {
          return red_insert_impl<Ops>(std::move(v), sibling_color);
        }
      }

      // special case for red parent nodes
      shapetree_t black_or_empty_insert_impl(key_value_type&& v) const
      {
        return empty() ?
          std::make_pair(RBB, rb_tree(std::move(v), RED)) :
          black_insert_impl(std::move(v));
      }

      template <typename Ops>
      shapetree_t black_insert_right_impl(key_value_type&& v) const
      {
        auto left_color = Ops::left(*this).color();
        auto new_right = Ops::right(*this).insert_impl<Ops>(std::move(v), left_color);

        if (new_right.first == Ops::shape_RRB())
        {
          // child will rotate RRB to RBR if sibling_color (left_color here) is black
          assert(left_color == RED);
          // (T{B} L{R} R{?})
          // (T{B} L{R} (R*{R} RL*{R} RR*{B})) -> (T{R} L{B} R*{B})
          return std::make_pair(RBB,
                                Ops::with_left_right_color(*this,
                                                           with_color(Ops::left(*this), BLACK),
                                                           // FIXME: move new_right?
                                                           with_color(new_right.second, BLACK),
                                                           RED));
        }
        else if (new_right.first == Ops::shape_RBR())
        {
          // (T{B} L{?} R{?}
          if (left_color == BLACK)
          {
            // (T{B} L{B} (R*{R} RL*{B} RR*{R})) -> (R*{B} (T{R} L RL*) RR*)
            return std::make_pair(BRR,
                                  // FIXME: move new_right
                                  Ops::with_left_right_color(new_right.second,
                                                             Ops::with_left_right_color(*this,
                                                                                        Ops::left(*this),
                                                                                        Ops::left(new_right.second),
                                                                              RED),
                                                             Ops::right(new_right.second),
                                                             BLACK));
          }
          else
          {
            // (T{B} L{R} (R*{R} RL*{B} RR*{R})) -> (T{R} L{B} R*{B})
            return std::make_pair(RBB,
                                  Ops::with_left_right_color(*this,
                                                             with_color(Ops::left(*this), BLACK),
                                                             with_color(new_right.second, BLACK),
                                                             RED));
          }
        }
        else
        {
          // (T{B} L{?} R{?})
          // (T L R*) -> (T L R*)
          return std::make_pair(Ops::rb_mk_shape(BLACK, left_color, rb_shape_parent(new_right.first)),
                                Ops::with_right(*this, std::move(new_right.second)));
        }
      }

      shapetree_t black_insert_impl(key_value_type&& v) const
      {
        if (key_less(v, keyval()))
        {
          return black_insert_right_impl<lhs_ops>(std::move(v));
        }
        else if (key_less(keyval(), v))
        {
          return black_insert_right_impl<rhs_ops>(std::move(v));
        }
        else
        {
          // (T{B} L{?} R{?}) -> (T{val = v} L R)
          return std::make_pair(rb_mk_shape(BLACK, left().color(), right().color()),
                                with_keyval(*this, std::move(v)));
        }
      }

      template <typename Ops>
      shapetree_t red_insert_impl(key_value_type&& v, rb_color_t sibling_color) const
      {
        // (T{R} L{B} R{B})
        assert(color() == RED);

        if (Ops::key_less(v, keyval()))
        {
          // As t is red, t.left() is black ...
          auto new_left = Ops::left(*this).black_or_empty_insert_impl(std::move(v));

          // no change causes three consecutive RED nodes, only have to consider these cases
          assert(rb_shape_parent(new_left.first) == BLACK || new_left.first == RBB);

          if (new_left.first == RBB && sibling_color == BLACK)
          {
              // (T{R} (L*{R} LL*{B} LR*{B}) R{B}) -> (L*{R} LL*{B} (T{R} LR*{B} R{B}))
            // FIXME: new_left.second moved? steal keyval...
            return std::make_pair(Ops::shape_RBR(),
                                  Ops::with_right(new_left.second,
                                                  Ops::with_left(*this, Ops::right(new_left.second))));
          }
          else
          {
            // (T{R} L*{R} R{B}) -> (T{R} L*{R} R{B}) - new_left.first == RBB && sibling_color == RED
            // or
            // (T{R} L*{B} R{B}) -> (T{R} L*{B} R{B}) - rb_shape_parent(new_left.first) == BLACK
            return std::make_pair(Ops::rb_mk_shape(RED, rb_shape_parent(new_left.first), BLACK),
                                  Ops::with_left(*this, std::move(new_left.second)));
          }
        }
        else if (Ops::key_less(keyval(), v))
        {
          // As t is red, t.right() is black ...
          auto new_right = Ops::right(*this).black_or_empty_insert_impl(std::move(v));

          // no change causes three consecutive RED nodes, only have to consider these cases
          assert(rb_shape_parent(new_right.first) == BLACK || new_right.first == RBB);

          // (T{R} L{B} R*{R}) -> (T{R} L{B} R*{R})
          // or
          // (T{R} L{B} R*{B}) -> (T{R} L{B} R*{B})
          return std::make_pair(Ops::rb_mk_shape(RED, BLACK, rb_shape_parent(new_right.first)),
                                Ops::with_right(*this, std::move(new_right.second)));
        }
        else
        {
          // (T{R} L{B} R{B}) -> (T{R, val = v}, L{B} R{B})
          return std::make_pair(RBB,
                                with_keyval(*this, std::move(v)));
        }
      }

    protected:
      static rb_tree with_color(rb_tree const& t, rb_color_t color) {
        return with_impl(t, color);
      }

      static rb_tree with_left_right_color(rb_tree const& t, rb_tree left, rb_tree right, rb_color_t color) {
        return with_left_right_impl_data(t, std::move(left), std::move(right), color);
      }

      rb_tree(key_value_type&& keyval, rb_color_t clr)
        : bst_base(std::move(keyval), clr)
      {}
      rb_tree(key_value_type&& keyval, rb_tree&& l, rb_tree&& r, rb_color_t clr = rb_color_t())
        : bst_base(std::move(keyval), std::move(l), std::move(r), clr)
      {}
    };
  }
}

#endif // PST_TREE_H__

