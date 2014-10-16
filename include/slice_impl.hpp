#ifndef __MULTIDIMENSIONAL_ARRAY__SLICE_IMPL_HPP__
#define __MULTIDIMENSIONAL_ARRAY__SLICE_IMPL_HPP__

#include "slice.hpp"

namespace MultidimensionalArray {
  template <class T>
  Slice<T>::Slice(Array<T>& array, unsigned int dimension):
    array_(array) {
    // Can't slice things that only have one dimension, for instance
    assert(dimension+1 < array.size().size());
    assert(dimension < array.size().size());

    Size::SizeType left, right;

    for (unsigned int i = 0; i <= dimension; i++)
      left.push_back(array.size()[i]);

    for (unsigned int i = dimension+1; i < array.size().size(); i++)
      right.push_back(array.size()[i]);

    left_size_.set_size(std::move(left));
    right_size_.set_size(std::move(right));
  }

  template <class T>
  Array<T> Slice<T>::get_element(size_t index) {
    assert(index < get_total_left_size());

    Array<T> ret;
    ret.size_ = right_size_;
    ret.values_ = &array_.get_pointer()[get_total_right_size() * index];
    ret.deallocate_on_destruction_ = false;

    return ret;
  }

  template <class T>
  Array<T> const Slice<T>::get_element(size_t index) const {
    assert(index < get_total_left_size());

    Array<T> ret;
    ret.size_ = right_size_;
    ret.values_ = &array_.get_pointer()[get_total_right_size() * index];
    ret.deallocate_on_destruction_ = false;

    return ret;
  }
};

#endif
