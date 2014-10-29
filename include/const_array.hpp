#ifndef __MULTIDIMENSIONAL_ARRAY__CONST_ARRAY_HPP__
#define __MULTIDIMENSIONAL_ARRAY__CONST_ARRAY_HPP__

#include "size.hpp"

namespace MultidimensionalArray {
  template <class T>
  class Array;

  template <class T>
  class ConstSlice;

  template <class T>
  class ConstView;

  template <class T>
  class ConstArray {
    public:
      typedef T value_type;

      ConstArray();

      ConstArray(ConstArray const& other);
      ConstArray(ConstArray&& other);

      ConstArray(Array<T> const& other);
      ConstArray(Array<T>&& other);

      ConstArray(Size const& size);
      ConstArray(Size const& size, T const* ptr,
          bool responsible_for_deleting = false);

      ~ConstArray();

      void swap(ConstArray& other);
      void swap(ConstArray&& other);

      ConstView<T> view();

      bool resize(Size const& size);

      Size const& size() const { return size_; }
      size_t total_size() const { return size_.total_size(); }

      void set_pointer(T const* ptr, bool responsible_for_deleting = false);
      T const* get_pointer() const { return values_; }

      template <class... Args>
      T const& operator()(Args const&... args) const;

      T const& get(Size::SizeType const& index) const;

    private:
      friend class Array<T>;
      friend class ConstSlice<T>;

      void cleanup();

      Size size_;
      T const* values_;
      bool deallocate_on_destruction_;
  };
};

#include "const_array_impl.hpp"

#endif
