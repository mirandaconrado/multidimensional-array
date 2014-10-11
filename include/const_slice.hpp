#ifndef __MULTIDIMENSIONAL_ARRAY__CONST_SLICE_HPP__
#define __MULTIDIMENSIONAL_ARRAY__CONST_SLICE_HPP__

#include "size.hpp"

namespace MultidimensionalArray {
  template <class T>
  class ConstArray;

  template <class T>
  class ConstSlice {
    public:
      ConstSlice(ConstArray<T>& array, unsigned int dimension);

      Size::SizeType const& left_size() const { return left_size_.get_size(); }
      Size const& get_left_size() const { return left_size_; }
      size_t get_total_left_size() const { return left_size_.get_total_size(); }

      Size::SizeType const& right_size() const { return right_size_.get_size(); }
      Size const& get_right_size() const { return right_size_; }
      size_t get_total_right_size() const { return right_size_.get_total_size(); }

      ConstArray<T> get_element(size_t index);
      ConstArray<T> const get_element(size_t index) const;

    private:
      ConstSlice(ConstSlice<T> const& other);
      ConstSlice const& operator=(ConstSlice<T> const& other);

      ConstArray<T>& array_;
      Size left_size_, right_size_;
  };
};

#include "const_slice_impl.hpp"

#endif
