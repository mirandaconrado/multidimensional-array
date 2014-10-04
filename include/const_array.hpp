#ifndef __MULTIDIMENSIONAL_ARRAY__CONST_ARRAY_HPP__
#define __MULTIDIMENSIONAL_ARRAY__CONST_ARRAY_HPP__

#include <cstdlib>
#include <vector>

namespace MultidimensionalArray {
  template <class T>
  class Array;

  template <class T>
  class ConstSlice;

  template <class T>
  class ConstArray {
    public:
      typedef std::vector<unsigned int> SizeType;

      ConstArray();

      ConstArray(ConstArray const& other);
      ConstArray(ConstArray&& other);

      ConstArray(Array<T> const& other);
      ConstArray(Array<T>&& other);

      ConstArray(SizeType const& size);
      ConstArray(SizeType const& size, T const* ptr,
          bool responsible_for_deleting = false);

      ~ConstArray();

      bool resize(SizeType const& size);

      SizeType const& get_size() const { return size_; }
      size_t get_total_size() const { return total_size_; }

      void set_pointer(T const* ptr, bool responsible_for_deleting = false);
      T const* get_pointer() const { return values_; }

      template <class... Args>
      T const& operator()(Args const&... args) const;

    private:
      friend class Array<T>;
      friend class ConstSlice<T>;

      void cleanup();

      bool same_size(SizeType const& other_size) const;
      bool check_index(SizeType const& indexes) const;
      bool check_index(unsigned int const indexes[]) const;

      size_t total_size_;
      SizeType size_;
      T const* values_;
      bool deallocate_on_destruction_;
  };
};

#include "const_array_impl.hpp"

#endif
