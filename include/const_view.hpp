#ifndef __MULTIDIMENSIONAL_ARRAY__CONST_VIEW_HPP__
#define __MULTIDIMENSIONAL_ARRAY__CONST_VIEW_HPP__

#include "const_array.hpp"

namespace MultidimensionalArray {
  template <class T>
  class ConstView {
    public:
      ConstView(ConstView const& other);
      ConstView(ConstView&& other);

      template <class... Args>
      T const& operator()(Args const&... args) const;

      T const& get_value(unsigned int const indexes[]) const;

      ConstView set_range_begin(unsigned int dimension, unsigned int value);
      ConstView set_range_end(unsigned int dimension, unsigned int value);
      ConstView set_range_stride(unsigned int dimension, unsigned int value);
      ConstView fix_dimension(unsigned int dimension, unsigned int value);

      typename ConstArray<T>::SizeType get_size() const { return size_; }

    private:
      friend class ConstArray<T>;

      ConstView(ConstArray<T>& array);

      bool same_size(typename ConstArray<T>::SizeType const& other_size) const;
      bool check_index(unsigned int const indexes[]) const;

      ConstArray<T>& array_;
      typename ConstArray<T>::SizeType size_, dimension_map_;
      typename ConstArray<T>::SizeType offset_, gain_, fixed_values_;
      std::vector<bool> fixed_flag_;
  };
};

#include "const_view_impl.hpp"

#endif
