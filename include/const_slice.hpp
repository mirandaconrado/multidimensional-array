#ifndef __MULTIDIMENSIONAL_ARRAY__CONST_SLICE_HPP__
#define __MULTIDIMENSIONAL_ARRAY__CONST_SLICE_HPP__

#include "size.hpp"

namespace MultidimensionalArray {
  template <class T>
  class ConstArray;

  template <class T>
  class ConstSlice {
    public:
      typedef T value_type;

      ConstSlice(ConstArray<T> const& array, unsigned int dimension);

      Size const& left_size() const { return left_size_; }
      size_t total_left_size() const { return left_size_.total_size(); }

      Size const& right_size() const { return right_size_; }
      size_t total_right_size() const { return right_size_.total_size(); }

      ConstArray<T> get_element(size_t index);
      ConstArray<T> const get_element(size_t index) const;

    private:
      ConstSlice(ConstSlice<T> const& other);
      ConstSlice const& operator=(ConstSlice<T> const& other);

      ConstArray<T> const& array_;
      Size left_size_, right_size_;
  };
};

#include "const_slice_impl.hpp"

#endif
