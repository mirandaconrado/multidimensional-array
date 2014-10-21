#ifndef __MULTIDIMENSIONAL_ARRAY__ARRAY_IMPL_HPP__
#define __MULTIDIMENSIONAL_ARRAY__ARRAY_IMPL_HPP__

#include "array.hpp"

namespace MultidimensionalArray {
  template <class T>
  Array<T>::Array():
    values_(nullptr),
    deallocate_on_destruction_(true) { }

  template <class T>
  Array<T>::Array(T const& other):
    size_({1}),
    values_(new T[1]),
    deallocate_on_destruction_(true) {
      values_[0] = other;
    }

  template <class T>
  Array<T>::Array(T&& other):
    size_({1}),
    values_(new T[1]),
    deallocate_on_destruction_(true) {
      values_[0] = std::move(other);
    }

  template <class T>
  Array<T>::Array(Array const& other):
    size_(other.size_),
    values_(nullptr),
    deallocate_on_destruction_(other.deallocate_on_destruction_) {
      if (other.deallocate_on_destruction_) {
        if (get_total_size() > 0) {
          values_ = new T[get_total_size()];
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
    size_(std::move(other.size_)),
    values_(std::move(other.values_)),
    deallocate_on_destruction_(std::move(other.deallocate_on_destruction_)) {
      other.values_ = nullptr;
    }

  template <class T>
  template <class T2>
  Array<T>::Array(Array<T2> const& other):
    Array() {
      size_ = other.get_size();
      deallocate_on_destruction_ = true;
      if (get_total_size() > 0)
        values_ = new T[get_total_size()];

      copy(other.get_pointer());
    }

  template <class T>
  Array<T>::Array(ConstArray<T> const& other):
    size_(other.size_),
    values_(nullptr),
    deallocate_on_destruction_(true) {
      if (get_total_size() > 0) {
        values_ = new T[get_total_size()];
        copy(other.values_);
      }
    }

  template <class T>
  template <class T2>
  Array<T>::Array(ConstArray<T2> const& other):
    size_(other.size_),
    values_(nullptr),
    deallocate_on_destruction_(true) {
      if (get_total_size() > 0) {
        values_ = new T[get_total_size()];
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

      if (get_total_size() > 0)
        values_ = new T[get_total_size()];
    }

  template <class T>
  Array<T>::Array(Size const& size, T const* other):
    Array(size) {
      copy(other);
    }

  template <class T>
  Array<T>::Array(Size const& size, T* other, bool responsible_for_deleting):
    Array() {
      size_ = size;
      values_ = other;
      deallocate_on_destruction_ = responsible_for_deleting;
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
  Array<T>::Array(Size::SizeType const& size, T* other,
      bool responsible_for_deleting):
    Array(Size(size), other, responsible_for_deleting) { }

  template <class T>
  template <class T2>
  Array<T>::Array(Size::SizeType const& size, T2 const* other):
    Array(Size(size), other) { }

  template <class T>
  Array<T>::~Array() {
    cleanup();
  }

  template <class T>
  void Array<T>::swap(Array& other) {
    Size temp1 = std::move(other.size_);
    other.size_ = size_;
    size_ = std::move(temp1);

    T* temp2 = other.values_;
    other.values_ = values_;
    values_ = temp2;

    bool temp3 = other.deallocate_on_destruction_;
    other.deallocate_on_destruction_ = deallocate_on_destruction_;
    deallocate_on_destruction_ = temp3;
  }

  template <class T>
  void Array<T>::swap(Array&& other) {
    Size temp1 = std::move(other.size_);
    other.size_ = size_;
    size_ = std::move(temp1);

    T* temp2 = other.values_;
    other.values_ = values_;
    values_ = temp2;

    bool temp3 = other.deallocate_on_destruction_;
    other.deallocate_on_destruction_ = deallocate_on_destruction_;
    deallocate_on_destruction_ = temp3;
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

    T* temp_pointer = other.values_;
    other.values_ = values_;
    values_ = temp_pointer;

    bool temp_deallocate = other.deallocate_on_destruction_;
    other.deallocate_on_destruction_ = deallocate_on_destruction_;
    deallocate_on_destruction_ = temp_deallocate;

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
  ConstView<T> Array<T>::view() const {
    return ConstView<T>(*this);
  }

  template <class T>
  bool Array<T>::resize(Size const& size, bool allow_allocation) {
    if (size.get_total_size() != get_total_size()) {
      if (!deallocate_on_destruction_)
        return false;

      if (values_) {
        delete[] values_;
        values_ = nullptr;
      }
      if (size.get_total_size() > 0 && allow_allocation)
        values_ = new T[size.get_total_size()];
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
    assert(values_ != nullptr);
    return values_[size_.get_position_variadic(args...)];
  }

  template <class T>
  template <class... Args>
  T const& Array<T>::operator()(Args const&... args) const {
    assert(values_ != nullptr);
    return values_[size_.get_position_variadic(args...)];
  }

  template <class T>
  T& Array<T>::get(Size::SizeType const& index) {
    assert(values_ != nullptr);
    return values_[size_.get_position(index)];
  }

  template <class T>
  T const& Array<T>::get(Size::SizeType const& index) const {
    assert(values_ != nullptr);
    return values_[size_.get_position(index)];
  }

  template <class T>
  template <class T2>
  void Array<T>::copy(T2 const* other) {
    assert(values_ != nullptr);
    assert(other != nullptr);
    for (size_t i = 0; i < get_total_size(); i++)
      values_[i] = other[i];
  }

  template <class T>
  template <class T2>
  void Array<T>::copy(View<T2> const& other) {
    assert(values_ != nullptr);
    auto it1 = other.get_size().cbegin();
    auto it2 = other.get_size().cend();
    for (size_t i = 0; i < get_total_size() && it1 != it2; i++, ++it1)
      values_[i] = other.get(*it1);
  }

  template <class T>
  template <class T2>
  void Array<T>::copy(ConstView<T2> const& other) {
    assert(values_ != nullptr);
    auto it1 = other.get_size().cbegin();
    auto it2 = other.get_size().cend();
    for (size_t i = 0; i < get_total_size() && it1 != it2; i++, ++it1)
      values_[i] = other.get(*it1);
  }

  template <class T>
  void Array<T>::cleanup() {
    if (values_ != nullptr && deallocate_on_destruction_) {
      delete[] values_;
      values_ = nullptr;
    }

    size_ = Size();
  }
};

#endif
