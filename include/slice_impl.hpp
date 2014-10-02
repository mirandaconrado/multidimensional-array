#ifndef __MULTIDIMENSIONAL_ARRAY__SLICE_IMPL_HPP__
#define __MULTIDIMENSIONAL_ARRAY__SLICE_IMPL_HPP__

#include "slice.hpp"

namespace MultidimensionalArray {
  template <class T>
  Slice<T>::Slice(Array<T>& array, unsigned int dimension):
    array_(array) {
    // Can't slice things that only have one dimension, for instance
    assert(dimension+1 < array.get_size().size());

    total_left_size_ = 1;
    for (unsigned int i = 0; i <= dimension; i++) {
      total_left_size_ *= array.get_size()[i];
      left_size_.push_back(array.get_size()[i]);
    }

    total_right_size_ = 1;
    for (unsigned int i = dimension+1; i < array.get_size().size(); i++) {
      total_right_size_ *= array.get_size()[i];
      right_size_.push_back(array.get_size()[i]);
    }
  }

  template <class T>
  Array<T> Slice<T>::get_element(size_t index) {
    assert(index < total_left_size_);

    Array<T> ret;
    ret.total_size_ = total_right_size_;
    ret.size_ = right_size_;
    ret.values_ = &array_.get_pointer()[total_right_size_ * index];
    ret.deallocate_on_destruction_ = false;

    return ret;
  }

  template <class T>
  Array<T> const Slice<T>::get_element(size_t index) const {
    assert(index < total_left_size_);

    Array<T> ret;
    ret.total_size_ = total_right_size_;
    ret.size_ = right_size_;
    ret.values_ = &array_.get_pointer()[total_right_size_ * index];
    ret.deallocate_on_destruction_ = false;

    return ret;
  }
};

#endif
