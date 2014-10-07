#ifndef __MULTIDIMENSIONAL_ARRAY__ARRAY_IMPL_HPP__
#define __MULTIDIMENSIONAL_ARRAY__ARRAY_IMPL_HPP__

#include "array.hpp"

namespace MultidimensionalArray {
  template <class T>
  Array<T>::Array():
    total_size_(0),
    values_(nullptr),
    deallocate_on_destruction_(true) { }

  template <class T>
  Array<T>::Array(Array const& other):
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
  Array<T>::Array(Array&& other):
    total_size_(std::move(other.total_size_)),
    size_(std::move(other.size_)),
    values_(std::move(other.values_)),
    deallocate_on_destruction_(std::move(other.deallocate_on_destruction_)) {
      other.values_ = nullptr;
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
  Array<T>::Array(ConstArray<T2> const& other):
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
  Array<T>::Array(View<T> const& other):
    Array(other.get_size()) {
      *this = other;
    }

  template <class T>
  template <class T2>
  Array<T>::Array(View<T2> const& other):
    Array(other.get_size()) {
      *this = other;
    }

  template <class T>
  Array<T>::Array(ConstView<T> const& other):
    Array(other.get_size()) {
      *this = other;
    }

  template <class T>
  template <class T2>
  Array<T>::Array(ConstView<T2> const& other):
    Array(other.get_size()) {
      *this = other;
    }

  template <class T>
  Array<T>::Array(Size const& size):
    Array() {
      size_ = size;
      deallocate_on_destruction_ = true;

      total_size_ = 1;
      for (auto v : size_.get_size())
        total_size_ *= v;

      if (total_size_ > 0)
        values_ = new T[total_size_];
    }

  template <class T>
  Array<T>::Array(Size const& size, T const* other):
    Array(size) {
      copy(other);
    }

  template <class T>
  template <class T2>
  Array<T>::Array(Size const& size, T2 const* other):
    Array(size) {
      copy(other);
    }

  template <class T>
  Array<T>::Array(Size::SizeType const& size):
    Array(Size(size)) { }

  template <class T>
  Array<T>::Array(Size::SizeType const& size, T const* other):
    Array(Size(size), other) { }

  template <class T>
  template <class T2>
  Array<T>::Array(Size::SizeType const& size, T2 const* other):
    Array(Size(size), other) { }

  template <class T>
  Array<T>::~Array() {
    cleanup();
  }

  template <class T>
  Array<T> const& Array<T>::operator=(Array const& other) {
    assert(size_.same(other.get_size()));
    copy(other.values_);
    return *this;
  }

  template <class T>
  Array<T> const& Array<T>::operator=(Array&& other) {
    assert(size_.same(other.get_size()));

    if (values_ != nullptr)
      delete[] values_;

    values_ = other.values_;
    other.values_ = nullptr;
    return *this;
  }

  template <class T>
  template <class T2>
  Array<T> const& Array<T>::operator=(Array<T2> const& other) {
    assert(size_.same(other.get_size()));
    copy(other.get_pointer());
    return *this;
  }

  template <class T>
  Array<T> const& Array<T>::operator=(ConstArray<T> const& other) {
    assert(size_.same(other.get_size()));
    copy(other.get_pointer());
    return *this;
  }

  template <class T>
  template <class T2>
  Array<T> const& Array<T>::operator=(ConstArray<T2> const& other) {
    assert(size_.same(other.get_size()));
    copy(other.get_pointer());
    return *this;
  }

  template <class T>
  Array<T> const& Array<T>::operator=(View<T> const& other) {
    assert(size_.same(other.get_size()));
    copy(other);
    return *this;
  }

  template <class T>
  template <class T2>
  Array<T> const& Array<T>::operator=(View<T2> const& other) {
    assert(size_.same(other.get_size()));
    copy(other);
    return *this;
  }

  template <class T>
  Array<T> const& Array<T>::operator=(ConstView<T> const& other) {
    assert(size_.same(other.get_size()));
    copy(other);
    return *this;
  }

  template <class T>
  template <class T2>
  Array<T> const& Array<T>::operator=(ConstView<T2> const& other) {
    assert(size_.same(other.get_size()));
    copy(other);
    return *this;
  }

  template <class T>
  View<T> Array<T>::view() {
    return View<T>(*this);
  }

  template <class T>
  bool Array<T>::resize(Size const& size, bool allow_allocation) {
    size_t new_total_size = 1;
    for (auto v : size.get_size())
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
  bool Array<T>::resize(Size::SizeType const& size, bool allow_allocation) {
    return resize(Size(size), allow_allocation);
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
    return values_[get_position_variadic(args...)];
  }

  template <class T>
  template <class... Args>
  T const& Array<T>::operator()(Args const&... args) const {
    return values_[get_position_variadic(args...)];
  }

  template <class T>
  T& Array<T>::get(Size::SizeType const& index) {
    assert(index.size() == size_.size_.size());
    return values_[get_position(&index[0])];
  }

  template <class T>
  T const& Array<T>::get(Size::SizeType const& index) const {
    assert(index.size() == size_.size_.size());
    return values_[get_position(&index[0])];
  }

  template <class T>
  template <class... Args>
  size_t Array<T>::get_position_variadic(Args const&... args) const {
    assert(sizeof...(args) == size_.size_.size());
    Size::SizeType::value_type indexes[] =
    {static_cast<Size::SizeType::value_type>(args)...};

    return get_position(indexes);
  }

  template <class T>
  size_t Array<T>::get_position(
      Size::SizeType::value_type const* indexes) const {
    assert(values_ != nullptr);
    assert(size_.check_index(indexes, size_.size_.size()));

    size_t position = indexes[0];
    for (size_t i = 0; i < size_.size_.size()-1; i++) {
      position *= size_[i+1];
      position += indexes[i+1];
    }

    return position;
  }

  template <class T>
  template <class T2>
  void Array<T>::copy(T2 const* other) {
    assert(values_ != nullptr);
    for (size_t i = 0; i < total_size_; i++)
      values_[i] = other[i];
  }

  template <class T>
  template <class T2>
  void Array<T>::copy(View<T2> const& other) {
    assert(values_ != nullptr);
    auto it1 = other.get_size().begin();
    auto it2 = other.get_size().end();
    for (size_t i = 0; i < total_size_ && it1 != it2; i++, ++it1)
      values_[i] = other(*it1);
  }

  template <class T>
  template <class T2>
  void Array<T>::copy(ConstView<T2> const& other) {
    assert(values_ != nullptr);
    auto it1 = other.get_size().begin();
    auto it2 = other.get_size().end();
    for (size_t i = 0; i < total_size_ && it1 != it2; i++, ++it1)
      values_[i] = other(*it1);
  }

  template <class T>
  void Array<T>::cleanup() {
    if (values_ != nullptr && deallocate_on_destruction_) {
      delete[] values_;
      values_ = nullptr;
    }

    total_size_ = 0;
    size_.size_.clear();
  }
};

#endif
