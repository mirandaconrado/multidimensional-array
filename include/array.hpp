#ifndef __MULTIDIMENSIONAL_ARRAY__ARRAY_HPP__
#define __MULTIDIMENSIONAL_ARRAY__ARRAY_HPP__

#include "const_array.hpp"

namespace MultidimensionalArray {
  template <class T>
  class ConstArray;

  template <class T>
  class Slice;

  template <class T>
  class Array {
    public:
      typedef std::vector<unsigned int> SizeType;

      Array();

      Array(Array const& other);
      Array(Array&& other);
      template <class T2>
      Array(Array<T2> const& other);

      Array(ConstArray<T> const& other);

      Array(SizeType const& size);
      Array(SizeType const& size, T const* other);
      template <class T2>
      Array(SizeType const& size, T2 const* other);

      ~Array();

      void copy(T const* other);
      template <class T2>
      void copy(T2 const* other);

      Array const& operator=(Array const& other);
      Array const& operator=(Array&& other);
      template <class T2>
      Array const& operator=(Array<T2> const& other);

      bool resize(SizeType const& size, bool allow_allocation = true);

      SizeType const& get_size() const { return size_; }
      size_t get_total_size() const { return total_size_; }

      void set_pointer(T* p, bool responsible_for_deleting = true);
      T* get_pointer() { return values_; }
      T const* get_pointer() const { return values_; }

      template <class... Args>
      T& operator()(Args const&... args);
      template <class... Args>
      T const& operator()(Args const&... args) const;

    private:
      friend class ConstArray<T>;
      friend class Slice<T>;

      void cleanup();

      bool same_size(SizeType const& other_size) const;
      bool check_index(SizeType const& indexes) const;
      bool check_index(unsigned int const indexes[]) const;

      size_t total_size_;
      SizeType size_;
      T* values_;
      bool deallocate_on_destruction_;
  };
};

#include "array_impl.hpp"

#endif
