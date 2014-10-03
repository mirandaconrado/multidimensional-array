#ifndef __MULTIDIMENSIONAL_ARRAY__ARRAY_IMPL_HPP__
#define __MULTIDIMENSIONAL_ARRAY__ARRAY_IMPL_HPP__

#include "array.hpp"

#include <cassert>

namespace MultidimensionalArray {
  template <class T>
  Array<T>::Array():
    total_size_(0),
    values_(nullptr),
    deallocate_on_destruction_(true) { }

  template <class T>
  Array<T>::Array(Array<T> const& other):
    total_size_(other.total_size_),
    size_(other.size_),
    values_(nullptr),
    deallocate_on_destruction_(other.deallocate_on_destruction_) {
      if (other.deallocate_on_destruction_) {
        if (total_size_ > 0) {
          values_ = new T[total_size_];
          copy(other.values_);
        }
      }
      else {
        values_ = other.values_;
        deallocate_on_destruction_ = false;
      }
    }

  template <class T>
  Array<T>::Array(Array<T>&& other):
    total_size_(std::move(other.total_size_)),
    size_(std::move(other.size_)),
    values_(std::move(other.values_)),
    deallocate_on_destruction_(std::move(other.deallocate_on_destruction_)) {
      other.values_ = nullptr;
    }

  template <class T>
  Array<T>::Array(ConstArray<T> const& other):
    total_size_(other.total_size_),
    size_(other.size_),
    values_(nullptr),
    deallocate_on_destruction_(true) {
      if (total_size_ > 0) {
        values_ = new T[total_size_];
        copy(other.values_);
      }
    }

  template <class T>
  template <class T2>
  Array<T>::Array(Array<T2> const& other):
    Array() {
      total_size_ = other.get_total_size();
      size_ = other.get_size();
      deallocate_on_destruction_ = true;
      if (total_size_ > 0)
        values_ = new T[total_size_];

      copy(other.get_pointer());
    }

  template <class T>
  Array<T>::Array(SizeType const& size):
    Array() {
      size_ = size;
      deallocate_on_destruction_ = true;

      total_size_ = 1;
      for (auto v : size_)
        total_size_ *= v;

      if (total_size_ > 0)
        values_ = new T[total_size_];
    }

  template <class T>
  Array<T>::Array(SizeType const& size, T const* other):
    Array(size) {
      copy(other);
    }

  template <class T>
  template <class T2>
  Array<T>::Array(SizeType const& size, T2 const* other):
    Array(size) {
      copy(other);
    }

  template <class T>
  Array<T>::~Array() {
    cleanup();
  }

  template <class T>
  void Array<T>::copy(T const* other) {
    assert(values_ != nullptr);
    for (size_t i = 0; i < total_size_; i++)
      values_[i] = other[i];
  }

  template <class T>
  template <class T2>
  void Array<T>::copy(T2 const* other) {
    assert(values_ != nullptr);
    for (size_t i = 0; i < total_size_; i++)
      values_[i] = other[i];
  }

  template <class T>
  Array<T> const& Array<T>::operator=(Array<T> const& other) {
    assert(same_size(other.size_));

    copy(other.values_);
    return *this;
  }

  template <class T>
  Array<T> const& Array<T>::operator=(Array<T>&& other) {
    assert(same_size(other.size_));

    if (values_ != nullptr)
      delete[] values_;

    values_ = other.values_;
    other.values_ = nullptr;
    return *this;
  }

  template <class T>
  template <class T2>
  Array<T> const& Array<T>::operator=(Array<T2> const& other) {
    assert(same_size(other.get_size()));

    copy(other.get_pointer());
    return *this;
  }

  template <class T>
  bool Array<T>::resize(SizeType const& size, bool allow_allocation) {
    size_t new_total_size = 1;
    for (auto v : size)
      new_total_size *= v;

    if (new_total_size != total_size_) {
      if (!deallocate_on_destruction_)
        return false;

      total_size_ = new_total_size;
      if (values_) {
        delete[] values_;
        values_ = nullptr;
      }
      if (total_size_ > 0 && allow_allocation)
        values_ = new T[total_size_];
    }

    size_ = size;

    return true;
  }

  template <class T>
  void Array<T>::set_pointer(T* p, bool responsible_for_deleting) {
    if (values_ != nullptr && deallocate_on_destruction_) {
      delete[] values_;
      values_ = nullptr;
    }

    values_ = p;
    deallocate_on_destruction_ = responsible_for_deleting;
  }

  template <class T>
  template <class... Args>
  T& Array<T>::operator()(Args const&... args) {
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
  template <class... Args>
  T const& Array<T>::operator()(Args const&... args) const {
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
  void Array<T>::cleanup() {
    if (values_ != nullptr && deallocate_on_destruction_) {
      delete[] values_;
      values_ = nullptr;
    }

    total_size_ = 0;
    size_.clear();
  }

  template <class T>
  bool Array<T>::same_size(SizeType const& other_size) const {
    if (size_.size() != other_size.size())
      return false;

    for (unsigned int i = 0; i < size_.size(); i++)
      if (size_[i] != other_size[i])
        return false;

    return true;
  }

  template <class T>
  bool Array<T>::check_index(SizeType const& indexes) const {
    if (size_.size() != indexes.size())
      return false;

    for (unsigned int i = 0; i < size_.size(); i++)
      if (indexes[i] >= size_[i])
        return false;

    return true;
  }

  template <class T>
  bool Array<T>::check_index(unsigned int const indexes[]) const {
    for (unsigned int i = 0; i < size_.size(); i++)
      if (indexes[i] >= size_[i])
        return false;

    return true;
  }
};

#endif
