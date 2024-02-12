#include <vector>

template <class P, class M> std::size_t offsetof(const M P::*member) {
  return (std::size_t) & (reinterpret_cast<P *>(0)->*member);
}

template <class P, class M> P *container_of(M *ptr, const M P::*member) {
  return (P *)((char *)ptr - offsetof(member));
}

// Mixin class for tree nodes.
template <class T> class TreeNode {
public:
  TreeNode<T>() : _parent(nullptr), _children() {}

  TreeNode<T>(T *parent) : _parent(parent), _children() {}

  const T *get() const { return static_cast<const T *>(this); }

  T *parent() const { return _parent; }

  T *child(int index) const { return _children[index]; }

  std::size_t size() const { return _children.size(); }

  void append_child(T *child) {
    _children.push_back(child);
    child->_parent = this;
  }

  template <class... Args> void append_child(Args... args) {
    append_child(new T(args...));
  }

  void prepend_child(T *child) {
    _children.insert(_children.begin(), child);
    child->_parent = this;
  }

  template <class... Args> void prepend_child(Args... args) {
    prepend_child(new T(args...));
  }

  void insert_before(T *oldChild, T *newChild) {
    for (auto it = _children.begin(); it != _children.end(); it++) {
      if (*it == oldChild) {
        _children.insert(it, newChild);
        newChild->_parent = this;
        return;
      }
    }
  }

  void insert_after(T *oldChild, T *newChild) {
    for (auto it = _children.begin(); it != _children.end(); it++) {
      if (*it == oldChild) {
        _children.insert(it + 1, newChild);
        newChild->_parent = this;
        return;
      }
    }
  }

protected:
  TreeNode<T> *_parent;
  std::vector<TreeNode<T> *> _children;
};
