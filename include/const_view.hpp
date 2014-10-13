#ifndef __MULTIDIMENSIONAL_ARRAY__CONST_VIEW_HPP__
#define __MULTIDIMENSIONAL_ARRAY__CONST_VIEW_HPP__

#include "size.hpp"

namespace MultidimensionalArray {
  template <class T>
  class Array;

  template <class T>
  class ConstArray;

  template <class T>
  class View;

  template <class T>
  class ConstView {
    public:
      ConstView(ConstView const& other);
      ConstView(ConstView&& other);

      ConstView(View<T> const& other);
      ConstView(View<T>&& other);

      Size::SizeType const& size() const { return size_.get_size(); }
      Size const& get_size() const { return size_; }
      size_t get_total_size() const { return size_.get_total_size(); }

      template <class... Args>
      T const& operator()(Args const&... args) const;

      T const& get(Size::SizeType const& index) const;

      ConstView set_range_begin(size_t dimension, size_t value);
      ConstView set_range_end(size_t dimension, size_t value);
      ConstView set_range_stride(size_t dimension, size_t value);
      ConstView fix_dimension(size_t dimension, size_t value);

    private:
      friend class Array<T>;
      friend class ConstArray<T>;

      ConstView(Array<T> const& array);
      ConstView(ConstArray<T> const& array);

      T const* get_pointer() const;

      template <class... Args>
      size_t get_position_variadic(Args const&... args) const;
      size_t get_position(Size::SizeType::value_type const* indexes) const;

      Array<T> const* array_;
      ConstArray<T> const* carray_;
      Size size_;
      bool original_view_;
      Size::SizeType dimension_map_;
      Size::SizeType offset_, gain_, fixed_values_;
      std::vector<bool> fixed_flag_;
  };
};

#include "const_view_impl.hpp"

#endif
