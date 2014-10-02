#ifndef __MULTIDIMENSIONAL_ARRAY__SLICE_HPP__
#define __MULTIDIMENSIONAL_ARRAY__SLICE_HPP__

#include "array.hpp"

namespace MultidimensionalArray {
  template <class T>
  class Slice {
    public:
      Slice(Array<T>& array, unsigned int dimension);

      typename Array<T>::SizeType const& get_left_size() const {
        return left_size_;
      }
      typename Array<T>::SizeType const& get_right_size() const {
        return right_size_;
      }
      size_t get_total_left_size() const { return total_left_size_; }
      size_t get_total_right_size() const { return total_right_size_; }

      Array<T> get_element(size_t index);
      Array<T> const get_element(size_t index) const;

    private:
      Slice(Slice<T> const& other);
      Slice const& operator=(Slice<T> const& other);

      Array<T>& array_;
      typename Array<T>::SizeType left_size_, right_size_;
      size_t total_left_size_, total_right_size_;
  };
};

#include "slice_impl.hpp"

#endif
