#ifndef __MULTIDIMENSIONAL_ARRAY__VIEW_HPP__
#define __MULTIDIMENSIONAL_ARRAY__VIEW_HPP__

#include "size.hpp"

namespace MultidimensionalArray {
  template <class T>
  class Array;

  template <class T>
  class ConstArray;

  template <class T>
  class ConstView;

  template <class T>
  class View {
    public:
      View(View const& other);
      View(View&& other);

      View const& operator=(Array<T> const& other);
      template <class T2>
      View const& operator=(Array<T2> const& other);
      View const& operator=(ConstArray<T> const& other);
      template <class T2>
      View const& operator=(ConstArray<T2> const& other);
      View const& operator=(View const& other);
      template <class T2>
      View const& operator=(View<T2> const& other);
      View const& operator=(ConstView<T> const& other);
      template <class T2>
      View const& operator=(ConstView<T2> const& other);

      Size::SizeType const& size() const { return size_.get_size(); }
      Size const& get_size() const { return size_; }
      size_t get_total_size() const { return size_.get_total_size(); }

      template <class... Args>
      T& operator()(Args const&... args);
      template <class... Args>
      T const& operator()(Args const&... args) const;

      T& get(Size::SizeType const& index);
      T const& get(Size::SizeType const& index) const;

      View set_range_begin(size_t dimension, size_t value) const;
      View set_range_end(size_t dimension, size_t value) const;
      View set_range_stride(size_t dimension, size_t value) const;
      View fix_dimension(size_t dimension, size_t value) const;

    private:
      friend class Array<T>;
      friend class ConstView<T>;

      View(Array<T>& array);

      template <class T2>
      void copy(T2 const* other);
      template <class T2>
      void copy(View<T2> const& other);
      template <class T2>
      void copy(ConstView<T2> const& other);

      Array<T>& array_;
      Size size_;
      bool original_view_;
      Size::SizeType dimension_map_;
      Size::SizeType offset_, gain_, fixed_values_;
      std::vector<bool> fixed_flag_;
  };
};

#include "view_impl.hpp"

#endif
