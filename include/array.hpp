#ifndef __MULTIDIMENSIONAL_ARRAY__ARRAY_HPP__
#define __MULTIDIMENSIONAL_ARRAY__ARRAY_HPP__

#include "size.hpp"

#include <initializer_list>

namespace MultidimensionalArray {
  template <class T>
  class ConstArray;

  template <class T>
  class ConstView;

  template <class T>
  class Slice;

  template <class T>
  class View;

  template <class T>
  class Array {
    public:
      typedef T value_type;

      Array();

      Array(T const& other);
      Array(T&& other);

      Array(Array const& other);
      Array(Array&& other);
      template <class T2>
      Array(Array<T2> const& other);

      Array(ConstArray<T> const& other);
      template <class T2>
      Array(ConstArray<T2> const& other);

      Array(View<T> const& other);
      template <class T2>
      Array(View<T2> const& other);

      Array(ConstView<T> const& other);
      template <class T2>
      Array(ConstView<T2> const& other);

      Array(Size const& size);
      Array(Size const& size, T const* other);
      Array(Size const& size, T* other, bool responsible_for_deleting = false);
      template <class T2>
      Array(Size const& size, T2 const* other);

      Array(Size::SizeType const& size);
      Array(Size::SizeType const& size, T const* other);
      Array(Size::SizeType const& size, T* other,
          bool responsible_for_deleting = false);
      template <class T2>
      Array(Size::SizeType const& size, T2 const* other);

      ~Array();

      void swap(Array& other);
      void swap(Array&& other);

      Array const& operator=(Array const& other);
      Array const& operator=(Array&& other);
      template <class T2>
      Array const& operator=(Array<T2> const& other);
      Array const& operator=(ConstArray<T> const& other);
      template <class T2>
      Array const& operator=(ConstArray<T2> const& other);
      Array const& operator=(View<T> const& other);
      template <class T2>
      Array const& operator=(View<T2> const& other);
      Array const& operator=(ConstView<T> const& other);
      template <class T2>
      Array const& operator=(ConstView<T2> const& other);

      View<T> view();
      ConstView<T> view() const;

      bool resize(Size const& size, bool allow_allocation = true);
      bool resize(Size::SizeType const& size, bool allow_allocation = true);

      Size::SizeType const& size() const { return size_.get_size(); }
      Size const& get_size() const { return size_; }
      size_t get_total_size() const { return size_.get_total_size(); }

      void set_pointer(T* p, bool responsible_for_deleting = true);
      T* get_pointer() { return values_; }
      T const* get_pointer() const { return values_; }

      template <class... Args>
      T& operator()(Args const&... args);
      template <class... Args>
      T const& operator()(Args const&... args) const;

      T& get(Size::SizeType const& index);
      T const& get(Size::SizeType const& index) const;

    private:
      friend class ConstArray<T>;
      friend class Slice<T>;

      template <class T2>
      void copy(T2 const* other);
      template <class T2>
      void copy(View<T2> const& other);
      template <class T2>
      void copy(ConstView<T2> const& other);

      void cleanup();

      Size size_;
      T* values_;
      bool deallocate_on_destruction_;
  };
};

#include "array_impl.hpp"

#endif
