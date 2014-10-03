#ifndef __MULTIDIMENSIONAL_ARRAY__CONST_ARRAY_IMPL_HPP__
#define __MULTIDIMENSIONAL_ARRAY__CONST_ARRAY_IMPL_HPP__

#include "const_array.hpp"

#include <cassert>

namespace MultidimensionalArray {
  template <class T>
  ConstArray<T>::ConstArray():
    total_size_(0),
    values_(nullptr),
    deallocate_on_destruction_(false) { }

  template <class T>
  ConstArray<T>::ConstArray(ConstArray const& other):
    total_size_(other.total_size_),
    size_(other.size_),
    values_(other.values_),
    deallocate_on_destruction_(false) { }

  template <class T>
  ConstArray<T>::ConstArray(ConstArray&& other):
    total_size_(std::move(other.total_size_)),
    size_(std::move(other.size_)),
    values_(std::move(other.values_)),
    deallocate_on_destruction_(std::move(other.deallocate_on_destruction_)) {
      other.values_ = nullptr;
    }

  template <class T>
  ConstArray<T>::ConstArray(Array<T> const& other):
    total_size_(other.total_size_),
    size_(other.size_),
    values_(other.values_),
    deallocate_on_destruction_(false) { }

  template <class T>
  ConstArray<T>::ConstArray(Array<T>&& other):
    total_size_(std::move(other.total_size_)),
    size_(std::move(other.size_)),
    values_(std::move(other.values_)),
    deallocate_on_destruction_(std::move(other.deallocate_on_destruction_)) {
      other.values_ = nullptr;
    }

  template <class T>
  ConstArray<T>::ConstArray(SizeType const& size):
    ConstArray() {
      size_ = size;

      total_size_ = 1;
      for (auto v : size_)
        total_size_ *= v;
    }

  template <class T>
  ConstArray<T>::ConstArray(SizeType const& size, T const* ptr,
      bool responsible_for_deleting):
    ConstArray(size) {
      values_ = ptr;
      deallocate_on_destruction_ = responsible_for_deleting;
    }

  template <class T>
  ConstArray<T>::~ConstArray() {
    cleanup();
  }

  template <class T>
  bool ConstArray<T>::resize(SizeType const& size) {
    size_t new_total_size = 1;
    for (auto v : size)
      new_total_size *= v;

    if (new_total_size != total_size_)
      return false;

    total_size_ = new_total_size;
    size_ = size;

    return true;
  }

  template <class T>
  void ConstArray<T>::set_pointer(T const* ptr, bool responsible_for_deleting) {
    if (values_ != nullptr && deallocate_on_destruction_) {
      delete[] values_;
      values_ = nullptr;
    }

    values_ = ptr;
    deallocate_on_destruction_ = responsible_for_deleting;
  }

  template <class T>
  template <class... Args>
  T const& ConstArray<T>::operator()(Args const&... args) const {
    assert(values_ != nullptr);
    assert(sizeof...(args) == size_.size());
    unsigned int indexes[] = {static_cast<SizeType::value_type>(args)...};
    assert(check_index(indexes));

    size_t position = indexes[0];
    for (unsigned int i = 0; i < size_.size()-1; i++) {
      position *= size_[i+1];
      position += indexes[i+1];
    }

    return values_[position];
  }

  template <class T>
  void ConstArray<T>::cleanup() {
    if (values_ != nullptr && deallocate_on_destruction_) {
      delete[] values_;
      values_ = nullptr;
    }

    total_size_ = 0;
    size_.clear();
  }

  template <class T>
  bool ConstArray<T>::same_size(SizeType const& other_size) const {
    if (size_.size() != other_size.size())
      return false;

    for (unsigned int i = 0; i < size_.size(); i++)
      if (size_[i] != other_size[i])
        return false;

    return true;
  }

  template <class T>
  bool ConstArray<T>::check_index(SizeType const& indexes) const {
    if (size_.size() != indexes.size())
      return false;

    for (unsigned int i = 0; i < size_.size(); i++)
      if (indexes[i] >= size_[i])
        return false;

    return true;
  }

  template <class T>
  bool ConstArray<T>::check_index(unsigned int const indexes[]) const {
    for (unsigned int i = 0; i < size_.size(); i++)
      if (indexes[i] >= size_[i])
        return false;

    return true;
  }
};

#endif
