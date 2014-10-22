#ifndef __MULTIDIMENSIONAL_ARRAY__SLICE_HPP__
#define __MULTIDIMENSIONAL_ARRAY__SLICE_HPP__

#include "size.hpp"

namespace MultidimensionalArray {
  template <class T>
  class Array;

  template <class T>
  class Slice {
    public:
      typedef T value_type;

      Slice(Array<T>& array, unsigned int dimension);

      Size const& left_size() const { return left_size_; }
      size_t total_left_size() const { return left_size_.total_size(); }

      Size const& right_size() const { return right_size_; }
      size_t total_right_size() const { return right_size_.total_size(); }

      Array<T> get_element(size_t index);
      Array<T> const get_element(size_t index) const;

    private:
      Slice(Slice<T> const& other);
      Slice const& operator=(Slice<T> const& other);

      Array<T>& array_;
      Size left_size_, right_size_;
  };
};

#include "slice_impl.hpp"

#endif
