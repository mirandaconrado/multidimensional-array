#ifndef __MULTIDIMENSIONAL_ARRAY__CONST_ARRAY_IMPL_HPP__
#define __MULTIDIMENSIONAL_ARRAY__CONST_ARRAY_IMPL_HPP__

#include "const_array.hpp"

namespace MultidimensionalArray {
  template <class T>
  ConstArray<T>::ConstArray():
    values_(nullptr),
    deallocate_on_destruction_(false) { }

  template <class T>
  ConstArray<T>::ConstArray(ConstArray const& other):
    size_(other.size_),
    values_(other.values_),
    deallocate_on_destruction_(false) { }

  template <class T>
  ConstArray<T>::ConstArray(ConstArray&& other):
    size_(std::move(other.size_)),
    values_(std::move(other.values_)),
    deallocate_on_destruction_(std::move(other.deallocate_on_destruction_)) {
      other.values_ = nullptr;
    }

  template <class T>
  ConstArray<T>::ConstArray(Array<T> const& other):
    size_(other.size_),
    values_(other.values_),
    deallocate_on_destruction_(false) { }

  template <class T>
  ConstArray<T>::ConstArray(Array<T>&& other):
    size_(std::move(other.size_)),
    values_(std::move(other.values_)),
    deallocate_on_destruction_(std::move(other.deallocate_on_destruction_)) {
      other.values_ = nullptr;
    }

  template <class T>
  ConstArray<T>::ConstArray(Size const& size):
    ConstArray() {
      size_ = size;
    }

  template <class T>
  ConstArray<T>::ConstArray(Size const& size, T const* ptr,
      bool responsible_for_deleting):
    ConstArray(size) {
      values_ = ptr;
      deallocate_on_destruction_ = responsible_for_deleting;
    }

  template <class T>
  ConstArray<T>::ConstArray(Size::SizeType const& size):
    ConstArray(Size(size)) { }

  template <class T>
  ConstArray<T>::ConstArray(Size::SizeType const& size, T const* ptr,
      bool responsible_for_deleting):
    ConstArray(Size(size), ptr, responsible_for_deleting) { }

  template <class T>
  ConstArray<T>::~ConstArray() {
    cleanup();
  }

  template <class T>
  ConstView<T> ConstArray<T>::view() {
    return ConstView<T>(*this);
  }

  template <class T>
  bool ConstArray<T>::resize(Size const& size) {
    if (size.get_total_size() != get_total_size())
      return false;

    size_ = size;

    return true;
  }

  template <class T>
  bool ConstArray<T>::resize(Size::SizeType const& size) {
    return resize(Size(size));
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
    return values_[size_.get_position_variadic(args...)];
  }

  template <class T>
  T const& ConstArray<T>::get(Size::SizeType const& index) const {
    assert(values_ != nullptr);
    return values_[size_.get_position(index)];
  }

  template <class T>
  void ConstArray<T>::cleanup() {
    if (values_ != nullptr && deallocate_on_destruction_) {
      delete[] values_;
      values_ = nullptr;
    }

    size_ = Size();
  }
};

#endif
