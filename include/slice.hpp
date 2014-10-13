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

      Size::SizeType const& left_size() const { return left_size_.get_size(); }
      Size const& get_left_size() const { return left_size_; }
      size_t get_total_left_size() const { return left_size_.get_total_size(); }

      Size::SizeType const& right_size() const { return right_size_.get_size(); }
      Size const& get_right_size() const { return right_size_; }
      size_t get_total_right_size() const { return right_size_.get_total_size(); }

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
