#pragma once

#ifndef PST_TREE_ITERATOR_H__
#define PST_TREE_ITERATOR_H__

#include <iterator>
#include <vector>

namespace pst
{
  namespace tree
  {
    template <typename Tree>
    struct tree_iterator : public std::iterator<std::forward_iterator_tag, typename Tree::key_value_type const>
    {
      tree_iterator() {}

      tree_iterator(tree_iterator const& ti)
        : path(ti.path)
      {}

      tree_iterator(Tree const& t)
      {
        descend_left(t);
      }

      tree_iterator& operator=(tree_iterator const& other) {
        path = other.path;
        return *this;
      }

      friend bool operator==(tree_iterator const& lhs, tree_iterator const& rhs) {
        lhs.path == rhs.path;
      }

      friend bool operator!=(tree_iterator const& lhs, tree_iterator const& rhs) {
        return !(lhs == rhs);
      }

      value_type operator*() const {
        return tree().keyval();
      }

      tree_iterator& operator++()
      {
        if (!tree().right().empty()) {
          descend_left(tree().right());
        }
        else {
          bool from_left = false;
          do {
            if (!has_parent()) {
              path.clear();
              break;
            }

            from_left = is_left_child();
            ascend();
          } while (!from_left);
        }

        return *this;
      }

      tree_iterator operator++(int)
      {
        tree_iterator here(*this);
        ++*this;
        return here;
      }

    private:
      bool has_parent() const
      {
        return 1 < path.size();
      }

      Tree const& parent() const
      {
        assert(has_parent());
        return *path[path.size() - 2];
      }

      bool is_left_child() const
      {
        assert(has_parent());
        return parent().left() == tree();
      }

      Tree const& tree() const
      {
        return *path.back();
      }

      bool has_left_child() const
      {
        return !tree().left().empty();
      }

      void ascend()
      {
        assert(has_parent());
        path.pop_back();
      }

      void descend_left(Tree const& t)
      {
        if (!t.empty()) {
          path.push_back(&t);
          descend_left(tree().left());
        }
      }

    private:
      std::vector<Tree const*> path;
    };
  }
}

#endif // PST_TREE_ITERATOR_H__
